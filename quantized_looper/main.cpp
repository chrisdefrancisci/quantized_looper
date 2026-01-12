#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

#include <reusable_synth/software/task.hpp>
#include <reusable_synth/utils/logger.hpp>

#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"
#include <gpio.h>
#include <main.h>
#include <quantized_looper/Hardware/led.hpp>
#include <tim.h>
#include <usart.h>

extern "C"
{
    extern void SystemClock_Config();
}

class LoggerSingleton
{
public:
    constexpr static int nLogs = 20;
    constexpr static int logLen = 200;
    static Logger<nLogs, logLen>* getLogger()
    {
        static Logger<nLogs, logLen> instance;
        return &instance;
    }

    LoggerSingleton(LoggerSingleton const&) = delete;
    void operator=(LoggerSingleton const&) = delete;

private:
    LoggerSingleton() {}
};

auto logger = LoggerSingleton::getLogger();

std::vector<std::unique_ptr<ledBase>>* g_leds = nullptr;

// Tap tempo globals
static volatile uint32_t last_tap_time = 0;
static volatile uint32_t cycle_time_ms = 1000; // Default 60 BPM (1 second)
static constexpr uint32_t MIN_CYCLE_TIME = 60;
static constexpr uint32_t MAX_CYCLE_TIME = 3000; // Min 20 BPM

// LED cycle tracking
static uint32_t last_update_time = 0;

extern UART_HandleTypeDef huart3;

void fade_led0()
{
    static float led_pct = 0.0f;
    static int direction = 1;

    uint32_t current_time = HAL_GetTick();

    if (last_update_time == 0) {
        last_update_time = current_time;
    }

    uint32_t elapsed = current_time - last_update_time;
    float change = direction * 2.0f * (float)elapsed / (float)cycle_time_ms;

    led_pct += change;

    if (led_pct >= 1.0f) {
        led_pct = 2.0f - led_pct; // Reflect over 1.0
        direction = -1;
    } else if (led_pct < 0.0f) {
        led_pct = -led_pct; // Reflect over 0
        direction = 1;
    }

    auto range = (*g_leds)[0]->getRange();
    int final_amt = range.first + (int)((range.second - range.first) * led_pct);

    (*g_leds)[0]->setIntensity(final_amt);
    last_update_time = current_time; // CRITICAL: Update for next call
}

// Task: Toggle LED 1 and log
void toggle_led1()
{
    static bool led_state = false;
    if (led_state) {
        (*g_leds)[1]->off();
    } else {
        (*g_leds)[1]->on();
    }
    led_state = !led_state;
    logger->info("LED 1 toggled");
}

// Task: Toggle LED 2 and log
void toggle_led2()
{
    static bool led_state = false;
    if (led_state) {
        (*g_leds)[2]->off();
    } else {
        (*g_leds)[2]->on();
    }
    led_state = !led_state;
    logger->info("LED 2 toggled");
}

void task_print_logs()
{
    auto log = logger->remove_log();
    if (log.has_value()) {
        const char* msg = log->pBuffer();
        HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 100);
        HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n", 2, 100);
    }
}

extern "C" void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_Btn_Pin) {
        uint32_t current_time = HAL_GetTick();

        // Debounce: ignore presses within 200ms of last press
        static uint32_t last_press_time = 0;
        if (current_time - last_press_time < 50) {
            return; // Too soon, ignore
        }

        last_press_time = current_time;

        if (last_tap_time > 0) {
            uint32_t time_diff = current_time - last_tap_time;

            // Clamp to reasonable BPM range
            if (time_diff >= MIN_CYCLE_TIME && time_diff <= MAX_CYCLE_TIME) {
                cycle_time_ms = time_diff;
                logger->info("BPM updated");
            }
        }

        last_tap_time = current_time;
    }
}

int main()
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0); // Lower priority than system
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    MX_TIM3_Init();
    MX_USART3_UART_Init();

    std::vector<std::unique_ptr<ledBase>> leds;
    leds.push_back(std::make_unique<led<TIM_HandleTypeDef>>(
      &htim3, TIM_CHANNEL_3, MX_TIM3_Init, MX_TIM3_DeInit));

    leds.push_back(std::make_unique<led<GPIO_TypeDef>>(LD2_GPIO_Port, LD2_Pin));
    leds.push_back(std::make_unique<led<GPIO_TypeDef>>(LD3_GPIO_Port, LD3_Pin));

    g_leds = &leds;

    std::array<task_control_block<uint32_t>, 4> tasks = {
        task_control_block<uint32_t>(fade_led0,
                                     HAL_GetTick,
                                     timer<uint32_t>::milliseconds(20),
                                     timer<uint32_t>::milliseconds(0)),
        task_control_block<uint32_t>(toggle_led1,
                                     HAL_GetTick,
                                     timer<uint32_t>::milliseconds(800),
                                     timer<uint32_t>::milliseconds(0)),
        task_control_block<uint32_t>(toggle_led2,
                                     HAL_GetTick,
                                     timer<uint32_t>::milliseconds(600),
                                     timer<uint32_t>::milliseconds(0)),
        task_control_block<uint32_t>(task_print_logs,
                                     HAL_GetTick,
                                     timer<uint32_t>::milliseconds(100),
                                     timer<uint32_t>::milliseconds(0))
    };

    scheduler(tasks);
}

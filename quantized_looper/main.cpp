#include <memory>
#include <vector>

#include <gpio.h>
#include <main.h>
#include <tim.h>

#include "stm32f767xx.h"
#include <quantized_looper/Hardware/led.hpp>

extern "C"
{
    extern void SystemClock_Config();
}

void
test()
{
    int* a;
};

int
main()
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM3_Init();

    std::vector<std::unique_ptr<ledBase>> leds;
    leds.push_back(std::make_unique<led<TIM_HandleTypeDef>>(
      &htim3, TIM_CHANNEL_3, MX_TIM3_Init, MX_TIM3_DeInit));

    leds.push_back(std::make_unique<led<GPIO_TypeDef>>(LD2_GPIO_Port, LD2_Pin));
    leds.push_back(std::make_unique<led<GPIO_TypeDef>>(LD3_GPIO_Port, LD3_Pin));

    int analogValue = 0;
    int increment = 6000;
    int onIdx = 0;

    while (true) {
        for (int i = 0; i < leds.size(); i++) {
            if (i == onIdx) {
                leds[i]->setIntensity(analogValue);
            } else {
                leds[i]->off();
            }
        }

        HAL_Delay(500);
        onIdx++;
        onIdx = onIdx >= leds.size() ? 0 : onIdx;

        analogValue += increment;
        if (analogValue > leds[0]->getRange().second ||
            analogValue < leds[0]->getRange().first) {
            for (int i = 0; i < 4; i++) {
                for (auto&& led : leds) {
                    led->on();
                }
                HAL_Delay(300);
                for (auto&& led : leds) {
                    led->off();
                }
                HAL_Delay(100);
            }
            increment = -increment;
        }
    }
}

/**
 * @file led_tasks.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief This file declares classes for how the available LEDs should be
 * updated over time.
 * @date 2026-01-16
 */

#pragma once

#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"

#include <quantized_looper/hardware/led.hpp>
#include <reusable_synth/hardware/led.hpp>

class ToggleLed
{
public:
    ToggleLed(LedBase* led);
    void operator()();

private:
    LedBase* led;
    bool state;
};

class FadeLed
{
public:
    FadeLed(Led<TIM_HandleTypeDef>* led,
            std::chrono::duration<uint32_t, std::milli> period);
    inline void setPeriod(std::chrono::duration<uint32_t, std::milli> newPeriod)
    {
        period = newPeriod;
    };
    void operator()();

private:
    Led<TIM_HandleTypeDef>* led;
    std::chrono::duration<uint32_t, std::milli> period;
    float led_pct;
    int direction;
    std::chrono::duration<uint32_t, std::milli> last_update_time;
};
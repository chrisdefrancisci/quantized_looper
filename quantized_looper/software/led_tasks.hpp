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

class LedToggleAnimation
{
public:
    /**
     * @brief Construct a new Led Toggle Animation object
     *
     * @param led
     */
    LedToggleAnimation(LedBase* led);

    /**
     * @brief Executes the animation.
     *
     */
    void operator()();

private:
    LedBase* led;
    bool state;
};

class LedBreatheAnimation
{
public:
    /**
     * @brief Construct a new Fade Led object
     *
     * @todo Define "analog LED interface" which has the set intensity function
     *
     * @param led Handle for an LED object
     * @param period The period of the LED breathing animation
     */
    LedBreatheAnimation(Led<TIM_HandleTypeDef>* led,
                        std::chrono::duration<uint32_t, std::milli> period);

    /**
     * @brief Set the next period - this will not be updated immediately.
     * 
     * @param newPeriod 
     */
    inline void setPeriod(std::chrono::duration<uint32_t, std::milli> newPeriod)
    {
        nextPeriod = newPeriod;
    };

    /**
     * @brief Executes the animation.
     *
     */
    void operator()();

private:
    Led<TIM_HandleTypeDef>* led;
    std::chrono::duration<uint32_t, std::milli> period;
    std::chrono::duration<uint32_t, std::milli> nextPeriod;
    float led_pct;
    int direction;
    std::chrono::duration<uint32_t, std::milli> last_update_time;
};
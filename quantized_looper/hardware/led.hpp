/**
 * @file led.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief
 * @date 2025-12-27
 */

#pragma once

// Library includes
#include <functional>
#include <utility>

// Reusable synth includes
#include <reusable_synth/hardware/led.hpp>
// #include "reusable_synth/Hardware/led.hpp"

// Hardware includes
#include <stm32f7xx_hal_gpio.h>
#include <stm32f7xx_hal_tim.h>

/**
 * @brief Template specialization for LED controlled by GPIO.
 *
 * @tparam GPIO_TypeDef
 */
template<>
class Led<GPIO_TypeDef> : public LedBase
{
public:
    /**
     * @brief Construct a new led object
     *
     * @param handle GPIO handle
     * @param pin Pin number
     */
    Led(GPIO_TypeDef* handle, unsigned int pin)
      : handle(handle)
      , pin(pin) {};

    void on() override { HAL_GPIO_WritePin(handle, pin, GPIO_PIN_SET); }

    void off() override { HAL_GPIO_WritePin(handle, pin, GPIO_PIN_RESET); }

    std::pair<int, int> getRange() const override
    {
        return std::pair<int, int>(0, 1);
    }

private:
    GPIO_TypeDef* handle;
    unsigned int pin;
    std::pair<int, int> range;
};

/**
 * @brief Template specialization for LED controlled by PWM.
 *
 * @tparam TIM_HandleTypeDef
 */
template<>
class Led<TIM_HandleTypeDef> : public LedBase
{
public:
    /**
     * @brief Construct a new Led object
     *
     * @param handle PWM handle
     * @param channel PWM channel
     * @param constructor PWM init function
     * @param destructor PWM deinit function
     */
    Led(
      TIM_HandleTypeDef* handle,
      unsigned int channel,
      std::function<void()> constructor = []() {},
      std::function<void()> destructor = []() {})
      : handle(handle)
      , channel(channel)
      , constructor(constructor)
      , destructor(destructor)
    {
        constructor();
        range.first = 0;
        range.second = handle->Init.Period;
    };

    virtual ~Led() { destructor(); };

    void on() override { HAL_TIM_PWM_Start(handle, channel); }

    void off() override { HAL_TIM_PWM_Stop(handle, channel); }

    void setIntensity(int value)
    {
        // Ensure value is within range
        value = value > range.second ? range.second : value;
        value = value < range.first ? range.first : value;

        handle->Instance->CCR3 = value;
    }

    void setIntensity(float value)
    {
        int valueInt = (range.second - range.first) * value - range.first;
        setIntensity(valueInt);
    }

    std::pair<int, int> getRange() const override { return range; }

private:
    TIM_HandleTypeDef* handle;
    unsigned int channel;
    std::function<void()> constructor;
    std::function<void()> destructor;
    std::pair<int, int> range;
};

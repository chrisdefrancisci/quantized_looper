/**
 * @file led.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief
 * @date 2025-12-27
 */

// Library includes
#include <functional>
#include <utility>

// Reusable synth includes
#include <reusable_synth/Hardware/led.hpp>
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
class led<GPIO_TypeDef> : public ledBase
{
public:
    /**
     * @brief Construct a new led object
     *
     * @param handle GPIO handle
     * @param pin Pin number
     */
    led(GPIO_TypeDef* handle, unsigned int pin)
      : handle(handle)
      , pin(pin) {};

    void on() override { HAL_GPIO_WritePin(handle, pin, GPIO_PIN_SET); }

    void off() override { HAL_GPIO_WritePin(handle, pin, GPIO_PIN_RESET); }

    void setIntensity(int value) override
    {
        if (value != 0) {
            on();
        } else {
            off();
        }
    }

    void setIntensity(float value) override { setIntensity(value > 0); }

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
class led<TIM_HandleTypeDef> : public ledBase
{
public:
    /**
     * @brief Construct a new led object
     *
     * @param handle PWM handle
     * @param channel PWM channel
     * @param constructor PWM init function
     * @param destructor PWM deinit function
     */
    led(
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

    virtual ~led() { destructor(); };

    void on() override { HAL_TIM_PWM_Start(handle, channel); }

    void off() override { HAL_TIM_PWM_Stop(handle, channel); }

    void setIntensity(int value) override
    {
        off();
        // Ensure value is within range
        value = value > range.second ? range.second : value;
        value = value < range.first ? range.first : value;

        handle->Instance->CCR3 = value;
        on();
    }

    void setIntensity(float value) override
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

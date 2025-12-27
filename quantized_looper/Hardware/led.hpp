/**
 * @file led.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief 
 * @date 2025-12-27
 */

// Standard library includes
#include <functional>
#include <type_traits>
#include <utility>

// Hardware includes
#include <stm32f7xx_hal_gpio.h>
#include <stm32f7xx_hal_tim.h>

/**
 * @brief This is the abstract interface class for writing to an LED.
 *
 * @todo This should be moved to reusable_synth/Hardware
 */
class ledBase 
{
public:
    virtual ~ledBase() = default;
    /**
     * @brief Turns LED on.
     */
    virtual void on() = 0;

    /**
     * @brief Turns LED off.
     */
    virtual void off() = 0;

    /**
     * @brief Set the intensity of the LED.
     * 
     * @param value Intensity, from 0 to max range.
     */
    virtual void setIntensity(int value) = 0;

    /**
     * @brief Set the Intensity of the LED.
     * 
     * @param value Intensity, from 0.0 to 1.0
     */
    virtual void setIntensity(float value) = 0;

    /**
     * @brief Get the range of the LED intensity
     * 
     * @return std::pair<int, int> 
     */
    virtual std::pair<int, int> getRange() const = 0;

protected:
    ledBase() = default;
};

/**
 * @brief A template derived class of ledBase that must be specialized for the hardware.
 * 
 * @todo This should be moved to reusable_synth/Hardware
 * @tparam T The type of the hardware handle.
 */
template <typename T>
class led : public ledBase
{
private:
    led() = default;
};

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
    led(GPIO_TypeDef* handle, unsigned int pin) :
        handle(handle),
        pin(pin)
    {
    };

    void on() override{
        HAL_GPIO_WritePin(handle, pin, GPIO_PIN_SET);
    }

    void off() override{
        HAL_GPIO_WritePin(handle, pin, GPIO_PIN_RESET);
    }

    void setIntensity(int value) override
    { 
        if (value != 0)
        {
            on();
        }
        else 
        {
            off();
        }
    }
    
    void setIntensity(float value) override { setIntensity(value > 0); }
    
    std::pair<int, int> getRange() const override { return std::pair<int, int>(0, 1); }


private:
    GPIO_TypeDef* handle;
    unsigned int pin;
    std::function<void()> constructor;
    std::function<void()> destructor;
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
    led(TIM_HandleTypeDef* handle, unsigned int channel, 
        std::function<void()> constructor = [](){}, std::function<void()> destructor = [](){}) :
        handle(handle),
        channel(channel),
        constructor(constructor),
        destructor(destructor)
    {
        constructor();
        range.first = 0;
        range.second = handle->Init.Period;
    };

    virtual ~led() 
    {
        destructor();
    };

    void on() override
    {
        HAL_TIM_PWM_Start(handle, channel);
    }

    void off() override
    {
        HAL_TIM_PWM_Stop(handle, channel);
    }

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


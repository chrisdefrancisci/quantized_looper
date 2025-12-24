/*
 * main.cpp
 *
 *  Created on: Dec 21, 2025
 *      Author: chris
 */

// #include <quantized_looper/Core/Inc/main.h>
// #include <quantized_looper/Core/Inc/gpio.h>
#include <main.h>
#include <gpio.h>

#include <quantized_looper/Hardware/led.hpp>

extern "C"
{
	extern void SystemClock_Config();
}

template <uint32_t port, uint16_t pin>
class port_pin
{
public:
	static void set_pin_high() { HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef*>(port), pin, GPIO_PIN_SET); }
	static void set_pin_low() { HAL_GPIO_WritePin(reinterpret_cast<GPIO_TypeDef*>(port), pin, GPIO_PIN_RESET); }
};

int main()
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();

	while(true)
	{
		port_pin<GPIOB_BASE, LD2_Pin>::set_pin_high();
		HAL_Delay(2000);
		port_pin<GPIOB_BASE, LD2_Pin>::set_pin_low();
		HAL_Delay(1000);
	}
}




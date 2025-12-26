#include <main.h>
#include <gpio.h>

#include <tim.h>

extern "C"
{
   extern void SystemClock_Config();
}

int main()
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM3_Init();


    int analog_value = 0;

    while(true)
    {
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        htim3.Instance->CCR3 = analog_value;
        analog_value += 2000;
        HAL_Delay(1000);

        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
        HAL_Delay(500);
    }
}

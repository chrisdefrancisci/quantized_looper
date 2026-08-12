/**
 * @file adcDma.cpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief
 * @date 2026-05-04
 */

#include <quantized_looper/hardware/adcDma.hpp>

#include <array>

#include "adc.h"
#include "dma.h"
#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_tim.h"
#include "tim.h"

#include <quantized_looper/utils/logger_singleton.hpp>
#include <reusable_synth/hardware/interrupt_handler.hpp>

static InterruptHandler halfCompleteCallback{};
static InterruptHandler completeCallback{};

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    halfCompleteCallback();
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    completeCallback();
}

Adc::Adc()
  : periphData(inputBuffer)
  , dmaManager(memoryData, inputBuffer)
{
    // Connect callbacks
    halfCompleteCallback.connect<&decltype(dmaManager)::setHalfCompleteFlag>(
      &dmaManager);
    completeCallback.connect<&decltype(dmaManager)::setCompleteFlag>(
      &dmaManager);
}

void Adc::start()
{
    LoggerSingleton::get()->info("Initializing ADC...");
    // TODO: someway to turn on this timer regardless of DMA/ADC coming first
    //      Maybe just need to accept that status == HAL_BUSY is also fine
    //      see
    //      https://community.st.com/stm32-mcus-products-25/how-get-timer-state-7197
    // if (HAL_TIM_Base_GetState(&htim2) == HAL_TIM_STATE_READY) {
    //     auto status = HAL_TIM_Base_Start(&htim2);
    //     assert_param(status == HAL_OK); // NOLINT(*cstyle-cast)
    // }
    auto status =
      HAL_ADC_Start_DMA(&hadc1, inputBuffer.data(), inputBuffer.size());
    assert_param(status == HAL_OK); // NOLINT(*cstyle-cast)
    LoggerSingleton::get()->info("...Complete");
}
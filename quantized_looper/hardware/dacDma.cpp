/**
 * @file dac.cpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief
 * @date 2026-03-24
 */

#include <quantized_looper/hardware/dacDma.hpp>

#include <array>

#include "dac.h"
#include "dma.h"
#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_dac.h"
#include "stm32f7xx_hal_tim.h"
#include <tim.h>

#include <quantized_looper/utils/logger_singleton.hpp>
#include <reusable_synth/hardware/interrupt_handler.hpp>

static std::array<Dac::DacType, Dac::outputSize> outputBuffer = { 0 };
static InterruptHandler halfCompleteCallback{};
static InterruptHandler completeCallback{};

extern "C" void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
    halfCompleteCallback();
}
extern "C" void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
    completeCallback();
}

Dac::Dac(std::span<ComputationType, inputSize> inputData)
  : memoryData(inputData)
  , periphData(outputBuffer)
  , dmaManager(convertedMemoryData, outputBuffer)
{

    // Connect callbacks
    halfCompleteCallback.connect<&decltype(dmaManager)::setHalfCompleteFlag>(
      &dmaManager);
    completeCallback.connect<&decltype(dmaManager)::setCompleteFlag>(
      &dmaManager);
}

void Dac::init()
{
    LoggerSingleton::get()->info("Initializing DAC");
    auto status = HAL_TIM_Base_Start(&htim2);
    assert_param(status == HAL_OK);
    status = HAL_DAC_Start_DMA(&hdac,
                               DAC_CHANNEL_1,
                               outputBuffer.data(),
                               outputBuffer.size(),
                               DAC_ALIGN_12B_R);

    assert_param(status == HAL_OK);
}
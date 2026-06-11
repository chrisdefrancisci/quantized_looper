/**
 * @file adcDma.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief Implements an ADC wrapper for ADC1, IN0.
 *
 * This corresponds to pin PA0 on the STM32F767ZI, CN10 pin 29 on the nucleo
 * board.
 * @date 2026-05-04
 */
#pragma once

#include <algorithm>
#include <ranges>
#include <span>
#include <type_traits>

#include <quantized_looper/utils/definitions.hpp>
#include <reusable_synth/hardware/circular_dma.hpp>
#include <reusable_synth/utils/noncopyable.hpp>

class Adc : Noncopyable
{
public:
    using ComputationType = QuantizedLooper::ComputationType;
    using AdcType = QuantizedLooper::AnalogInterfaceType;
    constexpr static int outputSize = QuantizedLooper::computationBufferSize;
    constexpr static int inputSize = QuantizedLooper::analogInterfaceBufferSize;

    Adc(std::span<ComputationType, outputSize> outputData);

    static void start();

    void execute()
    {
        dmaManager.execute();
        std::ranges::transform(
          memoryData,
          convertedMemoryData.begin(),
          //    QuantizedLooper::scale_input);
          [](AdcType x) -> ComputationType { return ComputationType(x); });
    }

private:
    std::array<AdcType, outputSize> memoryData{};
    std::span<ComputationType, outputSize> convertedMemoryData;
    std::span<AdcType, inputSize> periphData;
    CircularDma<DmaDirection::MemoryToPeripheral, 1, AdcType, outputSize>
      dmaManager;
};
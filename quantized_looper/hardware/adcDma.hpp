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
    constexpr static int nChannels = 2;
    constexpr static int outputSize =
      nChannels * QuantizedLooper::computationBufferSize;
    constexpr static int inputSize =
      nChannels * QuantizedLooper::analogInterfaceBufferSize;

    Adc();

    void start();

    void execute()
    {
        dmaManager.execute();
        std::ranges::transform(memoryData,
                               convertedMemoryData.begin(),
                               [](AdcType x) -> ComputationType {
                                   return QuantizedLooper::scale_input(x);
                               });
    }

    template<size_t Index>
    constexpr auto getRawInput()
      -> std::span<const AdcType, QuantizedLooper::computationBufferSize>
    {
        static_assert(Index < nChannels,
                      "ADC Index must be between 0 and nChannels - 1");
        return std::span<const AdcType, QuantizedLooper::computationBufferSize>(
          memoryData |
          std::views::drop(QuantizedLooper::computationBufferSize * Index) |
          std::views::take(QuantizedLooper::computationBufferSize));
    }

    template<size_t Index>
    constexpr auto getInput()
      -> std::span<const ComputationType,
                   QuantizedLooper::computationBufferSize>
    {
        static_assert(Index < nChannels,
                      "ADC Index must be between 0 and nChannels - 1");
        return std::span<const ComputationType,
                         QuantizedLooper::computationBufferSize>(
          convertedMemoryData |
          std::views::drop(QuantizedLooper::computationBufferSize * Index) |
          std::views::take(QuantizedLooper::computationBufferSize));
    }

private:
    std::array<AdcType, outputSize> memoryData{};
    std::array<ComputationType, outputSize> convertedMemoryData{};
    std::array<AdcType, inputSize> inputBuffer{};
    std::span<AdcType, inputSize> periphData;
    CircularDma<DmaDirection::PeripheralToMemory,
                nChannels,
                AdcType,
                outputSize>
      dmaManager;
};

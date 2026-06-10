/**
 * @file dac.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief Implements a DAC wrapper for channel 1 of the onboard DAC.
 * @date 2026-03-24
 */
#pragma once

#include <algorithm>
#include <ranges>
#include <span>
#include <type_traits>

#include <quantized_looper/utils/definitions.hpp>
#include <reusable_synth/hardware/circular_dma.hpp>

class Dac
{
public:
    using ComputationType = QuantizedLooper::ComputationType;
    using DacType = QuantizedLooper::AnalogInterfaceType;
    constexpr static int inputSize = QuantizedLooper::computationBufferSize;
    constexpr static int outputSize =
      QuantizedLooper::analogInterfaceBufferSize;

    Dac(std::span<ComputationType, inputSize> inputData);

    static void init();

    void execute()
    {
        dmaManager.execute();
        std::ranges::transform(memoryData,
                               convertedMemoryData.begin(),
                               QuantizedLooper::scale_output);
    }
    auto isReady() -> bool { return dmaManager.isReady(); }

    Dac(const Dac&) = delete;
    auto operator=(const Dac&) -> Dac& = delete;
    Dac(Dac&&) = delete;
    auto operator=(Dac&&) = delete;
    ~Dac() = default;

private:
    std::span<ComputationType, inputSize> memoryData;
    std::array<DacType, inputSize> convertedMemoryData{};
    std::span<DacType, outputSize> periphData; // TODO: I think this can be made
                                               // to be the array that DMA uses
    CircularDma<DmaDirection::PeripheralToMemory, 1, DacType, inputSize>
      dmaManager;
};
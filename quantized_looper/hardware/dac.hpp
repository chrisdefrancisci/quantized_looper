/**
 * @file dac.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief Implements a DAC singleton to hide all the ugliness
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

    void execute()
    {
        dmaManager.execute<DmaDirection::MemoryToPeripheral>();
        std::ranges::transform(
          memoryData,
          convertedMemoryData.begin(),
          +[](const ComputationType& in) -> DacType {
              using T = std::common_type_t<DacType, ComputationType>;
              T = 
              return DacType{};
          });
    }
    void isReady() { dmaManager.isReady(); }

    Dac(const Dac&) = delete;
    auto operator=(const Dac&) -> Dac& = delete;
    Dac(Dac&&) = delete;
    auto operator=(Dac&&) = delete;
    ~Dac() = default;

private:
    std::span<ComputationType, inputSize> memoryData;
    std::array<DacType, inputSize> convertedMemoryData{};
    std::span<DacType, outputSize> periphData;
    CircularDma<DacType, outputSize> dmaManager;
};
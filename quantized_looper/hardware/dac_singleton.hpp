/**
 * @file dac.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief Implements a DAC singleton to hide all the ugliness
 * @date 2026-03-24
 */
#pragma once

#include <span>

#include <quantized_looper/utils/definitions.hpp>
#include <reusable_synth/hardware/dac.hpp>

class DacSingleton
{
public:
    using ComputationType = QuantizedLooper::ComputationType;
    using DacType = QuantizedLooper::AnalogInterfaceType;
    constexpr static int inputSize = QuantizedLooper::computationBufferSize;
    constexpr static int outputSize =
      QuantizedLooper::analogInterfaceBufferSize;

    static Dac<ComputationType, DacType, inputSize>* get(
      std::span<ComputationType, inputSize> inputData,
      void (*convert)(DacType& out, const ComputationType& in));

    DacSingleton(const DacSingleton&) = delete;
    auto operator=(const DacSingleton&) -> DacSingleton& = delete;
    DacSingleton(DacSingleton&&) = delete;
    auto operator=(DacSingleton&&) = delete;

private:
    DacSingleton() = default;
};
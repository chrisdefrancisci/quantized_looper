/**
 * @file definitions.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief Definitions of types and constants used for this project.
 * @date 2026-03-28
 */

#pragma once

#include <chrono>
#include <cstdint>

namespace QuantizedLooper {

using ComputationType = float;
using AnalogInterfaceType = uint32_t;
using TickType = std::chrono::duration<uint32_t, std::milli>;

/** Sample rate of hardware and processing in Hz */
constexpr int sampleRate = 96000;

constexpr size_t computationBufferSize = 256;
constexpr size_t analogInterfaceBufferSize = computationBufferSize * 2;

constexpr ComputationType computationMin = -1.0;
constexpr ComputationType computationMax = 1.0;
constexpr AnalogInterfaceType analogMin = 0;
constexpr AnalogInterfaceType analogMax = 4096;

/**
 * @brief Scales a value from a peripheral that reads analog values to the
 * computation type.
 *
 * @param in The value from the analog peripheral.
 * @return ComputationType The value to be used in computations.
 */
constexpr auto scale_input(AnalogInterfaceType in) -> ComputationType
{
    using T = std::common_type_t<AnalogInterfaceType, ComputationType>;
    T denominator = analogMax - analogMin;
    T numerator = computationMax - computationMin;
    return ComputationType((numerator * T(in - analogMin) / denominator) +
                           computationMin);
};

/**
 * @brief Scales a value from computation to the analog peripheral type.
 *
 * @param in The computed value.
 * @return AnalogInterfaceType The value to be used by the peripheral.
 */
constexpr auto scale_output(ComputationType in) -> AnalogInterfaceType
{
    using T = std::common_type_t<AnalogInterfaceType, ComputationType>;
    T denominator = computationMax - computationMin;
    T numerator = analogMax - analogMin;
    return AnalogInterfaceType(
      (numerator * T(in - computationMin) / denominator) + analogMin);
};

}
/**
 * @file definitions.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief Definitions of types and constants used for this project.
 * @date 2026-03-28
 */

#pragma once

namespace QuantizedLooper {

using ComputationType = float;
using AnalogInterfaceType = uint32_t;

constexpr int computationBufferSize = 256;
constexpr int analogInterfaceBufferSize = computationBufferSize * 2;
}
/**
 * @file interval_tracker.hpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief
 * @date 2026-02-01
 */

#pragma once

#include <chrono>

template<typename TickType>
class IntervalTracker
{
public:
    IntervalTracker(TickType minInterval, TickType maxInterval) {}
    void addEvent(TickType time) {}
    auto getInterval() -> TickType {}

private:
    TickType maxInterval;
    TickType minInterval;
};

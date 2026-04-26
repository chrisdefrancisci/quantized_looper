/**
 * @file led_tasks.cpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief This function defines how LEDs are updated over time.
 * @date 2026-01-16
 */

#include "led_tasks.hpp"

#include <quantized_looper/utils/logger_singleton.hpp>
#include <sstream>

LedToggleAnimation::LedToggleAnimation(LedInterface* led)
  : led(led)
  , state(false) {};

void LedToggleAnimation::operator()()
{
    if (state) {
        led->off();
    } else {
        led->on();
    }
    state = !state;
}

LedBreatheAnimation::LedBreatheAnimation(
  LedInterface* led,
  std::chrono::duration<uint32_t, std::milli> (*getTick)(),
  std::chrono::duration<uint32_t, std::milli> period)
  : led(led)
  , getTick(getTick)
  , period(period)
  , nextPeriod(period)
  , ledPct(0)
  , direction(1)
  , lastUpdateTime(0) {};

void LedBreatheAnimation::operator()()
{
    auto currentTime = getTick();

    if (lastUpdateTime == std::chrono::duration<uint32_t, std::milli>(0)) {
        lastUpdateTime = currentTime;
    }

    auto elapsed = currentTime - lastUpdateTime;
    float change =
      direction * 2.0F * (float)elapsed.count() / (float)period.count();

    ledPct += change;

    if (ledPct >= 1.0F) {
        ledPct = 2.0F - ledPct; // Reflect over 1.0
        direction = -1.0F;
        period = nextPeriod;
        std::stringstream stream;
        stream << "LED fade direction -1 with period " << period.count()
               << " ms";
        LoggerSingleton::get()->info(stream.str());
    } else if (ledPct < 0.0F) {
        ledPct = -ledPct; // Reflect over 0
        direction = 1.0F;
        period = nextPeriod;
        std::stringstream stream;
        stream << "LED fade direction +1 with period " << period.count()
               << " ms";
        LoggerSingleton::get()->info(stream.str());
    }

    auto range = led->getRange();
    // TODO: replaces this with the led->setIntensity(float)
    //  And then remove the setIntensity(int) from the interface
    int finalAmt = range.first + (int)((range.second - range.first) * ledPct);

    led->setIntensity(finalAmt);
    led->on();
    lastUpdateTime = currentTime;
}
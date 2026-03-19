/**
 * @file led_tasks.cpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief This function defines how LEDs are updated over time.
 * @date 2026-01-16
 */

#include "led_tasks.hpp"

#include <quantized_looper/utils/logger_singleton.hpp>
#include <sstream>

LedToggleAnimation::LedToggleAnimation(LedBase* led)
  : led(led)
  , state(false) {};

void LedToggleAnimation::operator()()
{
    if (state) {
        LoggerSingleton::get()->info("Turning off LED");
        led->off();
    } else {
        LoggerSingleton::get()->info("Turning on LED");
        led->on();
    }
    state = !state;
}

LedBreatheAnimation::LedBreatheAnimation(
  LedBase* led,
  std::chrono::duration<uint32_t, std::milli> (*getTick)(),
  std::chrono::duration<uint32_t, std::milli> period)
  : led(led)
  , getTick(getTick)
  , period(period)
  , nextPeriod(period)
  , led_pct(0)
  , direction(1)
  , last_update_time(0) {};

void LedBreatheAnimation::operator()()
{
    auto current_time = getTick();

    if (last_update_time == std::chrono::duration<uint32_t, std::milli>(0)) {
        last_update_time = current_time;
    }

    auto elapsed = current_time - last_update_time;
    float change =
      direction * 2.0f * (float)elapsed.count() / (float)period.count();

    led_pct += change;

    if (led_pct >= 1.0f) {
        led_pct = 2.0f - led_pct; // Reflect over 1.0
        direction = -1;
        period = nextPeriod;
        std::stringstream ss;
        ss << "LED fade direction -1 with period " << period.count() << " ms";
        LoggerSingleton::get()->info(ss.str().c_str());
    } else if (led_pct < 0.0f) {
        led_pct = -led_pct; // Reflect over 0
        direction = 1;
        period = nextPeriod;
        std::stringstream ss;
        ss << "LED fade direction +1 with period " << period.count() << " ms";
        LoggerSingleton::get()->info(ss.str().c_str());
    }

    auto range = led->getRange();
    // TODO: replaces this with the led->setIntensity(float)
    //  And then remove the setIntensity(int) from the interface
    int final_amt = range.first + (int)((range.second - range.first) * led_pct);

    led->setIntensity(final_amt);
    led->on();
    last_update_time = current_time;
}
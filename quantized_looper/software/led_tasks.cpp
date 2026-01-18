/**
 * @file led_tasks.cpp
 * @author Chris DeFrancisci (chrisdefrancisci@gmail.com)
 * @brief This function defines how LEDs are updated over time.
 * @date 2026-01-16
 */

#include "led_tasks.hpp"

ToggleLed::ToggleLed(LedBase* led)
  : led(led)
  , state(false) {};

void ToggleLed::operator()()
{
    if (state) {
        led->off();
    } else {
        led->on();
    }
    state = !state;
}

FadeLed::FadeLed(Led<TIM_HandleTypeDef>* led,
                 std::chrono::duration<uint32_t, std::milli> period)
  : led(led)
  , period(period)
  , led_pct(0)
  , direction(1)
  , last_update_time(0) {};

void FadeLed::operator()()
{
    auto current_time =
      std::chrono::duration<uint32_t, std::milli>(HAL_GetTick());

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
    } else if (led_pct < 0.0f) {
        led_pct = -led_pct; // Reflect over 0
        direction = 1;
    }

    auto range = led->getRange();
    int final_amt = range.first + (int)((range.second - range.first) * led_pct);

    led->setIntensity(final_amt);
    led->on();
    last_update_time = current_time;
}
#include <Arduino.h>
#include "esp_timer.h"

#define RELAY_PIN 18

#define RELAY_ON  LOW
#define RELAY_OFF HIGH

// Налаштування часу
uint32_t period_ms  = 10000; // повний цикл 10 с
uint32_t on_time_ms = 2000;  // увімкнено 2 с

esp_timer_handle_t relay_timer = NULL;

void relay_timer_callback(void *arg)
{
    static bool relayOn = true;

    if (relayOn)
    {
        // Вимкнути реле
        digitalWrite(RELAY_PIN, RELAY_OFF);

        uint32_t off_time_ms = period_ms - on_time_ms;

        esp_timer_start_once(
            relay_timer,
            (uint64_t)off_time_ms * 1000ULL
        );

        relayOn = false;
    }
    else
    {
        // Увімкнути реле
        digitalWrite(RELAY_PIN, RELAY_ON);

        esp_timer_start_once(
            relay_timer,
            (uint64_t)on_time_ms * 1000ULL
        );

        relayOn = true;
    }
}

void setup()
{
    pinMode(RELAY_PIN, OUTPUT);

    // Початково реле вимкнене
    digitalWrite(RELAY_PIN, RELAY_OFF);

    const esp_timer_create_args_t timer_args =
    {
        .callback = &relay_timer_callback,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "relay_timer"
    };

    esp_timer_create(&timer_args, &relay_timer);

    // Перший цикл: увімкнути реле
    digitalWrite(RELAY_PIN, RELAY_ON);

    esp_timer_start_once(
        relay_timer,
        (uint64_t)on_time_ms * 1000ULL
    );
}

void loop()
{
    
}
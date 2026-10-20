#include <Arduino.h>

#define RELAY_PIN      18

#define PERIOD_MS      10000ULL   
#define ON_TIME_MS      2000ULL   

esp_timer_handle_t relay_timer = NULL;

void relay_timer_callback(void* arg) 
{
    static bool state = false;
    
    if (state) {
        digitalWrite(RELAY_PIN, HIGH);
        esp_timer_start_once(relay_timer, (PERIOD_MS - ON_TIME_MS) * 1000ULL);
        state = false;
    } else {
        digitalWrite(RELAY_PIN, LOW);
        esp_timer_start_once(relay_timer, ON_TIME_MS * 1000ULL);
        state = true;
    }
}

void setup() 
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); 

    const esp_timer_create_args_t timer_args = {
        .callback = &relay_timer_callback,
        .name = "relay_timer"
    };

    esp_timer_create(&timer_args, &relay_timer);
    
    digitalWrite(RELAY_PIN, LOW);
    esp_timer_start_once(relay_timer, ON_TIME_MS * 1000ULL);
}

void loop() 
{
    
}
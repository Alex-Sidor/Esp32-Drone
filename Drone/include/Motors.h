#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "Vector.h"

#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_TIMER             LEDC_TIMER_0
#define LEDC_MODE              LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL           LEDC_CHANNEL_0
#define LEDC_DUTY_RES          LEDC_TIMER_10_BIT
#define LEDC_MAX_NUMBER        1023
#define LEDC_FREQUENCY         (20000)

class DroneMotors{
public:

    const ledc_channel_t channels[4] = {
        LEDC_CHANNEL_0,
        LEDC_CHANNEL_1,
        LEDC_CHANNEL_2,
        LEDC_CHANNEL_3
    };

/*

o 0      o 1

    /\
    ||

o 2     o 3


*/

    DroneMotors(const gpio_num_t motorPins[4]);

    void testMotors();

    void runMotors(Vec2 direciton, float thrust);

    float min(float a, float b);
    float max(float a, float b);
    float minmax(float input, float bottom, float top);

private:

    gpio_num_t motors[4];
    
};
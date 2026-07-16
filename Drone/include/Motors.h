#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "Vector.h"

#define MOTOR_GPIO             (17) 
#define LEDC_TIMER             LEDC_TIMER_0
#define LEDC_MODE              LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL           LEDC_CHANNEL_0
#define LEDC_DUTY_RES          LEDC_TIMER_10_BIT
#define LEDC_FREQUENCY         (5000)

class DroneMotors{
public:

/*

o 0      o 1

    /\
    ||

o 2     o 3


*/

    DroneMotors(gpio_num_t droneMotors[4]);

    void testMotor(gpio_num_t pin);

    void runMotors(Vec2 direciton, float thrust);

    float DroneMotors::min(float a, float b);
    float DroneMotors::max(float a, float b);
    float DroneMotors::minmax(float input, float bottom, float top);



private:

    gpio_num_t motors[4]
    
}
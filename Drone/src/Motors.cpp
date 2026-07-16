#include "Motors.h"

DroneMotors::DroneMotors(gpio_num_t droneMotors[4]){

}

void DroneMotors::testMotor(gpio_num_t pin){
    gpio_set_level(pin, 1);
    vTaskDelay(pdMS_TO_TICKS(250));
    gpio_set_level(pin, 0);
    vTaskDelay(pdMS_TO_TICKS(250));
}

void DroneMotors::runMotors(Vec2 direciton, float thrust){
    
}

float DroneMotors::min(float a, float b){
    if(a<b)
        return a;
    return b;
}

float DroneMotors::max(float a, float b){
    if(a>b)
        return a;
    return b;
}

float DroneMotors::minmax(float input, float bottom, float top){
    return(max(min(input,top),bottom));
}


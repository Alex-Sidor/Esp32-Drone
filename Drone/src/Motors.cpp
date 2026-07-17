#include "Motors.h"

DroneMotors::DroneMotors(const gpio_num_t motorPins[4]){
    for(size_t i = 0; i < 4; i++){
        motors[i] = motorPins[i];
    }

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    for (int i = 0; i < 4; i++) {
        ledc_channel_config_t ledc_channel = {
            .gpio_num       = motors[i],
            .speed_mode     = LEDC_MODE,
            .channel        = channels[i],
            .timer_sel      = LEDC_TIMER,
            .duty           = 0,
            .hpoint         = 0
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    }
}

void DroneMotors::testMotors(){
    for(size_t i = 0; i < 4; i++){
        runMotor(i, 0.1);
        vTaskDelay(pdMS_TO_TICKS(100));
        runMotor(i, 0);
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}

void DroneMotors::runMotor(int i, float thrust){

    thrust = minmax(thrust, 0, 1);
    thrust *= LEDC_MAX_NUMBER;

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, channels[i], static_cast<uint32_t>(thrust)));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, channels[i]));
}

void DroneMotors::runMotors(Vec2 direciton, float thrust){
    
    thrust = minmax(thrust, 0, 1);

    thrust *= LEDC_MAX_NUMBER;
    
    for(size_t i = 0; i < 4; i++){
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, channels[i], static_cast<uint32_t>(thrust)));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, channels[i]));
    }
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


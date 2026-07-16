#include "Motors.h"

DroneMotors::DroneMotors(const gpio_num_t motorPins[4]){
    for(size_t i = 0; i < 4; i++){
        motors[i] = motorPins[i];
    }
}

void DroneMotors::initMotors() {

    const ledc_channel_t channels[4] = {
        LEDC_CHANNEL_0,
        LEDC_CHANNEL_1,
        LEDC_CHANNEL_2,
        LEDC_CHANNEL_3
    };

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
        gpio_set_level(motors[i], 1);
        vTaskDelay(pdMS_TO_TICKS(250));
        gpio_set_level(motors[i], 0);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
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


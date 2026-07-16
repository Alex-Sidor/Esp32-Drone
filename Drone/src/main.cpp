#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void testMotor(gpio_num_t pin){
    gpio_set_level(pin, 1);
    vTaskDelay(pdMS_TO_TICKS(250));
    gpio_set_level(pin, 0);
    vTaskDelay(pdMS_TO_TICKS(250));
}

extern "C" void app_main(void)
{
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);

    while (1)
    {
        testMotor(GPIO_NUM_4);
        testMotor(GPIO_NUM_5);
        testMotor(GPIO_NUM_18);
        testMotor(GPIO_NUM_19);
    }
}
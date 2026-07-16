#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "Motors.h"

#include "Vector.h"

extern "C" void app_main(void)
{
    const gpio_num_t pins[4] = {
        GPIO_NUM_4,
        GPIO_NUM_5,
        GPIO_NUM_18,
        GPIO_NUM_19
    };

    DroneMotors m(pins);

    m.initMotors();

    m.testMotors();
}
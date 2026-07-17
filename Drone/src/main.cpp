#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "Motors.h"

#include "Vector.h"
#include "MPU6050.h"

extern "C" void app_main(void)
{
    const gpio_num_t pins[4] = {
        GPIO_NUM_4,
        GPIO_NUM_5,
        GPIO_NUM_18,
        GPIO_NUM_19
    };

    DroneMotors m(pins);

    m.testMotors();

    vTaskDelay(pdMS_TO_TICKS(2000));


    MPU6050 imu;

    while(1){
        imu.update();
        vTaskDelay(pdMS_TO_TICKS(100));
        /*for(size_t i = 0; i < 10; i++){
            m.runMotors(Vec2(),((float)i)/10.0f);
            vTaskDelay(pdMS_TO_TICKS(100));
        }*/
    }
}
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "Motors.h"

#include "PID.h"

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

    vTaskDelay(pdMS_TO_TICKS(2000));

    m.testMotors();

    vTaskDelay(pdMS_TO_TICKS(2000));


    MPU6050 imu;

    PID x(0.000,0,0.1);
    PID y(0.000,0,0.1);

    while(1){
        imu.update();

        Vec3 a = imu.getAngle();
        Vec3 r = imu.getRotVel();

        float dt = imu.getDT();

        Vec2 out;

        out.x = x.update(a.x,r.x,dt);
        out.y = y.update(a.y,r.y,dt);

        m.runMotors(out,0.0);

        ESP_LOGI("Main", "\nx=%.2f\ty=%.2f", out.x, out.y);

        vTaskDelay(pdMS_TO_TICKS(10));
        /*for(size_t i = 0; i < 10; i++){
            m.runMotors(Vec2(),((float)i)/10.0f);
            vTaskDelay(pdMS_TO_TICKS(100));
        }*/
    }
}
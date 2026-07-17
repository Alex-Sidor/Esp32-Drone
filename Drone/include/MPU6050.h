#pragma once

#include "Vector.h"

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/unistd.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"

#include <math.h>

#define I2C_NUM     I2C_NUM_0
#define I2C_SCL     GPIO_NUM_22
#define I2C_SDA     GPIO_NUM_21 

#define MPU_ADDR        0x68

class MPU6050{
public:

    MPU6050();

    void update();

    Vec3 getAngle();

    Vec3 getAcceleration();

private:
    float filterGyroBias = 0.99f;

    int64_t lastTime = 0;





    const char* TAG = "MPU6050_I2C";

    esp_err_t mpuReadfromReg (uint8_t Reg, uint8_t *ReadBuffer, size_t len);
    esp_err_t mpuWriteReg (uint8_t Reg, uint8_t data);
    static esp_err_t i2c_master_init(void);

    Vec3 getAngleFromAccel(Vec3 v);

    Vec3 currentAcceleration;
    Vec3 currentRotationalVelocity;

    Vec3 currentAngle;

    uint8_t data[10];

};
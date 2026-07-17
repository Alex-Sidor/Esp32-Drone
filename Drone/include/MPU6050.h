#pragma once

#include "Vector.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/unistd.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_NUM     I2C_NUM_0
#define I2C_SCL     GPIO_NUM_22
#define I2C_SDA     GPIO_NUM_21 

#define MPU_ADDR        0x68

esp_err_t mpuReadfromReg (uint8_t Reg, uint8_t *ReadBuffer, size_t len)
{
	return (i2c_master_write_read_device(I2C_NUM, MPU_ADDR, &Reg, 1, ReadBuffer, len, 2000));
}

esp_err_t mpuWriteReg (uint8_t Reg, uint8_t data)
{
	uint8_t writeBuf[2];  // writeBuf[len+1];
	writeBuf[0] = Reg;
	writeBuf[1] = data;
	return (i2c_master_write_to_device(I2C_NUM, MPU_ADDR, writeBuf, 2, 1000));
}

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

void app_main(void)
{
    uint8_t data[10];
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    
    mpuReadfromReg(0x75, data, 1);
    ESP_LOGI(TAG, "%X", data[0]);
    
    mpuWriteReg(0x6B, 0);
    mpuWriteReg(0x19, 7); // sample rate 1KHz
    mpuWriteReg(0x1C, 0);  // ACC FS Range ±2g
    
    while (1)
    {
		mpuReadfromReg(0x3B, data, 6);
		
		int16_t RAWX = (data[0]<<8)|data[1];
		int16_t RAWY = (data[2]<<8)|data[3];
		int16_t RAWZ = (data[4]<<8)|data[5];
		
		float xg = (float)RAWX/16384;
		float yg = (float)RAWY/16384;
		float zg = (float)RAWZ/16384;
		
		ESP_LOGI(TAG, "\nx=%.2f\ty=%.2f\tz=%.2f", xg, yg, zg);
		sleep(1);
		
	}
}

class MPU6050{
public:

    MPU6050

    void update();

    Vec3 getAngle();

    Vec3 getAcceleration();

private:

    Vec3 getAngleFromAccel(Vec3 accel);

    Vec3 currentAcceleration;
    Vec3 currentRotationalVelocity;

    Vec3 currentAngle;



}
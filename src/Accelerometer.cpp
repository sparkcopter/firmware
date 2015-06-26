#include <stdint.h>

#include "Accelerometer.h"
#include "HAL.h"

Accelerometer::Accelerometer() {
    #ifdef SENSOR_MPU6050_ADDRESS
    mpu6050.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    #endif
}

bool Accelerometer::isAvailable() {
    #ifdef SENSOR_MPU6050_ADDRESS
    return true;
    #else
    return false;
    #endif
}

Vector3 Accelerometer::getAcceleration() {
    Vector3 acceleration;

    #ifdef SENSOR_MPU6050_ADDRESS
    // Get configured accelerometer scale
    float accelScale = 2048.0 * pow(2, 3 - mpu6050.getFullScaleAccelRange());

    // Get the raw accelerometer readings
    int16_t rax, ray, raz;
    mpu6050.getAcceleration(&rax, &ray, &raz);

    // Convert raw accelerometer readings into Gs
    acceleration.x = (double)rax/accelScale;
    acceleration.y = (double)ray/accelScale;
    acceleration.z = (double)raz/accelScale;
    #endif

    return acceleration;
}

#include "defines.h"

#ifdef MPU6050_INSTALLED

#include "HAL_MPU6050.h"

void HAL_MPU6050::initialize() {
    device.initialize();

    accelScale = 2048.0 * pow(2, 3 - device.getFullScaleAccelRange());
    gyroScale = 16.4 * pow(2, 3 - device.getFullScaleGyroRange());
}

Vector3 HAL_MPU6050::getAcceleration() {
    Vector3 acceleration;

    // Get the raw accelerometer readings
    int16_t rax, ray, raz;
    device.getAcceleration(&rax, &ray, &raz);

    // Convert raw accelerometer readings into Gs
    acceleration.x = (double)rax/accelScale;
    acceleration.y = (double)ray/accelScale;
    acceleration.z = (double)raz/accelScale;

    return acceleration;
}

Vector3 HAL_MPU6050::getRotation() {
    Vector3 rotation;

    // Get the raw gyroscope readings
    int16_t rgx, rgy, rgz;
    device.getRotation(&rgx, &rgy, &rgz);

    // Convert raw gyroscope readings into degrees per second
    rotation.x = (double)rgx/gyroScale;
    rotation.y = (double)rgy/gyroScale;
    rotation.z = (double)rgz/gyroScale;

    return rotation;
}

#endif // MPU6050_INSTALLED

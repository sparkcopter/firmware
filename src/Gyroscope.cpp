#include <stdint.h>

#include "Gyroscope.h"
#include "HAL.h"

Gyroscope::Gyroscope() {
    #ifdef SENSOR_MPU6050_ADDRESS
    mpu6050.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    #endif
}

bool Gyroscope::isAvailable() {
    #ifdef SENSOR_MPU6050_ADDRESS
    return true;
    #else
    return false;
    #endif
}

Vector3 Gyroscope::getRotation() {
    Vector3 rotation;

    #ifdef SENSOR_MPU6050_ADDRESS
    // Get configured gyroscope scale
    float gyroScale = 16.4 * pow(2, 3 - mpu6050.getFullScaleGyroRange());

    // Get the raw gyroscope readings
    int16_t rgx, rgy, rgz;
    mpu6050.getRotation(&rgx, &rgy, &rgz);

    // Convert raw gyroscope readings into degrees per second
    rotation.x = (double)rgx/gyroScale;
    rotation.y = (double)rgy/gyroScale;
    rotation.z = (double)rgz/gyroScale;
    #endif

    return rotation;
}

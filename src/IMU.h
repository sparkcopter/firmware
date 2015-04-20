#pragma once

#include "math.h"

#include "Vector3.h"
#include "MPU6050.h"
#include "IMUFilter.h"

class IMU {
public:
    // Constructor
    IMU(uint8_t filter);

    // Initialize the sensors
    void initialize();

    // Set the horizontal refernce plane
    void calibrate();

    // Update the IMU with latest sensor data
    void update();

    // Get the current orientation in roll (x), pitch (y) and yaw (z)
    Vector3 getOrientation();

private:
    MPU6050 accelgyro;
    float gyroScale;
    float accelScale;
    IMUFilter *filter;

    uint32_t lastUpdate = 0;

    Vector3 accel;
    Vector3 gyro;
};

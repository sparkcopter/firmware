#pragma once

#include "MPU6050.h"

class IMU {
public:
    // Constructor
    IMU();

    // Set the horizontal refernce plane
    void calibrate();

    // Get the yaw, pitch and roll measurements
    void getYawPitchRoll(float *yaw, float *pitch, float *roll);

private:
    MPU6050 accelgyro;
};

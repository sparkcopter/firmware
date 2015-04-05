#pragma once

#include "math.h"

#include "MPU6050.h"

class IMU {
public:
    // Constructor
    IMU();

    // Initialize the sensors
    void initialize();

    // Set the horizontal refernce plane
    void calibrate();

    // Update the IMU with latest sensor data
    void update();

    // Get the quarternion measurements
    void getQuarternions(float *q1, float *q2, float *q3, float *q4);

    // Get the yaw, pitch and roll measurements
    void getYawPitchRoll(float *yaw, float *pitch, float *roll);

private:
    MPU6050 accelgyro;
    float gyroScale;
    float accelScale;

    float GyroMeasError = M_PI * (40.0f / 180.0f);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
    float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // compute beta
    float GyroMeasDrift = M_PI * (2.0f / 180.0f);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
    float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
    uint32_t lastUpdate = 0;

    float q1 = 1.0f, q2 = 0.0f, q3 = 0.0f, q4 = 0.0f;

    void madgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz);
};

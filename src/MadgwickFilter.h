#pragma once

#include "math.h"
#include "stdint.h"

#include "spark_wiring.h"

#include "IMUFilter.h"

class MadgwickFilter : public IMUFilter {
public:
    void update(Vector3 accel, Vector3 gyro);
    Vector3 getOrientation();

private:
    float GyroMeasError = M_PI * (40.0f / 180.0f);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
    float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // compute beta
    float GyroMeasDrift = M_PI * (2.0f / 180.0f);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
    float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value

    float q1 = 1.0f, q2 = 0.0f, q3 = 0.0f, q4 = 0.0f;

    uint32_t lastUpdate = 0;
};

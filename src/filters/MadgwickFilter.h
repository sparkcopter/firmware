#pragma once

#include "IMUFilter.h"
#include "Quarternion.h"
#include "Vector3.h"

class MadgwickFilter : public IMUFilter {
public:
    void update(Vector3 accel, Vector3 gyro, double dt);
    Vector3 getOrientation();

private:
    double beta = 0.2;
    Quarternion q;

    double q0 = 1.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;
};

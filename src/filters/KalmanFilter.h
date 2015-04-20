#pragma once

#include "IMUFilter.h"
#include "Vector3.h"

class KalmanFilter : public IMUFilter {
public:
    void update(Vector3 accel, Vector3 gyro);
    Vector3 getOrientation();
};

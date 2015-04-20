#pragma once

#include "IMUFilter.h"
#include "Vector3.h"

class ComplementaryFilter : public IMUFilter {
public:
    void update(Vector3 accel, Vector3 gyro);
    Vector3 getOrientation();

private:
    Vector3 orientation;
};

#pragma once

#define CF_GYRO_WEIGHT      0.90
#define CF_ACCEL_WEIGHT     (1 - CF_GYRO_WEIGHT)

#include "stdint.h"

#include "spark_wiring.h"

#include "IMUFilter.h"
#include "Vector3.h"

class ComplementaryFilter : public IMUFilter {
public:
    void update(Vector3 accel, Vector3 gyro, double dt);
    Vector3 getOrientation();

private:
    Vector3 orientation;
};

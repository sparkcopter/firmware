#pragma once

#include "Vector3.h"

#define IMU_FILTER_COMPLEMENTARY    0
#define IMU_FILTER_MADGWICK         1

class IMUFilter {
public:
   virtual void update(Vector3 accel, Vector3 gyro, double dt) = 0;
   virtual Vector3 getOrientation() = 0;
};

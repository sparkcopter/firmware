#pragma once

#include "Vector3.h"

#define IMU_FILTER_COMPLEMENTARY    0
#define IMU_FILTER_KALMAN           1
#define IMU_FILTER_MADGWICK         2

class IMUFilter {
public:
   virtual void update(Vector3 accel, Vector3 gyro) = 0;
   virtual Vector3 getOrientation() = 0;
};

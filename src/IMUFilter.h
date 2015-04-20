#pragma once

#include "Vector3.h"

class IMUFilter {
public:
   virtual void update(Vector3 accel, Vector3 gyro) = 0;
   virtual Vector3 getOrientation() = 0;
};

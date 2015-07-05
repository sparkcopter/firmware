#pragma once

#include "Accelerometer.h"
#include "Barometer.h"
#include "Compass.h"
#include "Gyroscope.h"
#include "Vector3.h"

class IMU {
public:
  IMU();

  // Initialize sensors
  void initialize();

  // Get the current orientation (roll, pitch and yaw)
  Vector3 getOrientation();

  // Get the current altitude
  double getAltitude();
};

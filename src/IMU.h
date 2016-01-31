#pragma once

#include "Accelerometer.h"
#include "Barometer.h"
#include "Gyroscope.h"
#include "Magnetometer.h"
#include "Vector3.h"

class IMU {
public:
    IMU();

    // Initialize sensors
    void initialize();

    // TODO: Calibrate sensors on ground level

    // Update orientation and altitude from sensor data
    void update();

    // Get the current orientation (roll, pitch and yaw)
    Vector3 getOrientation();

    // Get the current altitude
    double getAltitude();

private:
    Vector3 orientation;
    unsigned long lastUpdate;
};

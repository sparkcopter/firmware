#pragma once

#include "Sensors.h"
#include "Vector3.h"

class AHRS {
public:
    // Initialize sensors
    void initialize();

    // Update orientation and altitude from sensor data
    void update();

    // Get the current orientation (roll, pitch and yaw)
    Vector3 getOrientation();

private:
    Accelerometer *accelerometer;
    Gyroscope *gyroscope;
    Magnetometer *magnetometer;

    Vector3 orientation;
    unsigned long lastUpdate;
};

#include <application.h>

#include "AHRS.h"
#include "Telemetry.h"

#define CF_GYRO_WEIGHT      0.90
#define CF_ACCEL_WEIGHT     (1 - CF_GYRO_WEIGHT)

void AHRS::initialize() {
    Sensors::initialize();

    accelerometer = Sensors::getAccelerometer();
    gyroscope = Sensors::getGyroscope();
    magnetometer = Sensors::getMagnetometer();
}

void AHRS::update() {
    if(!accelerometer || !gyroscope) {
      Serial.println("Couldn't update AHRS, the required devies were not connected");
      return;
    }

    // Get the time since last update
    unsigned long now = micros();
    unsigned long delta = now - lastUpdate;
    lastUpdate = now;
    float dt = (float)delta / 1000000.0;

    // Get sensor data
    Vector3 acceleration = accelerometer->getAcceleration();
    Vector3 rotation = gyroscope->getRotation();

    // Update telemetry
    Telemetry &telemetry = Telemetry::getInstance();
    telemetry.acceleration = acceleration;
    telemetry.rotation = rotation;

    // Get accelerometer orientation
    // See "Tilt Sensing Using a Three-Axis Accelerometer" eqns 37 and 38
    // http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    double accelRoll = atan2(acceleration.y, copysign(sqrt(acceleration.z * acceleration.z + 0.01 * acceleration.x * acceleration.x), acceleration.z)) * 180.0 / M_PI;
    double accelPitch = atan2(-acceleration.x, sqrt(acceleration.y * acceleration.y + acceleration.z * acceleration.z)) * 180.0 / M_PI;

    // Combine gyroscope and accelerometer data for roll and pitch
    orientation.roll = CF_GYRO_WEIGHT * (orientation.roll + rotation.x * dt) + CF_ACCEL_WEIGHT * accelRoll;
    orientation.pitch = CF_GYRO_WEIGHT * (orientation.pitch + rotation.y * dt) + CF_ACCEL_WEIGHT * accelPitch;

    // Calculate yaw using compass, fall back to gyroscope data
    if(magnetometer) {
        // Get compass heading
        orientation.yaw = magnetometer->getAzimuth();

        // TODO: Compensate heading for hard iron biases

        // TODO: Compensate heading for tilt
        // http://cache.freescale.com/files/sensors/doc/app_note/AN4248.pdf
        // http://forum.arduino.cc/index.php?topic=8573.0
        // compensatedYaw = atan2(-heading.y * cos(orientation.x) + heading.z * sin(orientation.x), heading.x * cos(orientation.y) + heading.z * sin(orientation.y) * sin(orientation.x) + heading.z * sin(orientation.y) * cos(orientation.x)) *180/PI;

        // TODO: Convert from magnetic north to true north?
        // TODO: Lookup declination from geo-ip?
        // http://www.magnetic-declination.com/
    } else {
        // Use gyroscope data for yaw
        float gyroYaw = orientation.yaw + rotation.z * 180.0 / M_PI * dt;

        // Restrict yaw to range of 0-360
        if(gyroYaw < 0)      gyroYaw += 360;
        if(gyroYaw > 360)    gyroYaw -= 360;

        orientation.yaw = gyroYaw;
    }

    telemetry.orientation = orientation;
}

Vector3 AHRS::getOrientation() {
    return orientation;
}

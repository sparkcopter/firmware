#include "application.h"

#include "IMU.h"
#include "Sensors.h"
#include "Telemetry.h"

#define CF_GYRO_WEIGHT      0.90
#define CF_ACCEL_WEIGHT     (1 - CF_GYRO_WEIGHT)
#define MICROS_ROLLOVER ((unsigned long)59652323)

IMU::IMU() {

}

void IMU::initialize() {
    Sensors::initialize();
}

void IMU::groundCalibrate() {
    Sensors::groundCalibrate();
}

void IMU::update() {
    Telemetry *telemetry = Telemetry::getInstance();

    // Get the time since last update
    // NOTE: Spark's micros() function rolls over every ~59 seconds due to a bug!
    // https://community.spark.io/t/micros-rolling-over-after-59-652323-seconds-not-71-minutes-solved
    unsigned long now = micros();
    unsigned long delta = now - lastUpdate;
    if((long)delta < 0) delta += MICROS_ROLLOVER;
    lastUpdate = now;
    double dt = (double)delta / 1000000.0;

    // Get sensor data
    Vector3 acceleration = Sensors::getAccelerometer()->getAcceleration();
    Vector3 rotation = Sensors::getGyroscope()->getRotation();

    // Update telemetry
    telemetry->acceleration = acceleration;
    telemetry->rotation = rotation;

    // Get accelerometer orientation
    // See "Tilt Sensing Using a Three-Axis Accelerometer" eqns 37 and 38
    // http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    double accelRoll = atan2(acceleration.y, copysign(sqrt(acceleration.z * acceleration.z + 0.01 * acceleration.x * acceleration.x), acceleration.z)) * RAD_TO_DEG;
    double accelPitch = atan2(-acceleration.x, sqrt(acceleration.y * acceleration.y + acceleration.z * acceleration.z)) * RAD_TO_DEG;

    // Combine gyroscope and accelerometer data for roll and pitch
    orientation.x = CF_GYRO_WEIGHT * (orientation.x + rotation.x * dt) + CF_ACCEL_WEIGHT * accelRoll;
    orientation.y = CF_GYRO_WEIGHT * (orientation.y + rotation.y * dt) + CF_ACCEL_WEIGHT * accelPitch;

    // Calculate yaw using compass, fall back to gyroscope data
    if(Sensors::compassAvailable()) {
        // Get heading vector
        Vector3 heading = Sensors::getCompass()->getHeading();
        telemetry->heading = heading;

        // Convert to scalar compass heading
        double scalarHeading = atan2(heading.y, heading.x);
        if(scalarHeading < 0) {
            scalarHeading += 2*M_PI;
        }

        orientation.z = scalarHeading * 180/M_PI;
    } else {
        // Use gyroscope data for yaw, restrict range to -180 -> 180
        orientation.z = remainder(orientation.z + rotation.z * dt, 360);
    }

    // Update telemetry
    telemetry->orientation = orientation;
}

Vector3 IMU::getOrientation() {
    return orientation;
}

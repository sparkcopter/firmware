#include "application.h"

#include "IMU.h"
#include "Sensors.h"
#include "Telemetry.h"

IMU::IMU() {

}

void IMU::initialize() {
    Sensors::initialize();
}

void IMU::groundCalibrate() {
    Sensors::groundCalibrate();
}

void IMU::update() {
    Vector3 acceleration = Sensors::getAccelerometer()->getAcceleration();
    Vector3 rotation = Sensors::getGyroscope()->getRotation();

    // Update telemetry
    Telemetry *telemetry = Telemetry::getInstance();
    telemetry->acceleration = acceleration;
    telemetry->rotation = rotation;

    // TODO:
    // - Get Roll and Pitch by combining gyro and accelerometer data

    // If we have a compass
    // - Use it for Yaw
    // else
    // - Use the gyroscope data
}

Vector3 IMU::getOrientation() {
    return orientation;
}

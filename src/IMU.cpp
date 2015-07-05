#include "application.h"

#include "Accelerometer.h"
#include "Gyroscope.h"
#include "IMU.h"
#include "Sensors.h"

IMU::IMU() {

}

void IMU::initialize() {
    Sensors::initialize();
}

void IMU::update() {
    Vector3 acceleration = Sensors::getAccelerometer()->getAcceleration();
    Vector3 rotation = Sensors::getGyroscope()->getRotation();

    Serial.print("Acceleration: ");
    Serial.print(acceleration.x);
    Serial.print(", ");
    Serial.print(acceleration.y);
    Serial.print(", ");
    Serial.print(acceleration.z);
    Serial.println();

    Serial.print("Rotation: ");
    Serial.print(rotation.x);
    Serial.print(", ");
    Serial.print(rotation.y);
    Serial.print(", ");
    Serial.print(rotation.z);
    Serial.println();

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

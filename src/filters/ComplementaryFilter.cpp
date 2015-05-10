#include "ComplementaryFilter.h"

void ComplementaryFilter::update(Vector3 accel, Vector3 gyro) {
    uint32_t now = millis();
    float deltat = ((now - lastUpdate)/1000.0f);
    lastUpdate = now;

    // Integrate gyroscope data (turn radians per second into degrees)
    orientation.x += RADIANS_TO_DEGREES(gyro.x) * deltat;
    orientation.y -= RADIANS_TO_DEGREES(gyro.y) * deltat;
    orientation.z += RADIANS_TO_DEGREES(gyro.z) * deltat;

    // Get accelerometer orientation
    Vector3 accelOrientation;
    accelOrientation.x = RADIANS_TO_DEGREES(atan2f(accel.y, accel.z));
    accelOrientation.y = RADIANS_TO_DEGREES(atan2f(accel.x, accel.z));
    accelOrientation.z = 0; // Yaw data from accelerometer is junk

    // Combine gyroscope and accelerometer data
    orientation.x = orientation.x * CF_GYRO_WEIGHT + accelOrientation.x * CF_ACCEL_WEIGHT;
    orientation.y = orientation.y * CF_GYRO_WEIGHT + accelOrientation.y * CF_ACCEL_WEIGHT;

    // Restrict yaw range to -180 -> 180
    orientation.z = remainder(orientation.z, 360);
}

Vector3 ComplementaryFilter::getOrientation() {
    return orientation;
}

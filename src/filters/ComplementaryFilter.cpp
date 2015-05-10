#include "ComplementaryFilter.h"

void ComplementaryFilter::update(Vector3 accel, Vector3 gyro, double dt) {
    // Get accelerometer orientation
    // See "Tilt Sensing Using a Three-Axis Accelerometer" eqns 37 and 38
    // http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    double accelRoll = atan2(accel.y, copysign(sqrt(accel.z * accel.z + 0.01 * accel.x * accel.x), accel.z)) * RAD_TO_DEG;
    double accelPitch = atan2(-accel.x, sqrt(accel.y * accel.y + accel.z * accel.z)) * RAD_TO_DEG;

    // Combine gyroscope and accelerometer data for roll and pitch
    orientation.x = CF_GYRO_WEIGHT * (orientation.x + gyro.x * dt) + CF_ACCEL_WEIGHT * accelRoll;
    orientation.y = CF_GYRO_WEIGHT * (orientation.y + gyro.y * dt) + CF_ACCEL_WEIGHT * accelPitch;

    // Use gyroscope data for yaw, restrict range to -180 -> 180
    orientation.z = remainder(orientation.z + gyro.z * dt, 360);
}

Vector3 ComplementaryFilter::getOrientation() {
    return orientation;
}

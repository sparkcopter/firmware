#include "MadgwickFilter.h"

void MadgwickFilter::update(Vector3 accel, Vector3 gyro, double dt) {
    // Comvert gyro rates to radians per second
    gyro.x *= DEG_TO_RAD;
    gyro.y *= DEG_TO_RAD;
    gyro.z *= DEG_TO_RAD;

    // Rate of change of quaternion from gyroscope
    float qDot1 = 0.5f * (-q.x * gyro.x - q.y * gyro.y - q.z * gyro.z);
    float qDot2 = 0.5f * (q.w * gyro.x + q.y * gyro.z - q.z * gyro.y);
    float qDot3 = 0.5f * (q.w * gyro.y - q.x * gyro.z + q.z * gyro.x);
    float qDot4 = 0.5f * (q.w * gyro.z + q.x * gyro.y - q.y * gyro.x);

    // Normalise accelerometer measurement
    accel.normalize();

    // Gradient decent algorithm corrective step
    Quarternion step;
    step.w = 4 * q.w * q.y * q.y + 2 * q.y * accel.x + 4 * q.w * q.x * q.x - 2 * q.x * accel.y;
    step.x = 4 * q.x * q.z * q.z - 2 * q.z * accel.x + 4 * q.w * q.w * q.x - 2 * q.w * accel.y - 4 * q.x + 8 * q.x * q.x * q.x + 8 * q.x * q.y * q.y + 4 * q.x * accel.z;
    step.y = 4 * q.w * q.w * q.y + 2 * q.w * accel.x + 4 * q.y * q.z * q.z - 2 * q.z * accel.y - 4 * q.y + 8 * q.y * q.x * q.x + 8 * q.y * q.y * q.y + 4 * q.y * accel.z;
    step.z = 4 * q.x * q.x * q.z - 2 * q.x * accel.x + 4 * q.y * q.y * q.z - 2 * q.y * accel.y;

    // Normalise step magnitude
    step.normalize();

    // Apply feedback step
    qDot1 -= beta * step.w;
    qDot2 -= beta * step.x;
    qDot3 -= beta * step.y;
    qDot4 -= beta * step.z;

    // Integrate rate of change of quaternion to yield quaternion
    q.w += qDot1 * dt;
    q.x += qDot2 * dt;
    q.y += qDot3 * dt;
    q.z += qDot4 * dt;

    // Normalise quaternion
    q.normalize();
}

Vector3 MadgwickFilter::getOrientation() {
    return q.toEuler();
}

#include "MadgwickFilter.h"

void MadgwickFilter::update(Vector3 accel, Vector3 gyro, double dt) {
    // Convert gyro readings into radians per second
    gyro.x *= DEG_TO_RAD;
    gyro.y *= DEG_TO_RAD;
    gyro.z *= DEG_TO_RAD;

    float norm;                                               // vector norm
    float f1, f2, f3;                                         // objetive funcyion elements
    float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements
    float qDot1, qDot2, qDot3, qDot4;
    float hatDot1, hatDot2, hatDot3, hatDot4;
    float gerrx, gerry, gerrz, gbiasx, gbiasy, gbiasz;        // gyro bias error

    // Auxiliary variables to avoid repeated arithmetic
    float _halfq1 = 0.5f * q1;
    float _halfq2 = 0.5f * q2;
    float _halfq3 = 0.5f * q3;
    float _halfq4 = 0.5f * q4;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q4 = 2.0f * q4;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q3q4 = 2.0f * q3 * q4;

    // Normalise accelerometer measurement
    norm = sqrt(accel.x * accel.x + accel.y * accel.y + accel.z * accel.z);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    accel.x *= norm;
    accel.y *= norm;
    accel.z *= norm;

    // Compute the objective function and Jacobian
    f1 = _2q2 * q4 - _2q1 * q3 - accel.x;
    f2 = _2q1 * q2 + _2q3 * q4 - accel.y;
    f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - accel.z;
    J_11or24 = _2q3;
    J_12or23 = _2q4;
    J_13or22 = _2q1;
    J_14or21 = _2q2;
    J_32 = 2.0f * J_14or21;
    J_33 = 2.0f * J_11or24;

    // Compute the gradient (matrix multiplication)
    hatDot1 = J_14or21 * f2 - J_11or24 * f1;
    hatDot2 = J_12or23 * f1 + J_13or22 * f2 - J_32 * f3;
    hatDot3 = J_12or23 * f2 - J_33 *f3 - J_13or22 * f1;
    hatDot4 = J_14or21 * f1 + J_11or24 * f2;

    // Normalize the gradient
    norm = sqrt(hatDot1 * hatDot1 + hatDot2 * hatDot2 + hatDot3 * hatDot3 + hatDot4 * hatDot4);
    hatDot1 /= norm;
    hatDot2 /= norm;
    hatDot3 /= norm;
    hatDot4 /= norm;

    // Compute estimated gyroscope biases
    gerrx = _2q1 * hatDot2 - _2q2 * hatDot1 - _2q3 * hatDot4 + _2q4 * hatDot3;
    gerry = _2q1 * hatDot3 + _2q2 * hatDot4 - _2q3 * hatDot1 - _2q4 * hatDot2;
    gerrz = _2q1 * hatDot4 - _2q2 * hatDot3 + _2q3 * hatDot2 - _2q4 * hatDot1;

    // Compute and remove gyroscope biases
    gbiasx += gerrx * dt * zeta;
    gbiasy += gerry * dt * zeta;
    gbiasz += gerrz * dt * zeta;
    gyro.x -= gbiasx;
    gyro.y -= gbiasy;
    gyro.z -= gbiasz;

    // Compute the quaternion derivative
    qDot1 = -_halfq2 * gyro.x - _halfq3 * gyro.y - _halfq4 * gyro.z;
    qDot2 =  _halfq1 * gyro.x + _halfq3 * gyro.z - _halfq4 * gyro.y;
    qDot3 =  _halfq1 * gyro.y - _halfq2 * gyro.z + _halfq4 * gyro.x;
    qDot4 =  _halfq1 * gyro.z + _halfq2 * gyro.y - _halfq3 * gyro.x;

    // Compute then integrate estimated quaternion derivative
    q1 += (qDot1 -(beta * hatDot1)) * dt;
    q2 += (qDot2 -(beta * hatDot2)) * dt;
    q3 += (qDot3 -(beta * hatDot3)) * dt;
    q4 += (qDot4 -(beta * hatDot4)) * dt;

    // Normalize the quaternion
    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;
    q4 *= norm;
}

Vector3 MadgwickFilter::getOrientation() {
    Vector3 orientation;

    orientation.x = atan2(2.0f * (q1*q2 + q3*q4), q1*q1 - q2*q2 - q3*q3 + q4*q4);   // Roll
    orientation.y = -asin(2.0f * (q2*q4 - q1*q3));                                  // Pitch
    orientation.z = atan2(2.0f * (q2*q3 + q1*q4), q1*q1 + q2*q2 - q3*q3 - q4*q4);   // Yaw

    // Convert from radians per second to degrees per second
    orientation.x *= 180.0f / M_PI;
    orientation.y *= 180.0f / M_PI;
    orientation.z *= 180.0f / M_PI;

    return orientation;
}

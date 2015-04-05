#include "IMU.h"
#include "Logger.h"

IMU::IMU() {

}

void IMU::initialize() {
    accelgyro.initialize();

    // Get configured accelerometer and gyroscope scales
    accelScale = 2048.0 * pow(2, 3 - accelgyro.getFullScaleAccelRange());
    gyroScale = 16.4 * pow(2, 3 - accelgyro.getFullScaleGyroRange());
}

void IMU::calibrate() {
    // Reset device registers and initialize device
    accelgyro.reset();
    accelgyro.initialize();

    // Set low-pass filter to 188 Hz
    accelgyro.setDLPFMode(MPU6050_DLPF_BW_188);

    // Set sample rate to 1 kHz
    accelgyro.setRate(0);

    // Wait for stable gyroscope data
    delay(200);

    // Enable FIFO
    accelgyro.setFIFOEnabled(true);

    // - Enable gyro and accelerometer sensors for FIFO
    // - Accumulate 80 samples in 80 milliseconds = 960 bytes (max fifo size is 1024)
    // - Disable fifo sensors
    I2Cdev::writeByte(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_FIFO_EN, 0x78);
    delay(80);
    I2Cdev::writeByte(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_FIFO_EN, 0x0);

    // Combine all packets of calibration data
    int32_t gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0};
    uint16_t packetCount = accelgyro.getFIFOCount()/12;
    for(int i=0; i<packetCount; i++) {
        // Read this packet's gyro and accel data
        int16_t accelTemp[3] = {0, 0, 0}, gyroTemp[3] = {0, 0, 0};
        I2Cdev::readWords(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_FIFO_R_W, 3, (uint16_t *)accelTemp);
        I2Cdev::readWords(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_FIFO_R_W, 3, (uint16_t *)gyroTemp);

        // Add this packet to the bias totals
        for(int d=0; d<3; d++) {
            accelBias[d] += accelTemp[d];
            gyroBias[d] += gyroTemp[d];
        }
    }

    // Average the calibration data
    for(int d=0; d<3; d++) {
        accelBias[d] /= packetCount;
        gyroBias[d] /= packetCount;
    }

    // Disable FIFO
    accelgyro.setFIFOEnabled(false);

    // Remove gravity from z-axis of accelerometer bias
    uint16_t  accelSensitivity = 16384;  // = 1g
    accelBias[2] > 0 ? accelBias[2] -= accelSensitivity : accelBias[2] += accelSensitivity;

    // Push gyro biases to hardware registers
    // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    accelgyro.setXGyroOffset(-gyroBias[0] / 4);
    accelgyro.setYGyroOffset(-gyroBias[1] / 4);
    accelgyro.setZGyroOffset(-gyroBias[2] / 4);

    // Read factory accelerometer trim values
    // Write total accelerometer bias, including calculated average accelerometer bias from above
    // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
    accelgyro.setXAccelOffset(accelgyro.getXAccelOffset() - (accelBias[0] / 8));
    accelgyro.setYAccelOffset(accelgyro.getYAccelOffset() - (accelBias[1] / 8));
    accelgyro.setZAccelOffset(accelgyro.getZAccelOffset() - (accelBias[2] / 8));

    Logger::info("Calibrated sensors!");
    Logger::info("- Accelerometer bias: x=%6d y=%6d z=%6d", accelBias[0], accelBias[1], accelBias[2]);
    Logger::info("- Gyroscope bias:     x=%6d y=%6d z=%6d", gyroBias[0], gyroBias[1], gyroBias[2]);
}

void IMU::update() {
    int16_t ax, ay, az, gx, gy, gz;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    madgwickQuaternionUpdate(ax/accelScale, ay/accelScale, az/accelScale, gx/gyroScale*M_PI/180.0f, gy/gyroScale*M_PI/180.0f, gz/gyroScale*M_PI/180.0f);
}

void IMU::getQuarternions(float *q1, float *q2, float *q3, float *q4) {
    *q1 = this->q1;
    *q2 = this->q2;
    *q3 = this->q3;
    *q4 = this->q4;
}

void IMU::getYawPitchRoll(float *yaw, float *pitch, float *roll) {
    *yaw   = atan2(2.0f * (q2*q3 + q1*q4), q1*q1 + q2*q2 - q3*q3 - q4*q4);
    *pitch = -asin(2.0f * (q2*q4 - q1*q3));
    *roll  = atan2(2.0f * (q1*q2 + q3*q4), q1*q1 - q2*q2 - q3*q3 + q4*q4);

    *pitch *= 180.0f / M_PI;
    *yaw   *= 180.0f / M_PI;
    *roll  *= 180.0f / M_PI;
}

void IMU::madgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz) {
    uint32_t now = micros();
    float deltat = ((now - lastUpdate)/1000000.0f);
    lastUpdate = now;

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
    norm = sqrt(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Compute the objective function and Jacobian
    f1 = _2q2 * q4 - _2q1 * q3 - ax;
    f2 = _2q1 * q2 + _2q3 * q4 - ay;
    f3 = 1.0f - _2q2 * q2 - _2q3 * q3 - az;
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
    gbiasx += gerrx * deltat * zeta;
    gbiasy += gerry * deltat * zeta;
    gbiasz += gerrz * deltat * zeta;
    gx -= gbiasx;
    gy -= gbiasy;
    gz -= gbiasz;

    // Compute the quaternion derivative
    qDot1 = -_halfq2 * gx - _halfq3 * gy - _halfq4 * gz;
    qDot2 =  _halfq1 * gx + _halfq3 * gz - _halfq4 * gy;
    qDot3 =  _halfq1 * gy - _halfq2 * gz + _halfq4 * gx;
    qDot4 =  _halfq1 * gz + _halfq2 * gy - _halfq3 * gx;

    // Compute then integrate estimated quaternion derivative
    q1 += (qDot1 -(beta * hatDot1)) * deltat;
    q2 += (qDot2 -(beta * hatDot2)) * deltat;
    q3 += (qDot3 -(beta * hatDot3)) * deltat;
    q4 += (qDot4 -(beta * hatDot4)) * deltat;

    // Normalize the quaternion
    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;
    q4 *= norm;
}

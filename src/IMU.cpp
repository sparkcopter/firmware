#include "IMU.h"
#include "Logger.h"
#include "MadgwickFilter.h"
#include "ComplementaryFilter.h"

IMU::IMU(uint8_t filterType) {
    switch(filterType) {
        case IMU_FILTER_COMPLEMENTARY:
            this->filter = new ComplementaryFilter();
            break;
        case IMU_FILTER_MADGWICK:
            this->filter = new MadgwickFilter();
            break;
        default:
            break;
    }
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
    int16_t rax, ray, raz, rgx, rgy, rgz;
    accelgyro.getMotion6(&rax, &ray, &raz, &rgx, &rgy, &rgz);

    // Convert raw accelerometer readings into Gs
    accel.x = (float)rax/accelScale;
    accel.y = (float)ray/accelScale;
    accel.z = (float)raz/accelScale;

    // Convert raw gyroscope readings into radians per second
    gyro.x = (float)rgx/gyroScale * M_PI/180.0f;
    gyro.y = (float)rgy/gyroScale * M_PI/180.0f;
    gyro.z = (float)rgz/gyroScale * M_PI/180.0f;

    // Update filter
    filter->update(accel, gyro);
}

Vector3 IMU::getOrientation() {
    return filter->getOrientation();
}

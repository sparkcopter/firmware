#include "MPU6050.h"
#include "Logger.h"

MPU6050::MPU6050() {
    this->device = new I2C(MPU6050_DEFAULT_ADDRESS);
}

MPU6050::MPU6050(uint8_t address) {
    this->device = new I2C(address);
}

void MPU6050::initialize() {
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    setSleepEnabled(false);
}

void MPU6050::reset() {
    device->writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, true);
}

void MPU6050::setSleepEnabled(bool enabled) {
    device->writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

void MPU6050::setClockSource(uint8_t source) {
    device->writeBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

void MPU6050::setFullScaleGyroRange(uint8_t range) {
    device->writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

void MPU6050::setFullScaleAccelRange(uint8_t range) {
    device->writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

void MPU6050::setDLPFMode(uint8_t mode) {
    device->writeBits(MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

void MPU6050::setRate(uint8_t rate) {
    device->writeByte(MPU6050_RA_SMPLRT_DIV, rate);
}

void MPU6050::setFifoEnabled(bool enabled) {
    device->writeBit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, enabled);
}

void MPU6050::calibrate() {
    uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
    uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

    // Reset device registers and biases and wait for reset to complete
    reset();
    delay(100);

    // Disable sleep mode
    setSleepEnabled(false);

    // Set low-pass filter to 188 Hz
    setDLPFMode(MPU6050_DLPF_BW_188);

    // Set sample rate to 1 kHz
    setRate(0);

    // Set gyro full-scale to 250 degrees per second, maximum sensitivity
    setFullScaleGyroRange(MPU6050_GYRO_FS_250);

    // Set accelerometer full-scale to 2 g, maximum sensitivity
    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

    // Configure FIFO to capture accelerometer and gyro data for bias calculation
    setFifoEnabled(true);

    // - Enable gyro and accelerometer sensors for FIFO
    // - Accumulate 80 samples in 80 milliseconds = 960 bytes (max fifo size is 1024)
    // - Disable fifo sensors
    device->writeByte(MPU6050_RA_FIFO_EN, 0x78);
    delay(80);
    device->writeByte(MPU6050_RA_FIFO_EN, 0x0);

    // Read FIFO sample count
    uint16_t fifoCount;
    device->readWord(MPU6050_RA_FIFO_COUNTH, &fifoCount);
    uint16_t packetCount = fifoCount/12;

    // Combine all packets of calibration data
    int32_t gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0};
    for(int i=0; i<packetCount; i++) {
        // Read this packet's gyro and accel data
        int16_t accelTemp[3] = {0, 0, 0}, gyroTemp[3] = {0, 0, 0};
        device->readWords(MPU6050_RA_FIFO_R_W, 3, (uint16_t *)accelTemp);
        device->readWords(MPU6050_RA_FIFO_R_W, 3, (uint16_t *)gyroTemp);

        // Add this packet to the bias totals
        for(int d=0; d<3; d++) {
            accelBias[d] += accelTemp[d];
            gyroBias[d] += gyroTemp[d];
        }
    }

    // Get the average of the calibration data
    for(int d=0; d<3; d++) {
        accelBias[d] /= packetCount;
        gyroBias[d] /= packetCount;
    }

    // Remove gravity from z-axis of accelerometer bias
    uint16_t  accelSensitivity = 16384;  // = 1g
    accelBias[2] > 0 ? accelBias[2] -= accelSensitivity : accelBias[2] += accelSensitivity;

    // Push gyro biases to hardware registers
    // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    device->writeWord(MPU6050_RA_XG_OFFS_USRH, -gyroBias[0] / 4);
    device->writeWord(MPU6050_RA_YG_OFFS_USRH, -gyroBias[1] / 4);
    device->writeWord(MPU6050_RA_ZG_OFFS_USRH, -gyroBias[2] / 4);

    // Read factory accelerometer trim values
    int16_t accelBiasFactory[3];
    device->readWord(MPU6050_RA_XA_OFFS_H, (uint16_t *)&accelBiasFactory[0]);
    device->readWord(MPU6050_RA_YA_OFFS_H, (uint16_t *)&accelBiasFactory[1]);
    device->readWord(MPU6050_RA_ZA_OFFS_H, (uint16_t *)&accelBiasFactory[2]);

    // Write total accelerometer bias, including calculated average accelerometer bias from above
    // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
    device->writeWord(MPU6050_RA_XA_OFFS_H, accelBiasFactory[0] - (accelBias[0]/8));
    device->writeWord(MPU6050_RA_YA_OFFS_H, accelBiasFactory[1] - (accelBias[1]/8));
    device->writeWord(MPU6050_RA_ZA_OFFS_H, accelBiasFactory[2] - (accelBias[2]/8));

    Logger::info("Calibrated sensors!");
    Logger::info("- Accelerometer bias: x=%6d y=%6d z=%6d", accelBias[0], accelBias[1], accelBias[2]);
    Logger::info("- Gyroscope bias:     x=%6d y=%6d z=%6d", gyroBias[0], gyroBias[1], gyroBias[2]);
}

void MPU6050::getRawMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    // Read 7 16-bit values, starting at the first byte of accelerometer data
    // - 3 values for accelerometer
    // - 1 value for temperature (ignored)
    // - 3 values for gyroscope
    device->readWords(MPU6050_RA_ACCEL_XOUT_H, 7, wordBuffer);

    *ax = wordBuffer[0];
    *ay = wordBuffer[1];
    *az = wordBuffer[2];
    *gx = wordBuffer[4];
    *gy = wordBuffer[5];
    *gz = wordBuffer[6];
}

void MPU6050::getRawAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    // Read 3 16-bit values, starting at the first byte of accelerometer data
    device->readWords(MPU6050_RA_ACCEL_XOUT_H, 3, wordBuffer);

    // 3 axes of accelerometer data
    *x = wordBuffer[0];
    *y = wordBuffer[1];
    *z = wordBuffer[2];
}

void MPU6050::getRawRotation(int16_t* x, int16_t* y, int16_t* z) {
    // Read 3 16-bit values, starting at the first byte of gyroscope data
    device->readWords(MPU6050_RA_GYRO_XOUT_H, 3, wordBuffer);

    // 3 axes of accelerometer data
    *x = wordBuffer[0];
    *y = wordBuffer[1];
    *z = wordBuffer[2];
}

int16_t MPU6050::getRawTemperature() {
    // Read 1 16-bit value, starting at the first byte of temperature data
    device->readWord(MPU6050_RA_TEMP_OUT_H, wordBuffer);
    return wordBuffer[0];
}

void MPU6050::getMotion(float* ax, float* ay, float* az, float* gx, float* gy, float* gz) {
    // TODO
}

void MPU6050::getAcceleration(float* x, float* y, float* z) {
    // TODO
}

void MPU6050::getRotation(float* x, float* y, float* z) {
    // TODO
}

int16_t MPU6050::getTemperature() {
    // Adjust raw data into celcius
    return getRawTemperature()/340.00 + 36.53;
}

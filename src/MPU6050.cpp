#include "MPU6050.h"

MPU6050::MPU6050() {
    this->address = MPU6050_DEFAULT_ADDRESS;
}

MPU6050::MPU6050(uint8_t address) {
    this->address = address;
}

void MPU6050::init() {
    // Initiate the Wire library and join the I2C bus as a master
    Wire.begin();

    // Wake up the MPU-6050 for communication
    Wire.beginTransmission(this->address);
    Wire.write(MPU6050_RA_PWR_MGMT_1);
    Wire.write(0);
    Wire.endTransmission(true);

    // Set up default gyro and accelerometers ranges
    setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
}

void MPU6050::setFullScaleGyroRange(uint8_t range) {
    // Set the gyro range on the chip
    writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);

    // Calculate and save the gyro scaling factor
    switch(range) {
      case MPU6050_GYRO_FS_250:
            this->gyroRes = 250.0/32768.0;
            break;
      case MPU6050_GYRO_FS_500:
            this->gyroRes = 500.0/32768.0;
            break;
      case MPU6050_GYRO_FS_1000:
            this->gyroRes = 1000.0/32768.0;
            break;
      case MPU6050_GYRO_FS_2000:
            this->gyroRes = 2000.0/32768.0;
            break;
    }
}

void MPU6050::setFullScaleAccelRange(uint8_t range) {
    // Set the accelerometer range on the chip
    writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);

    // Calculate and save the accelerometer scaling factor
    switch(range) {
      case MPU6050_ACCEL_FS_2:
            this->accelRes = 2.0/32768.0;
            break;
      case MPU6050_ACCEL_FS_4:
            this->accelRes = 4.0/32768.0;
            break;
      case MPU6050_ACCEL_FS_8:
            this->accelRes = 8.0/32768.0;
            break;
      case MPU6050_ACCEL_FS_16:
            this->accelRes = 16.0/32768.0;
            break;
    }
}

void MPU6050::getRawMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    // Read 14 bytes of data, starting at the first byte of accelerometer data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    readBytes(MPU6050_RA_ACCEL_XOUT_H, 14, buffer);

    // 3 axes of accelerometer data
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];

    // Note: next 2 bytes are temperature data, which we ignore

    // 3 axes of gyroscope data
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

void MPU6050::getRawAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    // Read 6 bytes of data, starting at the first byte of accelerometer data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    readBytes(MPU6050_RA_ACCEL_XOUT_H, 6, buffer);

    // 3 axes of accelerometer data
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050::getRawRotation(int16_t* x, int16_t* y, int16_t* z) {
    // Read 6 bytes of data, starting at the first byte of gyroscope data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    readBytes(MPU6050_RA_GYRO_XOUT_H, 6, buffer);

    // 3 axes of gyroscope data
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t MPU6050::getRawTemperature() {
    // Read 2 bytes of data, starting at the first byte of temperature data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    readBytes(MPU6050_RA_TEMP_OUT_H, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

void MPU6050::getMotion(float* ax, float* ay, float* az, float* gx, float* gy, float* gz) {
    int16_t rax, ray, raz, rgx, rgy, rgz;
    getRawMotion(&rax, &ray, &raz, &rgx, &rgy, &rgz);

    *ax = (float)rax * accelRes;
    *ay = (float)ray * accelRes;
    *az = (float)raz * accelRes;

    *gx = (float)rgx * gyroRes;
    *gy = (float)rgy * gyroRes;
    *gz = (float)rgz * gyroRes;
}

void MPU6050::getAcceleration(float* x, float* y, float* z) {

}

void MPU6050::getRotation(float* x, float* y, float* z) {

}

int16_t MPU6050::getTemperature() {
    // Adjust raw data into celcius
    return getRawTemperature()/340.00 + 36.53;
}

int8_t MPU6050::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    int8_t count = 0;

    // Request n bytes of register data from the i2c bus
    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.endTransmission(false);
    Wire.beginTransmission(regAddr);
    Wire.requestFrom(this->address, length);

    // Read requested registers into a buffer
    for(; Wire.available(); count++) {
        data[count] = Wire.read();
    }

    return count;
}

int8_t MPU6050::readByte(uint8_t regAddr, uint8_t *data) {
    return readBytes(regAddr, 1, data);
}

bool MPU6050::writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    uint8_t status = 0;

    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.write(data, length);
    status = Wire.endTransmission();

    return status == 0;
}

bool MPU6050::writeByte(uint8_t regAddr, uint8_t data) {
    return writeBytes(regAddr, 1, &data);
}

bool MPU6050::writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    if(readByte(regAddr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(regAddr, b);
    } else {
        return false;
    }
}

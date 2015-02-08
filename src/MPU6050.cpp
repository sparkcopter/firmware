#include "MPU6050.h"

MPU6050::MPU6050() {
    this->address = MPU6050_DEFAULT_ADDRESS;
}

MPU6050::MPU6050(uint8_t address) {
    this->address = address;
}

void MPU6050::init() {
    Wire.begin();
    Wire.beginTransmission(this->address);
    Wire.write(MPU6050_PWR_MGMT_1);
    Wire.write(0);
    Wire.endTransmission(true);
}

void MPU6050::getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    this->readBytes(MPU6050_ACCEL_XOUT_H, 14, buffer);

    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];

    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

void MPU6050::getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    this->readBytes(MPU6050_ACCEL_XOUT_H, 6, buffer);

    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050::getRotation(int16_t* x, int16_t* y, int16_t* z) {
    this->readBytes(MPU6050_GYRO_XOUT_H, 6, buffer);

    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t MPU6050::getTemperature() {
    this->readBytes(MPU6050_TEMP_OUT_H, 2, buffer);

    int16_t temp = (((int16_t)buffer[0]) << 8) | buffer[1];

    return temp/340.00+36.53;
}

void MPU6050::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.endTransmission(false);
    Wire.requestFrom(this->address, length, true);

    for(int8_t i = 0; i < length; i++) {
        data[i] = Wire.read();
    }
}

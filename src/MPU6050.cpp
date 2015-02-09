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
    Wire.write(MPU6050_PWR_MGMT_1);
    Wire.write(0);
    Wire.endTransmission(true);
}

void MPU6050::getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    // Read 14 bytes of data, starting at the first byte of accelerometer data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    this->readBytes(MPU6050_ACCEL_XOUT_H, 14, buffer);

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

void MPU6050::getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
    // Read 6 bytes of data, starting at the first byte of accelerometer data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    this->readBytes(MPU6050_ACCEL_XOUT_H, 6, buffer);

    // 3 axes of accelerometer data
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

void MPU6050::getRotation(int16_t* x, int16_t* y, int16_t* z) {
    // Read 6 bytes of data, starting at the first byte of gyroscope data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    this->readBytes(MPU6050_GYRO_XOUT_H, 6, buffer);

    // 3 axes of gyroscope data
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}

int16_t MPU6050::getTemperature() {
    // Read 2 bytes of data, starting at the first byte of temperature data
    // Sensor data is 16-bit, so we have to read 2 bytes for each sensor
    this->readBytes(MPU6050_TEMP_OUT_H, 2, buffer);
    int16_t temp = (((int16_t)buffer[0]) << 8) | buffer[1];

    // Adjust raw data into celcius
    return temp/340.00+36.53;
}

void MPU6050::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    // Request n bytes of register data from the i2c bus
    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.endTransmission(false);
    Wire.requestFrom(this->address, length, true);

    // Read requested registers into a buffer
    for(int8_t i = 0; i < Wire.available(); i++) {
        data[i] = Wire.read();
    }
}

#include "stdint.h"
#include "application.h"

#define MPU6050_DEFAULT_ADDRESS  0x68
#define MPU6050_PWR_MGMT_1       0x6B

// Register addresses
#define MPU6050_ACCEL_XOUT_H     0x3B
#define MPU6050_TEMP_OUT_H       0x41
#define MPU6050_GYRO_XOUT_H      0x43

class MPU6050 {
public:
    MPU6050();
    MPU6050(uint8_t address);

    // Wake up the MPU-6050
    void init();

    // Get current accelerometer and gryoscope readings
    void getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

    // Get current accelerometer readings
    void getAcceleration(int16_t* x, int16_t* y, int16_t* z);

    // Get current gyroscope readings
    void getRotation(int16_t* x, int16_t* y, int16_t* z);

    // Get current temperature
    int16_t getTemperature();

private:
    void readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
    uint8_t buffer[14];
    uint8_t address;
};

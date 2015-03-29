#include "application.h"

#define MPU6050_DEFAULT_ADDRESS  0x68

// Register addresses
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_PWR_MGMT_1       0x6B

// Config register bits
#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2
#define MPU6050_ACONFIG_AFS_SEL_BIT     4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH  2

// Full-scale gyroscope ranges
#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

// Full-scale accelerometer ranges
#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

class MPU6050 {
public:
    // Constructors
    MPU6050();
    MPU6050(uint8_t address);

    // Wake up the MPU-6050
    void init();

    // Calibrate the sensors
    void calibrate(float *gyroBias, float *accelBias);

    // Set full-scale gyroscope range
    void setFullScaleGyroRange(uint8_t range);

    // Set full-scale accelerometer range
    void setFullScaleAccelRange(uint8_t range);

    // Get raw accelerometer and gryoscope readings
    void getRawMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

    // Get raw accelerometer readings
    void getRawAcceleration(int16_t* x, int16_t* y, int16_t* z);

    // Get raw gyroscope readings
    void getRawRotation(int16_t* x, int16_t* y, int16_t* z);

    // Get raw temperature reading
    int16_t getRawTemperature();

    // Get scaled motion readings (acceleration in Gs, rotation in degrees per second)
    void getMotion(float* ax, float* ay, float* az, float* gx, float* gy, float* gz);

    // Get scaled accelerometer readings (in Gs)
    void getAcceleration(float* x, float* y, float* z);

    // Get scaled gyroscope readings (in degrees per second)
    void getRotation(float* x, float* y, float* z);

    // Get scaled temperature reading
    int16_t getTemperature();

private:
    int8_t readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
    int8_t readByte(uint8_t regAddr, uint8_t *data);

    bool writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
    bool writeByte(uint8_t regAddr, uint8_t data);
    bool writeBits(uint8_t regAddr, uint8_t start, uint8_t length, uint8_t data);

    uint8_t buffer[14];
    uint8_t address;
    float gyroRes;
    float accelRes;
};

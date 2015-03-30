#pragma once

#include "I2C.h"

#define MPU6050_DEFAULT_ADDRESS  0x68

// Register addresses
#define MPU6050_RA_XA_OFFS_H        0x06
#define MPU6050_RA_YA_OFFS_H        0x08
#define MPU6050_RA_ZA_OFFS_H        0x0A
#define MPU6050_RA_XG_OFFS_USRH     0x13
#define MPU6050_RA_YG_OFFS_USRH     0x15
#define MPU6050_RA_ZG_OFFS_USRH     0x17
#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_FIFO_EN          0x23
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_FIFO_COUNTH      0x72
#define MPU6050_RA_FIFO_R_W         0x74

// CFG register bits
#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6050_CFG_DLPF_CFG_BIT    2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

// GCONFIG register bits
#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

// ACONFIG register bits
#define MPU6050_ACONFIG_AFS_SEL_BIT     4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH  2

// FIFO register bits
#define MPU6050_TEMP_FIFO_EN_BIT    7
#define MPU6050_XG_FIFO_EN_BIT      6
#define MPU6050_YG_FIFO_EN_BIT      5
#define MPU6050_ZG_FIFO_EN_BIT      4
#define MPU6050_ACCEL_FIFO_EN_BIT   3
#define MPU6050_SLV2_FIFO_EN_BIT    2
#define MPU6050_SLV1_FIFO_EN_BIT    1
#define MPU6050_SLV0_FIFO_EN_BIT    0

// USERCTRL register bits
#define MPU6050_USERCTRL_DMP_EN_BIT             7
#define MPU6050_USERCTRL_FIFO_EN_BIT            6
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU6050_USERCTRL_DMP_RESET_BIT          3
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

// PWR1 register bits
#define MPU6050_PWR1_DEVICE_RESET_BIT   7
#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_PWR1_CYCLE_BIT          5
#define MPU6050_PWR1_TEMP_DIS_BIT       3
#define MPU6050_PWR1_CLKSEL_BIT         2
#define MPU6050_PWR1_CLKSEL_LENGTH      3

// CFG digital low-pass filter modes
#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06

// GCONFIG full-scale gyroscope ranges
#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

// ACONFIG full-scale accelerometer ranges
#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

// PWR1 clock sources
#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07


class MPU6050 {
public:
    // Constructors
    MPU6050();
    MPU6050(uint8_t address);

    // Wake up the MPU-6050
    void initialize();

    // Reset device
    void reset();

    // Set sleep mode status
    void setSleepEnabled(bool enabled);

    // Set the clock source
    void setClockSource(uint8_t source);

    // Set full-scale gyroscope range
    void setFullScaleGyroRange(uint8_t range);

    // Set full-scale accelerometer range
    void setFullScaleAccelRange(uint8_t range);

    // Set digital low-pass filter mode
    void setDLPFMode(uint8_t mode);

    // Set gyroscope sample rate divider
    void setRate(uint8_t rate);

    // Set FIFO enabled status
    void setFifoEnabled(bool enabled);

    // Calibrate the sensors
    void calibrate();

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
    I2C *device;
    uint8_t buffer[14];
    uint16_t wordBuffer[7];
    float gyroRes;
    float accelRes;
};

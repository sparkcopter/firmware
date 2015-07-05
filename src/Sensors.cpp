#include "defines.h"
#include "Sensors.h"

#ifdef AK8975_INSTALLED
#include "HAL_AK8975.h"
#endif

#ifdef BMP085_INSTALLED
#include "HAL_BMP085.h"
#endif

#ifdef HMC5883L_INSTALLED
#include "HAL_HMC5883L.h"
#endif

#ifdef MPU6050_INSTALLED
#include "HAL_MPU6050.h"
#endif

void Sensors::initialize() {
    #ifdef AK8975_INSTALLED
    HAL_AK8975::getInstance().initialize();
    #endif

    #ifdef BMP085_INSTALLED
    HAL_BMP085::getInstance().initialize();
    #endif

    #ifdef HMC5883L_INSTALLED
    HAL_HMC5883L::getInstance().initialize();
    #endif

    #ifdef MPU6050_INSTALLED
    HAL_MPU6050::getInstance().initialize();
    #endif
}

Accelerometer *Sensors::getAccelerometer() {
    #if ACCELEROMETER_TYPE == MPU6050
    return &HAL_MPU6050::getInstance();
    #else
    #error No accelerometer configured! Make sure to set ACCELEROMETER_TYPE in defines.h
    #endif
}

Barometer *Sensors::getBarometer() {
    #if BAROMETER_TYPE == BMP085
    return &HAL_BMP085::getInstance();
    #else
    #warning No barometer configured! Make sure to set BAROMETER_TYPE in defines.h
    return NULL;
    #endif
}

Compass *Sensors::getCompass() {
    #if COMPASS_TYPE == HMC5883L
    return &HAL_HMC5883L::getInstance();
    #elif COMPASS_TYPE == AK8975
    return &HAL_AK8975::getInstance();
    #else
    #warning No compass configured! Make sure to set COMPASS_TYPE in defines.h
    return NULL;
    #endif
}

Gyroscope *Sensors::getGyroscope() {
    #if GYROSCOPE_TYPE == MPU6050
    return &HAL_MPU6050::getInstance();
    #else
    #error No gyroscope configured! Make sure to set GYROSCOPE_TYPE in defines.h
    #endif
}

bool barometerAvailable() {
    #ifdef BAROMETER_TYPE
    return true;
    #else
    return false;
    #endif
}

bool compassAvailable() {
    #ifdef COMPASS_TYPE
    return true;
    #else
    return false;
    #endif
}

#pragma once

//
// Which sensor devices are installed?
//

// The AK8963 is API compatible with the AK8975
// #define AK8975_INSTALLED

// The BMP180 is API compatible with with BMP085
#define BMP085_INSTALLED

// HMC5883L compass
#define HMC5883L_INSTALLED

// The MPU6500 is API compatible with the MPU6050
#define MPU6050_INSTALLED

// The MPU9150 contains both a MPU6050 and AK8975 sensor
// #define MPU9150_INSTALLED

// The MPU9150 contains both a MPU6050 and AK8975 sensor
// #define MPU9250_INSTALLED


//
// Which devices should we use for each sensor type?
//

#ifdef AK8975_INSTALLED
  #define COMPASS_TYPE AK8975
#endif

#ifdef BMP085_INSTALLED
  #define BAROMETER_TYPE BMP085
#endif

#ifdef HMC5883L_INSTALLED
  #define COMPASS_TYPE HMC5883L
#endif

#ifdef MPU6050_INSTALLED
  #define ACCELEROMETER_TYPE MPU6050
#endif

#ifdef MPU9150_INSTALLED
  #define ACCELEROMETER_TYPE MPU6050
  #define COMPASS_TYPE AK8975
#endif

#ifdef MPU9250_INSTALLED
  #define ACCELEROMETER_TYPE MPU6050
  #define COMPASS_TYPE AK8975
#endif

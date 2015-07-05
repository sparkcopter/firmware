#include "application.h"

// TODO: Remove when this is fixed upstream
// https://community.particle.io/t/compilation-of-core-code-for-photon-gives-errors/12195/11
#ifdef PLATFORM_ID
#include "i2c_hal.h"
#endif

#include "IMU.h"
#include "MPU6050.h"

SYSTEM_MODE(AUTOMATIC);

IMU imu;

void setup() {
    Serial.begin(115200);

    Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();

    imu.initialize();
}

void loop() {
    imu.update();
    Vector3 orientation = imu.getOrientation();

    delay(10);
}

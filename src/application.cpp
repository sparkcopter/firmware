#include "application.h"

// TODO: Remove when this is fixed upstream
// https://community.particle.io/t/compilation-of-core-code-for-photon-gives-errors/12195/11
#ifdef PLATFORM_ID
#include "i2c_hal.h"
#endif

#include "IMU.h"
#include "MPU6050.h"
#include "NavdataTelemetryTransport.h"
#include "SerialTelemetryTransport.h"
#include "Telemetry.h"

IMU imu;
Telemetry *telemetry = Telemetry::getInstance();

void setup() {
    // Disconnect from the Spark cloud, to avoid pauses
    Spark.disconnect();

    // Activate high-speed i2c
    Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();

    imu.initialize();

    telemetry->addTransport(new SerialTelemetryTransport());
    telemetry->addTransport(new NavdataTelemetryTransport());

    RGB.control(true);
    RGB.color(255, 165, 0);
}

void loop() {
    imu.update();
    Vector3 orientation = imu.getOrientation();

    telemetry->send();

    delay(10);
}

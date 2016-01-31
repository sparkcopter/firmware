#include "application.h"

#include "IMU.h"
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

    // TODO: Maybe remove this?
    delay(10);
}

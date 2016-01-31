#include "application.h"

#include "AHRS.h"
#include "SerialTelemetryTransport.h"
#include "Telemetry.h"

AHRS ahrs;

Telemetry *telemetry = Telemetry::getInstance();
SerialTelemetryTransport serialTransport;

void setup() {
    // Disconnect from the Particle cloud, to avoid pauses
    Particle.disconnect();

    // Activate serial port (for debug printing)
    Serial.begin(115200);

    // Activate high-speed i2c
    Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();

    // Initialize the AHRS
    ahrs.initialize();

    // Set the LED to yellow
    RGB.control(true);
    RGB.color(255, 165, 0);
}

void loop() {
    // Update sensor data in the AHRS
    ahrs.update();

    // Get latest orientation data from AHRS
    Vector3 orientation = ahrs.getOrientation();

    // Send telemetry information
    serialTransport.sendTelemetry(telemetry);
}

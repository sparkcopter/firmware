#include <application.h>

#include "AHRS.h"
#include "NavdataTelemetryTransport.h"
#include "SerialTelemetryTransport.h"
#include "Telemetry.h"

// AHRS
AHRS ahrs;

// Telemetry transports
SerialTelemetryTransport serialTelemetry;
NavdataTelemetryTransport navdataTelemetry;

void setup() {
    // Activate serial port (for debug printing)
    Serial.begin(115200);

    // Activate i2c bus
    Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();

    // Initialize the AHRS
    ahrs.initialize();

    // Boot countdown
    for(int i=5; i>0; i--) {
      Serial.print(i);
      Serial.print("...");
      delay(1000);
    }
    Serial.println();

    navdataTelemetry.initialize();

    Serial.println(WiFi.localIP());
}

void loop() {
    // Update sensor data in the AHRS
    ahrs.update();

    // Get latest orientation data from AHRS
    Vector3 orientation = ahrs.getOrientation();

    // Send telemetry information
    Telemetry &telemetry = Telemetry::getInstance();
    serialTelemetry.send(telemetry);
    navdataTelemetry.send(telemetry);
}

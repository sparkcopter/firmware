#include <application.h>

#include "SerialTelemetryTransport.h"

void SerialTelemetryTransport::send(Telemetry &telemetry) {
    Serial.print("Acceleration: ");
    Serial.print(telemetry.acceleration.x);
    Serial.print(", ");
    Serial.print(telemetry.acceleration.y);
    Serial.print(", ");
    Serial.print(telemetry.acceleration.z);
    Serial.println();

    Serial.print("Rotation:     ");
    Serial.print(telemetry.rotation.x);
    Serial.print(", ");
    Serial.print(telemetry.rotation.y);
    Serial.print(", ");
    Serial.print(telemetry.rotation.z);
    Serial.println();

    Serial.print("Orientation:  ");
    Serial.print(telemetry.orientation.x);
    Serial.print(", ");
    Serial.print(telemetry.orientation.y);
    Serial.print(", ");
    Serial.print(telemetry.orientation.z);
    Serial.println();
}

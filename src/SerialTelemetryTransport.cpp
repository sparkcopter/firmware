#include "SerialTelemetryTransport.h"

void SerialTelemetryTransport::sendTelemetry(Telemetry *t) {
    Serial.printlnf("Acceleration: %f, %f, %f", t->acceleration.x, t->acceleration.y, t->acceleration.z);
    Serial.printlnf("Rotation:     %f, %f, %f", t->rotation.x, t->rotation.y, t->rotation.z);
    Serial.printlnf("Orientation:  %f, %f, %f", t->orientation.x, t->orientation.y, t->orientation.z);
}

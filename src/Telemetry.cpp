#include "Telemetry.h"

void Telemetry::setTransport(TelemetryTransport *transport) {
    this->transport = transport;
}

void Telemetry::send() {
  transport->sendTelemetry(this);
}

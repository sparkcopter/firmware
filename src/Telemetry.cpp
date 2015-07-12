#include "Telemetry.h"

void Telemetry::addTransport(TelemetryTransport *transport) {
    this->transports[numTransports++] = transport;
}

void Telemetry::send() {
    for(int i=0; i<numTransports; i++) {
        transports[i]->sendTelemetry(this);
    }
}

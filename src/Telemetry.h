#pragma once

#include "TelemetryTransport.h"
#include "Vector3.h"

#define TELEMETRY_MAX_TRANSPORTS 2

class Telemetry {
public:
    static Telemetry* getInstance() {
        static Telemetry instance;
        return &instance;
    }

    void addTransport(TelemetryTransport *transport);
    void send();

    Vector3 acceleration;
    Vector3 rotation;
    // Vector3 magneticField;
    Vector3 orientation;

private:
    TelemetryTransport *transports[TELEMETRY_MAX_TRANSPORTS];
    int numTransports = 0;
};

#pragma once

#include "TelemetryTransport.h"
#include "Vector3.h"

class Telemetry {
public:
    static Telemetry* getInstance() {
        static Telemetry instance;
        return &instance;
    }

    void setTransport(TelemetryTransport *transport);
    void send();

    Vector3 acceleration;
    Vector3 rotation;

private:
    TelemetryTransport *transport;
};

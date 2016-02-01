#pragma once

#include "Telemetry.h"

class TelemetryTransport {
public:
    virtual void send(Telemetry &telemetry) = 0;
};

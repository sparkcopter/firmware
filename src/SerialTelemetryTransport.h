#pragma once

#include "TelemetryTransport.h"

class SerialTelemetryTransport : public TelemetryTransport {
public:
    void send(Telemetry &telemetry);
};

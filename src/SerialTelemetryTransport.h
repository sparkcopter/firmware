#pragma once

#include "Telemetry.h"
#include "TelemetryTransport.h"

class SerialTelemetryTransport : public TelemetryTransport {
public:
    SerialTelemetryTransport();
    void sendTelemetry(Telemetry *telemetry);
};

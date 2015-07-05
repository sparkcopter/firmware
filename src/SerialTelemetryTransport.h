#pragma once

#include "Telemetry.h"
#include "TelemetryTransport.h"

class SerialTelemetryTransport : public TelemetryTransport {
public:
    void sendTelemetry(Telemetry *telemetry);
};

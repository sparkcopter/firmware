#pragma once

#include "Telemetry.h"

class Telemetry;
class TelemetryTransport {
public:
    virtual void sendTelemetry(Telemetry *telemetry) = 0;
};

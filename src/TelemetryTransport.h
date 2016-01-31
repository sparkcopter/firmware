#pragma once

class TelemetryTransport {
public:
    virtual void sendTelemetry(Telemetry *telemetry) = 0;
};

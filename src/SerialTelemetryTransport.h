#pragma once

#include <application.h>

#include "Telemetry.h"
#include "TelemetryTransport.h"

class SerialTelemetryTransport : public TelemetryTransport {
public:
    inline static SerialTelemetryTransport *getInstance() {
        static SerialTelemetryTransport instance;
        return &instance;
    }

    void sendTelemetry(Telemetry *t);
};

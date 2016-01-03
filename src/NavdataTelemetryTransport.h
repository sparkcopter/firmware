#pragma once

#include <application.h>

#include "NavData.h"
#include "Telemetry.h"
#include "TelemetryTransport.h"

#define NAVDATA_UDP_PORT 5554
#define NAVDATA_MAX_SIZE 4096
#define NAVDATA_HEADER  0x55667788
#define NAVDATA_FLUSH_FREQUENCY 30

class NavdataTelemetryTransport : public TelemetryTransport {
public:
    NavdataTelemetryTransport();
    void sendTelemetry(Telemetry *telemetry);

private:
    void writeSequenceNumber();
    void writeDemo(Telemetry *telemetry);
    void writeTime();
    void writeChecksum();
    void writeBuffer(void *buf, uint8_t size);
    void writeLongWord(int32_t longWord);
    void flush();

    UDP udp;

    IPAddress ipAddress;
    int port;
    bool clientConnected = false;
    int sequenceNumber = 0;

    uint8_t bufferSize = 0;
    uint8_t buffer[1024];
    int lastFlush = 0;
};

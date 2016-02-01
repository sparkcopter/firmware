#include "NavdataTelemetryTransport.h"

void NavdataTelemetryTransport::initialize() {
    udp.begin(NAVDATA_UDP_PORT);
}

void NavdataTelemetryTransport::send(Telemetry &telemetry) {
    // Check if we got a navdata "wake-up" packet from a client
    if(udp.parsePacket()) {
        // Flush the "wake-up" packet through
        // NOTE: Always read/flush UDP packets before writing back
        while(udp.available()) udp.read();
        udp.flush();

        // Store the navdata client's details
        ipAddress = udp.remoteIP();
        port = udp.remotePort();

        // New client connected
        clientConnected = true;
    }

    if(clientConnected) {
        // Send telemetry if necessary
        if(lastFlush + NAVDATA_FLUSH_FREQUENCY < millis()) {
            // Start the navdata payload
            udp.beginPacket(ipAddress, port);

            // Write the header (32-bit)
            writeLongWord(NAVDATA_HEADER);

            // Write the drone state (32-bit bit mask)
            writeLongWord(0); // TODO

            // Write the sequence number
            writeSequenceNumber();

            // Write the vision flag (not used)
            writeLongWord(0);

            // Write some options data
            writeDemo(telemetry);
            writeTime();

            // Write the checksum
            writeChecksum();

            // Write the buffer
            flush();
        }
    }
}

void NavdataTelemetryTransport::writeSequenceNumber() {
    writeLongWord(sequenceNumber);
}

void NavdataTelemetryTransport::writeDemo(Telemetry &telemetry) {
    navdata_demo_t payload;

    memset(&payload, 0, sizeof(navdata_demo_t));

    payload.tag = NAVDATA_DEMO_TAG;
    payload.size = sizeof(navdata_demo_t);

    payload.theta = telemetry.orientation.pitch * 1000.0;
    payload.phi = telemetry.orientation.roll * 1000.0;
    payload.psi = telemetry.orientation.yaw * 1000.0;

    // TODO: Anything else

    writeBuffer(&payload, payload.size);
}

void NavdataTelemetryTransport::writeTime() {
    // 11 MSB = seconds, 21 LSB = microseconds
    uint32_t elapsedTime = micros();
    uint32_t convertedTime = ((int)floor(elapsedTime/1000000) << 21) | (elapsedTime % 1000000);

    navdata_time_t payload;
    payload.tag = NAVDATA_TIME_TAG;
    payload.size = sizeof(navdata_time_t);
    payload.time = convertedTime;

    writeBuffer(&payload, payload.size);
}

void NavdataTelemetryTransport::writeChecksum() {
    int checksum = 0;
    for(int i=0; i<bufferSize; i++) {
        checksum += buffer[i];
    }

    navdata_cks_t payload;
    payload.tag = NAVDATA_CKS_TAG;
    payload.size = sizeof(navdata_cks_t);
    payload.cks = checksum;

    writeBuffer(&payload, sizeof(navdata_cks_t));
}

void NavdataTelemetryTransport::writeLongWord(int32_t longWord) {
    writeBuffer(&longWord, sizeof(int32_t));
}

void NavdataTelemetryTransport::writeBuffer(void *buf, uint8_t size) {
    memcpy(buffer + bufferSize, buf, size);
    bufferSize += size;
}

void NavdataTelemetryTransport::flush() {
    udp.write(buffer, bufferSize);
    udp.endPacket();
    bufferSize = 0;
    sequenceNumber++;
    lastFlush = millis();
}

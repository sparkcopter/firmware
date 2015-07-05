#include "NavdataTelemetryTransport.h"

NavdataTelemetryTransport::NavdataTelemetryTransport() {
    udp.begin(NAVDATA_UDP_PORT);
}

void NavdataTelemetryTransport::sendTelemetry(Telemetry *telemetry) {
    // Check if we got a navdata "wake-up" packet from a client
    if(udp.parsePacket()) {
        Serial.println("SOMEONE CONNECTED ON UDP");

        // Flush the "wake-up" packet through
        while(udp.available()) udp.read();
        udp.flush();

        // Store the navdata client's details
        ipAddress = udp.remoteIP();
        port = udp.remotePort();

        // New client connected
        clientConnected = true;
    }

    // Send telemetry if necessary
    if(clientConnected && lastFlush + NAVDATA_FLUSH_FREQUENCY < millis()) {
        // Start the navdata payload
        udp.beginPacket(ipAddress, port);

        // Write the header (32-bit)
        writeHeader();

        // Write the drone state (32-bit bit mask)
        writeLongWord(0); // TODO

        // Write the sequence number
        writeSequenceNumber();

        // Write the vision flag (not used)
        writeLongWord(0);

        // Write some options data
        writeDemo();
        writeTime();

        // Write the checksum
        writeChecksum();

        // Write the buffer
        flush();
    }
}

void NavdataTelemetryTransport::writeHeader() {
    writeLongWord(NAVDATA_HEADER);
}

void NavdataTelemetryTransport::writeSequenceNumber() {
    writeLongWord(sequenceNumber);
}

void NavdataTelemetryTransport::writeDemo() {
    navdata_demo_t payload;

    memset(&payload, 0, sizeof(navdata_demo_t));

    payload.tag = NAVDATA_DEMO_TAG;
    payload.size = sizeof(navdata_demo_t);

    payload.theta = 45*1000.0;//orientation.y * 1000.0;
    payload.phi = 45*1000.0;//orientation.x * 1000.0;
    payload.psi = 45*1000.0;//orientation.z * 1000.0;

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

void NavdataTelemetryTransport::writeBuffer(void *buf, uint8_t size) {
    memcpy(buffer + bufferSize, buf, size);
    bufferSize += size;
}

void NavdataTelemetryTransport::writeLongWord(int32_t longWord) {
    writeBuffer(&longWord, sizeof(int32_t));
}

void NavdataTelemetryTransport::flush() {
    udp.write(buffer, bufferSize);
    udp.endPacket();
    bufferSize = 0;
    sequenceNumber++;
    lastFlush = millis();
}

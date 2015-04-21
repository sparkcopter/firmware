#include "math.h"

#include "Logger.h"
#include "NavData.h"

NavData::NavData() {

}

void NavData::init() {
    udp.begin(NAVDATA_UDP_PORT);
}

void NavData::checkForClient() {
    // Check if we got a navdata "wake-up" packet from a client
    if(!udp.parsePacket()) return;

    // Flush the "wake-up" packet through
    while(udp.available()) udp.read();
    udp.flush();

    // Store the navdata client's details
    ipAddress = udp.remoteIP();
    port = udp.remotePort();

    // New client connected
    clientConnected = true;
}

void NavData::send() {
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
        // writeRawMeasures();

        // Write the checksum
        writeChecksum();

        // Write the buffer
        flush();
    }
}

void NavData::updateOrientation(Vector3 orientation) {
    this->orientation = orientation;
}

void NavData::writeHeader() {
    writeLongWord(NAVDATA_HEADER);
}

void NavData::writeSequenceNumber() {
    writeLongWord(sequenceNumber);
}

void NavData::writeDemo() {
    navdata_demo_t payload;

    memset(&payload, 0, sizeof(navdata_demo_t));

    payload.tag = NAVDATA_DEMO_TAG;
    payload.size = sizeof(navdata_demo_t);

    payload.theta = orientation.y * 1000.0;
    payload.phi = orientation.x * 1000.0;
    payload.psi = orientation.z * 1000.0;

    // TODO: Anything else

    writeBuffer(&payload, sizeof(navdata_demo_t));
}

void NavData::writeTime() {
    // 11 MSB = seconds, 21 LSB = microseconds
    uint32_t elapsedTime = micros();
    uint32_t convertedTime = ((int)floor(elapsedTime/1000000) << 21) | (elapsedTime % 1000000);

    navdata_time_t payload;
    payload.tag = NAVDATA_TIME_TAG;
    payload.size = sizeof(navdata_time_t);
    payload.time = convertedTime;

    writeBuffer(&payload, sizeof(navdata_time_t));
}

void NavData::writeRawMeasures() {
    navdata_raw_measures_t payload;

    memset(&payload, 0, sizeof(navdata_raw_measures_t));

    payload.tag = NAVDATA_RAW_MEASURES_TAG;
    payload.size = sizeof(navdata_raw_measures_t);

    // TODO

    writeBuffer(&payload, sizeof(navdata_raw_measures_t));
}

void NavData::writeChecksum() {
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

void NavData::writeBuffer(void *buf, uint8_t size) {
    memcpy(buffer + bufferSize, buf, size);
    bufferSize += size;
}

void NavData::writeLongWord(int32_t longWord) {
    writeBuffer(&longWord, sizeof(int32_t));
}

void NavData::flush() {
    udp.write(buffer, bufferSize);
    udp.endPacket();
    bufferSize = 0;
    sequenceNumber++;
    lastFlush = millis();
}

#pragma once

#include <spark_wiring_udp.h>

#define _ATTRIBUTE_PACKED_  __attribute__ ((packed))

#define NAVDATA_UDP_PORT 5554

#define NAVDATA_MAX_SIZE 4096
#define NAVDATA_HEADER  0x55667788
#define NAVDATA_FLUSH_FREQUENCY 30

class NavData {
public:
    NavData();

    void init();
    void checkForClient();
    void send();

private:
    enum NavDataTag {
        NAVDATA_DEMO_TAG = 0,
        NAVDATA_TIME_TAG,
        NAVDATA_RAW_MEASURES_TAG,
        NAVDATA_PHYS_MEASURES_TAG,
        NAVDATA_GYROS_OFFSETS_TAG,
        NAVDATA_EULER_ANGLES_TAG,
        NAVDATA_REFERENCES_TAG,
        NAVDATA_TRIMS_TAG,
        NAVDATA_RC_REFERENCES_TAG,
        NAVDATA_PWM_TAG,
        NAVDATA_ALTITUDE_TAG,
        NAVDATA_VISION_RAW_TAG,
        NAVDATA_VISION_OF_TAG,
        NAVDATA_VISION_TAG,
        NAVDATA_VISION_PERF_TAG,
        NAVDATA_TRACKERS_SEND_TAG,
        NAVDATA_VISION_DETECT_TAG,
        NAVDATA_WATCHDOG_TAG,
        NAVDATA_ADC_DATA_FRAME_TAG,
        NAVDATA_VIDEO_STREAM_TAG,
        NAVDATA_CKS_TAG = 0xFFFF
    };

    void writeHeader();
    void writeSequenceNumber();
    void writeChecksum();
    void writeTime();
    void writeDemo();
    void writeBuffer(void *buf, uint8_t size);
    void writeLongWord(int32_t longWord);
    void flush();

    UDP udp = UDP();

    IPAddress ipAddress;
    int port;
    bool clientConnected = false;
    int sequenceNumber = 0;

    uint8_t bufferSize = 0;
    uint8_t buffer[1024];
    int lastFlush = 0;

    // Demo option
    typedef struct {
      uint16_t tag;
      uint16_t size;

      uint32_t ctrl_state;             // Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum.
      uint32_t vbat_flying_percentage; // Battery voltage

      float theta;                     // Pitch in milli-degrees
      float phi;                       // Roll in milli-degrees
      float psi;                       // Yaw in mill-degrees

      int32_t altitude;                // Altitude in CM

      float   vx;                      // Estimated linear x-axis speed
      float   vy;                      // Estimated linear y-axis speed
      float   vz;                      // Estimated linear z-axis speed

      int32_t unused[27];
    }_ATTRIBUTE_PACKED_ navdata_demo_t;

    // Time option
    typedef struct {
      uint16_t  tag;
      uint16_t  size;

      uint32_t  time;                  // 11 MSB = seconds, 21 LSB = microseconds
    }_ATTRIBUTE_PACKED_ navdata_time_t;

    // Checksum option
    typedef struct {
      uint16_t  tag;
      uint16_t  size;

      uint32_t  cks;                   // Integer sum of all bytes in buffer
    }_ATTRIBUTE_PACKED_ navdata_cks_t;
};

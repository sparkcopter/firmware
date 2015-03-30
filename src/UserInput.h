#pragma once

#include <spark_wiring_udp.h>

#define USER_INPUT_UDP_BUFFER_SIZE  1024
#define USER_INPUT_UDP_NAVDATA_PORT 5554
#define USER_INPUT_UDP_VIDEO_PORT   5556
#define USER_INPUT_UDP_AT_PORT      5556
#define USER_INPUT_UDP_CONTROL_PORT 5559

class UserInput {
public:
    UserInput();

    void init();
    void read();

    void (*ref)(bool, bool);
    void (*pcmd)(bool, bool, float, float, float, float);
    void (*ftrim)(void);

private:
    UDP *udp = new UDP();

    char buffer[USER_INPUT_UDP_BUFFER_SIZE];
    int lastSequenceNumber;

    bool validSequenceNumber(int sequenceNumber);
    bool commandMatches(const char *commandName, char *command);
    void parseCommand(char *command);

    void parseRef(char *args);
    void parsePcmd(char *args);
    void parseFtrim(char *args);
    void parseConfig(char *args);
    void parseCtrl(char *args);
    void parseLed(char *args);
    void parseAnim(char *args);

    typedef union _float_or_int_t {
        float f;
        int   i;
    } float_or_int_t;
};

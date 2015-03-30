#pragma once

#include <spark_wiring_udp.h>

#define UDP_BUFFER_SIZE 1024

class UserInput {
public:
    UserInput(int port);

    void init();
    void read();

    void (*ref)(bool, bool);
    void (*pcmd)(bool, bool, float, float, float, float);
    void (*ftrim)(void);

private:
    UDP *udp = new UDP();

    int port;
    char buffer[UDP_BUFFER_SIZE];
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

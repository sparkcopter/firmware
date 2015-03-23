#include "application.h"

#define UDP_BUFFER_SIZE 1024

class UserInput {
public:
    UserInput(int port);

    void init();
    void read();

    void (*takeoff)(void);
    void (*land)(void);
    void (*move)(float, float, float, float);

private:
    UDP *udp = new UDP();
    int port;
    char buffer[UDP_BUFFER_SIZE];
    int lastSequenceNumber;

    bool validSequenceNumber(int sequenceNumber);
    void handleRef(char *args);
    void handlePcmd(char *args);
    void handleFtrim();
    void handleConfig(char *args);
    void handleLed(char *args);
    void handleAnim(char *args);
};

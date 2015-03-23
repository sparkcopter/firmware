#include "application.h"

#define UDP_BUFFER_SIZE 1024

class UserInput {
public:
    UserInput(int port);

    void init();
    void read();

private:
    UDP *udp = new UDP();
    int port;
    char buffer[UDP_BUFFER_SIZE];
};

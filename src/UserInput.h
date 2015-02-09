#include "application.h"

class UserInput {
public:
    UserInput(int port);

private:
    TCPServer *server;
    TCPClient *client;
};

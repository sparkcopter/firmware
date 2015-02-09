#include "UserInput.h"

UserInput::UserInput(int port) {
    server = new TCPServer(port);
}

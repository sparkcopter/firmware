#include "Logger.h"
#include "UserInput.h"

typedef union _float_or_int_t {
    float f;
    int   i;
} float_or_int_t;

UserInput::UserInput(int port) {
    this->port = port;
}

void UserInput::init() {
    udp->begin(port);
}

void UserInput::read() {
    if(udp->parsePacket()) {
        // Read UDP packet
        int bytesRead = 0;
        while(udp->available()) {
            buffer[bytesRead++] = udp->read();
        }
        buffer[bytesRead] = '0';

        // Split packet into AT commands
        char *commandTokens;
        char *command = strtok_r(buffer, "\r\n", &commandTokens);
        while(command)  {
            // Parse AT command
            if(strncmp("AT*", command, 3) == 0) {
                char *commandStart = command + 3;
                if(strncmp("REF=", commandStart, 4) == 0) {
                    handleRef(commandStart + 4);
                } else if(strncmp("PCMD=", commandStart, 5) == 0) {
                    handlePcmd(commandStart + 5);
                } else if(strncmp("FTRIM=", commandStart, 6) == 0) {
                    handleFtrim();
                } else if(strncmp("CONFIG=", commandStart, 7) == 0) {
                    handleConfig(commandStart + 7);
                } else if(strncmp("LED=", commandStart, 4) == 0) {
                    handleLed(commandStart + 4);
                } else if(strncmp("ANIM=", commandStart, 5) == 0) {
                    handleAnim(commandStart + 5);
                } else {
                    Logger::log("Received unknown AT command: %s", commandStart);
                }
            }

            // Get the next AT* command (if any)
            command = strtok_r(NULL, "\r\n", &commandTokens);
        }

        udp->flush();
    }
}

bool UserInput::validSequenceNumber(int sequenceNumber) {
    // TODO: Actually verify
    return true;
}

// AT*REF commands (Takeoff/Landing/Emergency stop command)
void UserInput::handleRef(char *args) {
    // Parse arguments
    char *argTokens;

    // Extract UDP sequence number
    int sequenceNumber = atoi(strtok_r(args, ",", &argTokens));
    if(!validSequenceNumber(sequenceNumber)) {
        return;
    }

    // Extract REF commands
    int controlField = atoi(strtok_r(NULL, ",", &argTokens));
    int takeoffBit = (controlField >> 9) & 1;
    int emergencyBit = (controlField >> 8) & 1;

    // Run callback
    takeoffBit ? takeoff() : land();
}

// AT*PCMD commands (Move the drone)
void UserInput::handlePcmd(char *args) {
    // Parse arguments
    char *argTokens;

    // Extract UDP sequence number
    int sequenceNumber = atoi(strtok_r(args, ",", &argTokens));
    if(!validSequenceNumber(sequenceNumber)) {
        return;
    }

    // Extract movement mode (currently unused)
    int progressive = (int)atoi(strtok_r(NULL, ",", &argTokens));

    // Extract movement commands (floats stored as ints)
    float_or_int_t leftTilt, frontTilt, verticalSpeed, angularSpeed;
    leftTilt.i = atoi(strtok_r(NULL, ",", &argTokens));
    frontTilt.i = atoi(strtok_r(NULL, ",", &argTokens));
    verticalSpeed.i = atoi(strtok_r(NULL, ",", &argTokens));
    angularSpeed.i = atoi(strtok_r(NULL, ",", &argTokens));

    // Run callback
    move(leftTilt.f, frontTilt.f, verticalSpeed.f, angularSpeed.f);
}

// AT*FTRIM commands (Sets the reference for the horizontal plane)
void UserInput::handleFtrim() {
    // TODO
}

// AT*CONFIG commands (Sets config variables)
void UserInput::handleConfig(char *args) {
    // TODO
}

// AT*LED (Set a led animation)
void UserInput::handleLed(char *args) {
    // TODO
}

// AT*ANIM (Set a flight animation)
void UserInput::handleAnim(char *args) {
    // TODO
}

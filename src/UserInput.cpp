#include "Logger.h"
#include "UserInput.h"

UserInput::UserInput(int port) {
    this->port = port;
}

void UserInput::init() {
    udp->begin(port);
}

void UserInput::read() {
    // Check for UDP packets
    if(!udp->parsePacket()) return;

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
            parseCommand(command + 3);
        }

        // Get the next AT* command (if any)
        command = strtok_r(NULL, "\r\n", &commandTokens);
    }

    udp->flush();
}

void UserInput::parseCommand(char *command) {
    char *argsStart = strchr(command, '=') + 1;
    char *args;

    // Check valid sequence number
    int sequenceNumber = atoi(strtok_r(argsStart, ",", &args));
    if(!validSequenceNumber(sequenceNumber)) return;

    // Run the appropriate command
    if(commandMatches("REF", command)) {
        parseRef(args);
    } else if(commandMatches("PCMD", command)) {
        parsePcmd(args);
    } else if(commandMatches("FTRIM", command)) {
        parseFtrim(args);
    } else if(commandMatches("CONFIG", command)) {
        parseConfig(args);
    } else if(commandMatches("CTRL", command)) {
        parseCtrl(args);
    } else if(commandMatches("LED", command)) {
        parseLed(args);
    } else if(commandMatches("ANIM", command)) {
        parseAnim(args);
    } else {
        Logger::warn("Received unknown AT command: %s", command);
    }
}

bool UserInput::validSequenceNumber(int sequenceNumber) {
    // TODO: Actually verify
    return true;
}

bool UserInput::commandMatches(const char *commandName, char *command) {
    return strncmp(commandName, command, strlen(commandName)) == 0;
}

// AT*REF commands (Takeoff/Landing/Emergency stop command)
void UserInput::parseRef(char *args) {
    // Extract REF commands
    int controlField = atoi(strtok_r(NULL, ",", &args));
    bool emergencyBit = (controlField >> 8) & 1;
    bool takeoffBit = (controlField >> 9) & 1;

    // Run callback
    ref(emergencyBit, takeoffBit);
}

// AT*PCMD commands (Move the drone)
void UserInput::parsePcmd(char *args) {
    // Extract movement mode
    int movementModeField = (int)atoi(strtok_r(NULL, ",", &args));
    bool progressive = (movementModeField >> 0) & 1;
    bool combinedYaw = (movementModeField >> 1) & 1;

    // Extract movement commands (floats stored as ints)
    float_or_int_t leftTilt, frontTilt, verticalSpeed, angularSpeed;
    leftTilt.i = atoi(strtok_r(NULL, ",", &args));
    frontTilt.i = atoi(strtok_r(NULL, ",", &args));
    verticalSpeed.i = atoi(strtok_r(NULL, ",", &args));
    angularSpeed.i = atoi(strtok_r(NULL, ",", &args));

    // Run callback
    pcmd(progressive, combinedYaw, leftTilt.f, frontTilt.f, verticalSpeed.f, angularSpeed.f);
}

// AT*FTRIM commands (Sets the reference for the horizontal plane)
void UserInput::parseFtrim(char *args) {
    ftrim();
}

// AT*CONFIG commands (Sets config variables)
void UserInput::parseConfig(char *args) {
    // TODO
}

// AT*CTRL commands (Sets config variables)
void UserInput::parseCtrl(char *args) {
    int ctrlAction = (int)atoi(strtok_r(NULL, ",", &args));
    if(ctrlAction == 4) {
        // TODO: Dump configuration
    } else if(ctrlAction == 5) {
        // TODO: Reset CONFIG ACK state
    }
}

// AT*LED (Set a led animation)
void UserInput::parseLed(char *args) {
    // TODO
}

// AT*ANIM (Set a flight animation)
void UserInput::parseAnim(char *args) {
    // TODO
}

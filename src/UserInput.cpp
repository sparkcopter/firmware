#include "Logger.h"
#include "UserInput.h"

UserInput::UserInput(int port) {
    this->port = port;
}

void UserInput::init() {
    // Wait for WiFi to connect
    while (!WiFi.ready()) {}

    // Open UDP port
    udp->begin(port);

    // Turn on RED LED
    RGB.control(true);
    RGB.color(255, 0, 0);
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
                Serial.print("Valid AT* command: ");
                Serial.println(command);

                char *commandStart = command + 3;
                if(strncmp("REF=", commandStart, 4) == 0) {
                    // Parse arguments
                    char *argTokens;
                    int sequenceNumber = atoi(strtok_r(command+7, ",", &argTokens));
                    int controlField = atoi(strtok_r(NULL, ",", &argTokens));

                    // Extract REF commands
                    int takeoff = (controlField >> 9) & 1;
                    int emergency = (controlField >> 8) & 1;

                    // Do something (temporary)
                    if(takeoff) {
                        RGB.color(0, 255, 0);
                    } else {
                        RGB.color(255, 0, 0);
                    }
                } else if(strncmp("PCMD=", commandStart, 5) == 0) {
                    // TODO
                }
            }

            // Get the next AT* command (if any)
            command = strtok_r(NULL, "\r\n", &commandTokens);
        }

        udp->flush();
    }
}

// void setCallback(char *command, void *function) {}
// setCallback("takeoff", &takeoff);
// setCallback("progress", &progress)

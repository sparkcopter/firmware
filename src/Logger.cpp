#include "Logger.h"

void Logger::init() {
    Serial.begin(LOGGER_SERIAL_SPEED);
}

void Logger::log(const char* format, ...) {
    char buffer[LOGGER_BUFFER_LENGTH];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, LOGGER_BUFFER_LENGTH, format, args);
    va_end(args);

    Serial.println(buffer);
    Serial.flush();
}

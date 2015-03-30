#include "Logger.h"

void Logger::init() {
    Serial.begin(LOGGER_SERIAL_SPEED);
}

void Logger::error(const char* format, ...) {
    char buffer[LOGGER_BUFFER_LENGTH];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, LOGGER_BUFFER_LENGTH, format, args);
    va_end(args);

    Serial.print(LOGGER_COLOR_RED);
    Serial.print(buffer);
    Serial.println(LOGGER_COLOR_RESET);
    Serial.flush();
}

void Logger::warn(const char* format, ...) {
    char buffer[LOGGER_BUFFER_LENGTH];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, LOGGER_BUFFER_LENGTH, format, args);
    va_end(args);

    Serial.print(LOGGER_COLOR_YELLOW);
    Serial.print(buffer);
    Serial.println(LOGGER_COLOR_RESET);
    Serial.flush();
}

void Logger::debug(const char* format, ...) {
    char buffer[LOGGER_BUFFER_LENGTH];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, LOGGER_BUFFER_LENGTH, format, args);
    va_end(args);

    Serial.print(LOGGER_COLOR_GRAY);
    Serial.print(buffer);
    Serial.println(LOGGER_COLOR_RESET);
    Serial.flush();
}

void Logger::info(const char* format, ...) {
    char buffer[LOGGER_BUFFER_LENGTH];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, LOGGER_BUFFER_LENGTH, format, args);
    va_end(args);

    Serial.println(buffer);
    Serial.flush();
}

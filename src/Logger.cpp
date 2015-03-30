#include "Logger.h"

void Logger::init() {
    Serial.begin(LOGGER_SERIAL_SPEED);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    Serial.print(LOGGER_COLOR_RED);
    log(format, args);
    Serial.print(LOGGER_COLOR_RESET);
    va_end(args);
}

void Logger::warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    Serial.print(LOGGER_COLOR_YELLOW);
    log(format, args);
    Serial.print(LOGGER_COLOR_RESET);
    va_end(args);
}

void Logger::debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    Serial.print(LOGGER_COLOR_GRAY);
    log(format, args);
    Serial.print(LOGGER_COLOR_RESET);
    va_end(args);
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(format, args);
    va_end(args);
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

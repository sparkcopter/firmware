#pragma once

#include <stdarg.h>
#include <spark_wiring_usbserial.h>

#define LOGGER_SERIAL_SPEED     115200
#define LOGGER_BUFFER_LENGTH    255

#define LOGGER_COLOR_RED        "\033[31m"
#define LOGGER_COLOR_GREEN      "\033[32m"
#define LOGGER_COLOR_YELLOW     "\033[33m"
#define LOGGER_COLOR_BLUE       "\033[34m"
#define LOGGER_COLOR_MAGENTA    "\033[35m"
#define LOGGER_COLOR_CYAN       "\033[36m"
#define LOGGER_COLOR_GRAY       "\033[37m"
#define LOGGER_COLOR_RESET      "\033[0m"

class Logger {
public:
    static void init();

    static void error(const char* format, ...);
    static void warn(const char* format, ...);
    static void info(const char* format, ...);
    static void debug(const char* format, ...);

private:
    static void log(const char* format, ...);
};

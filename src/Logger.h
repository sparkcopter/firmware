#include <stdarg.h>

#include "application.h"

#define LOGGER_SERIAL_SPEED 115200
#define LOGGER_BUFFER_LENGTH 255

class Logger {
public:
    static void init();
    static void log(const char* format, ...);
};

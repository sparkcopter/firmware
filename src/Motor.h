#include "application.h"

class Motor {
public:
    // Initialize a motor on the given pin
    Motor(uint16_t pin);

    // Set the speed of the motor (0 = off, 255 = full speed)
    void setSpeed(uint8_t speed);

    // Stop the motor
    void stop();

private:
    uint16_t pin;
};

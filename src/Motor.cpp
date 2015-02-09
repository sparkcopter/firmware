#include "Motor.h"

Motor::Motor(uint16_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Motor::setSpeed(uint8_t speed) {
    analogWrite(this->pin, speed);
}

void Motor::stop() {
    this->setSpeed(0);
}

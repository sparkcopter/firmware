#pragma once

#include <spark_wiring_i2c.h>

class I2C {
public:
    // Constructor
    I2C(uint8_t address);

    int8_t readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
    int8_t readByte(uint8_t regAddr, uint8_t *data);

    int8_t readWords(uint8_t regAddr, uint8_t length, uint16_t *data);
    int8_t readWord(uint8_t regAddr, uint16_t *data);

    bool writeBits(uint8_t regAddr, uint8_t start, uint8_t length, uint8_t data);
    bool writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data);

    bool writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
    bool writeByte(uint8_t regAddr, uint8_t data);

    bool writeWords(uint8_t regAddr, uint8_t length, uint16_t* data);
    bool writeWord(uint8_t regAddr, uint16_t data);

private:
    uint8_t address;
};

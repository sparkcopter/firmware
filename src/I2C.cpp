#include "I2C.h"

I2C::I2C(uint8_t address) {
    this->address = address;
}

int8_t I2C::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    int8_t count = 0;

    // Request n bytes of register data from the i2c bus
    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.endTransmission(false);
    Wire.beginTransmission(regAddr);
    Wire.requestFrom(this->address, length);

    // Read requested registers into a buffer
    for(; Wire.available(); count++) {
        data[count] = Wire.read();
    }

    return count;
}

int8_t I2C::readByte(uint8_t regAddr, uint8_t *data) {
    return readBytes(regAddr, 1, data);
}

int8_t I2C::readWords(uint8_t regAddr, uint8_t length, uint16_t *data) {
    int8_t count = 0;

    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.endTransmission();
    Wire.beginTransmission(this->address);
    Wire.requestFrom(this->address, length * 2);

    bool msb = true; // starts with MSB, then LSB
    for (; Wire.available() && count < length;) {
        if (msb) {
            data[count] = Wire.read() << 8;
        } else {
            data[count++] |= Wire.read();
        }
        msb = !msb;
    }

    Wire.endTransmission();

    return count;
}

int8_t I2C::readWord(uint8_t regAddr, uint16_t *data) {
    return readWords(regAddr, 1, data);
}

bool I2C::writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    if(readByte(regAddr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(regAddr, b);
    } else {
        return false;
    }
}

bool I2C::writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    readByte(regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(regAddr, b);
}

bool I2C::writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    uint8_t status = 0;

    Wire.beginTransmission(this->address);
    Wire.write(regAddr);
    Wire.write(data, length);
    status = Wire.endTransmission();

    return status == 0;
}

bool I2C::writeByte(uint8_t regAddr, uint8_t data) {
    return writeBytes(regAddr, 1, &data);
}

bool I2C::writeWords(uint8_t regAddr, uint8_t length, uint16_t* data) {
    uint8_t status = 0;

    Wire.beginTransmission(address);
    Wire.write(regAddr);

    for(uint8_t i = 0; i < length * 2; i++) {
        Wire.write((uint8_t)(data[i] >> 8));    // send MSB
        Wire.write((uint8_t)data[i++]);         // send LSB
    }

    status = Wire.endTransmission();

    return status == 0;
}

bool I2C::writeWord(uint8_t regAddr, uint16_t data) {
    writeWords(regAddr, 1, &data);
}

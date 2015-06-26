#pragma once

#include "Vector3.h"

class Gyroscope {
public:
    Gyroscope();

    // Is there an gyroscope sensor available?
    bool isAvailable();

    // Get the current rotation rate in degrees per second
    Vector3 getRotation();
};

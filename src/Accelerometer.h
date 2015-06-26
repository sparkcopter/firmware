#pragma once

#include "Vector3.h"

class Accelerometer {
public:
    Accelerometer();

    // Is there an accelerometer sensor available?
    bool isAvailable();

    // Get the current acceleration in Gs
    Vector3 getAcceleration();
};

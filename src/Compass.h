#pragma once

#include "Vector3.h"

class Compass {
public:
    // Get the current heading vector, in milliGauss
    virtual Vector3 getHeading() = 0;
};

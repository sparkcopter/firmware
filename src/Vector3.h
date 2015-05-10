#pragma once

#include "math.h"

#define DEGREES_TO_RADIANS(angle) ((angle) * (M_PI / 180.0))
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))

class Vector3 {
public:
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};

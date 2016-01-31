#pragma once

#include "Vector3.h"

class Telemetry {
public:
    static Telemetry *getInstance() {
        static Telemetry instance;
        return &instance;
    }

    Vector3 acceleration;
    Vector3 rotation;
    Vector3 magneticField;
    Vector3 orientation;
};

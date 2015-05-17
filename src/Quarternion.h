#pragma once

#include "math.h"

#include "Vector3.h"

class Quarternion {
public:
    double w = 1.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    double norm() {
        return sqrt(w*w + x*x + y*y + z*z);
    }

    void normalize() {
        double n = norm();

        if(n > 0) {
            w /= n;
            x /= n;
            y /= n;
            z /= n;
        }
    }

    Vector3 toEuler() {
        Vector3 orientation;

        orientation.x = atan2(2.0f * (w*x + y*z), w*w - x*x - y*y + z*z) * RAD_TO_DEG;
        orientation.y = -asin(2.0f * (x*z - w*y)) * RAD_TO_DEG;
        orientation.z = atan2(2.0f * (x*y + w*z), w*w + x*x - y*y - z*z) * RAD_TO_DEG;

        return orientation;
    }
};

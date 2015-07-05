#pragma once

#include "Accelerometer.h"
#include "Barometer.h"
#include "Compass.h"
#include "Gyroscope.h"

class Sensors {
public:
    static void initialize();

    static bool barometerAvailable();
    static bool compassAvailable();

    static Accelerometer *getAccelerometer();
    static Barometer *getBarometer();
    static Compass *getCompass();
    static Gyroscope *getGyroscope();
};

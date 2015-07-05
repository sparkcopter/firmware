#pragma once

#include "Vector3.h"

class Aircraft {
    //
    // SensorDevices - provide sensors
    //
    //
    // Sensors - sensors help the aircraft fly level and steady
    //    Accelerometer
    //        - getAcceleration
    //    Gyroscope
    //        - getRotation
    //    Magnetometer
    //        - getHeading
    //    Barometer
    //        - getAltitude
    //        - getPressure
    //
    // Control - flight control (up, down, left, right, etc)
    //    ATControl - Parrot AR "AT" control protocol
    //
    // Telemetry - streamed measurement data
    //    NavDataTelemetry - Parrot AR "NavData" telemetry protocol
    //    SerialTelemetry - telemetry data over usb/serial
    //
    //    - attitude (roll, pitch - in degrees)
    //    - heading (yaw - in degrees)
    //    - altitude (height - in meters)
    //
    // Motors - applying power to the motors moves the aircraft
    //    - setPower
    //

    //
    // - Read desired position/heading from Control
    // - Read current position/heading from Sensors
    // - Use PID controllers to calculate which motor adjustments to make
    // - Adjust motor power accordingly
    // - Update and send Telemetry
    //
};

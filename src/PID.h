#pragma once

#include "application.h"

#define PID_DIRECTION_DIRECT    0
#define PID_DIRECTION_REVERSE   1

#define PID_DERIVATIVE_SOURCE_ERROR 0
#define PID_DERIVATIVE_SOURCE_PV    1

class PID {
public:
    // Create a PID controller
    PID(double kp, double ki, double kd, int direction);

    // Perform a new PID calculation with the given process variable
    double update(double processVariable);

    // Set the setpoint (target value) for the system
    void setSetpoint(double setPoint);

    // Set the Kp, Ki and Kd tunings
    // http://en.wikipedia.org/wiki/PID_controller#Manual_tuning
    void setTunings(double kp, double ki, double kd);

    // Set direction of the controller
    // - PID_DIRECTION_DIRECT  = increase output with positive error
    // - PID_DIRECTION_REVERSE = increase output with negative error
    void setDirection(int direction);

    // Set the sample time (in milliseconds) for PID calculation
    void setSampleTime(int sampleTime);

    // Restrict the output range of the controller (default 0 - 255)
    void setOutputLimits(double outMin, double outMax);

private:
    // PID tuning parameters
    double kp, ki, kd;

    // Controller direction
    int direction = PID_DIRECTION_DIRECT;

    // The variable to use for the derivative term
    int derivativeSource = PID_DERIVATIVE_SOURCE_PV;

    // Time between samples (in milliseconds)
    int sampleTime = 30;

    // Output limits
    double outMin = 0, outMax = 255;

    // Setpoint
    double setPoint = 0.0;

    // Current calculated output of the system
    double output = 0.0;

    // Last update time, last input
    unsigned long lastTime;
    double lastProcessVariable = 0.0;
    double lastError = 0.0;

    // Accumulated integral term
    double integralTerm = 0.0;
};

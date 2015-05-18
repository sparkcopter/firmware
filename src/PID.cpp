#include "PID.h"
#include "Logger.h"

PID::PID(double kp, double ki, double kd, int direction) {
    setTunings(kp, ki, kd);
    setDirection(direction);

    lastTime = millis() - sampleTime;
}

double PID::update(double processVariable) {
    unsigned long now = millis();
    unsigned long deltaMillis = now - lastTime;

    if(deltaMillis >= sampleTime) {
        double deltaSeconds = ((double)deltaMillis)/1000.0;

        // Compute the current error
        double error = setPoint - processVariable;

        // Compute the proportional term
        // http://en.wikipedia.org/wiki/PID_controller#Proportional_term
        double proportionalTerm = kp * error;

        // Accumulate the "integral" term
        // http://en.wikipedia.org/wiki/PID_controller#Integral_term
        integralTerm += ki * deltaSeconds * error;

        // Ensure "integral" term is within output range to avoid integral windup:
        // http://en.wikipedia.org/wiki/PID_controller#Integral_windup
        if(integralTerm > outMax) {
            integralTerm = outMax;
        } else if(integralTerm < outMin) {
            integralTerm = outMin;
        }

        // Compute the derivative term
        // http://en.wikipedia.org/wiki/PID_controller#Derivative_term
        double derivativeTerm;
        if(derivativeSource == PID_DERIVATIVE_SOURCE_ERROR) {
            derivativeTerm = kd * (error - lastError) / deltaSeconds;
        } else {
            // Use the process variable derivative here instead of error derivative.
            // This avoids excessive output movement when the setpoint changes:
            // http://en.wikipedia.org/wiki/PID_controller#Set_Point_step_change
            derivativeTerm = -1 * kd * (processVariable - lastProcessVariable) / deltaSeconds;
        }

        // Compute PID output
        output = proportionalTerm + integralTerm + derivativeTerm;

        // Ensure output is within output range
        if(output > outMax) {
            output = outMax;
        } else if(output < outMin) {
            output = outMin;
        }

        // Remember working variables
        lastProcessVariable = processVariable;
        lastError = error;
        lastTime = now;
    }

    return output;
}

void PID::setSetpoint(double setPoint) {
    this->setPoint = setPoint;
}

void PID::setTunings(double kp, double ki, double kd) {
    // Don't allow negative tunings
    if (kp < 0 || ki < 0 || kd < 0) return;

    // Reverse PID factors if required
    if(direction == PID_DIRECTION_REVERSE) {
        kp = 0 - kp;
        ki = 0 - ki;
        kd = 0 - kd;
    }

    // Store new tunings
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void PID::setDirection(int direction) {
    // Adjust PID factors if direction has changed
    if(direction != this->direction) {
        kp = 0 - kp;
        ki = 0 - ki;
        kd = 0 - kd;
    }

    // Store new direction
    this->direction = direction;
}

void PID::setSampleTime(int sampleTime) {
    if(sampleTime <= 0) return;

    // Store new sample time
    this->sampleTime = sampleTime;
}

void PID::setOutputLimits(double outMin, double outMax) {
    if(outMin >= outMax) return;

    // Ensure "integral" term is within output range to avoid integral windup:
    // http://en.wikipedia.org/wiki/PID_controller#Integral_windup
    if(integralTerm > outMax) {
        integralTerm = outMax;
    } else if(integralTerm < outMin) {
        integralTerm = outMin;
    }

    // Store new output limits
    this->outMin = outMin;
    this->outMax = outMax;
}

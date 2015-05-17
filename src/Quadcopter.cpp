#include "NavData.h"
#include "Quadcopter.h"

Quadcopter::Quadcopter(int motorConfiguration) {
    this->motorConfiguration = motorConfiguration;
}

void Quadcopter::initialize() {
    // Initialize sensors
    imu.initialize();

    // Set up PID controllers
    rollPid.setOutputLimits(-20, 20);
}

void Quadcopter::trim() {
    imu.calibrate();
}

void Quadcopter::setMovement(double leftTilt, double frontTilt, double verticalSpeed, double angularSpeed) {
    // TODO - store these locally
}

void Quadcopter::takeOff() {
    // TODO
}

void Quadcopter::land() {
    // TODO - takeoff
}

void Quadcopter::process() {
    // Read sensors
    imu.update();
    Vector3 orientation = imu.getOrientation();

    // Update navdata
    NavData::getInstance().updateOrientation(orientation);

    // Update the PIDs
    double rollOutput = rollPid.update(orientation.x);

    // Control the motors
    if(motorConfiguration == QUADCOPTER_CONFIGURATION_PLUS) {
        // // Pitch Control
        // M1 = M1 + RX(Elevator);
        // M4 = M4 - RX(Elevator);
        //
        // // Roll Control
        // M2 = M2 + RX(Aileron);
        // M3 = M3 - RX(Aileron);
    } else if(motorConfiguration == QUADCOPTER_CONFIGURATION_X) {
        // // Pitch Control
        // M1 = M1 + RX(Elevator) /2;
        // M3 = M3 + RX(Elevator) /2;
        // M4 = M4 - RX(Elevator) /2;
        // M2 = M2 - RX(Elevator) /2;
        //
        // // Roll Control
        // M1 = M1 + RX(Aileron)/2;
        // M2 = M2 + RX(Aileron) /2;
        // M3 = M3 - RX(Aileron) /2;
        // M4 = M4 - RX(Aileron) /2;
    }
}

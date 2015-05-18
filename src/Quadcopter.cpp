#include "Logger.h"
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
    pitchPid.setOutputLimits(-20, 20);
}

void Quadcopter::trim() {
    imu.calibrate();
    RGB.color(255, 255, 0);
}

void Quadcopter::setMovement(double leftTilt, double frontTilt, double verticalSpeed, double angularSpeed) {
    this->leftTilt = leftTilt;
    this->frontTilt = frontTilt;
    this->verticalSpeed = verticalSpeed;
    this->angularSpeed = angularSpeed;


}

void Quadcopter::takeOff() {
    RGB.color(0, 255, 0);

    state = QUADCOPTER_STATE_FLYING;
}

void Quadcopter::land() {
    RGB.color(255, 0, 0);

    state = QUADCOPTER_STATE_LANDED;
}

void Quadcopter::process() {
    // Read sensors
    imu.update();
    Vector3 orientation = imu.getOrientation();

    // Update navdata
    NavData::getInstance().updateOrientation(orientation);

    // http://theboredengineers.com/2012/05/the-quadcopter-basics/
    // https://github.com/benripley/Arduino-Quadcopter/tree/master/Quadcopter

    // Update the roll and pitch PIDs
    //
    // rollPower
    // - Leaning left = +20
    // - Leaning right = -20
    //
    // pitchPower
    // - Leaning forward = +20
    // - Leaning backward = -20
    double rollPower = rollPid.update(orientation.x);
    double pitchPower = pitchPid.update(orientation.y);

    // Control the motors
    if(motorConfiguration == QUADCOPTER_CONFIGURATION_PLUS) {
        // m1 = basePower + pitchPower;     // front
        // m2 = basePower + rollPower;      // left
        // m3 = basePower - rollPower;      // right
        // m4 = basePower + pitchPower;     // back
    } else if(motorConfiguration == QUADCOPTER_CONFIGURATION_X) {
        int hoverPower = 200;
        int throttleInfluence = 25;

        // throttlePower
        // - Full up throttle = +25
        // - Full down throttle = -25
        float throttlePower = verticalSpeed * throttleInfluence;

        // Calculate power for each motor
        m1Speed = hoverPower + throttlePower + rollPower / 2 + pitchPower / 2; // front/left
        m2Speed = hoverPower + throttlePower + rollPower / 2 - pitchPower / 2; // back/left
        m3Speed = hoverPower + throttlePower - rollPower / 2 + pitchPower / 2; // front/right
        m4Speed = hoverPower + throttlePower - rollPower / 2 - pitchPower / 2; // back/right
    }

    NavData::getInstance().updateMotors(m1Speed, m2Speed, m3Speed, m4Speed);

    if(state == QUADCOPTER_STATE_FLYING) {
        motor1.setSpeed(m1Speed);
        motor2.setSpeed(m2Speed);
        motor3.setSpeed(m3Speed);
        motor4.setSpeed(m4Speed);
    } else {
        motor1.setSpeed(0);
        motor2.setSpeed(0);
        motor3.setSpeed(0);
        motor4.setSpeed(0);
    }
}

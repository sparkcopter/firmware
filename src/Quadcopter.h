#pragma once

/*
    Quadcopters can be aligned in one of two configurations -

    X-Configuration:          Plus-Configuration:

    --1--       --3--               --1--
        o       o                     o
          o   o               |       o       |
            o                 2 o  o  o  o  o 3
          o   o               |       o       |
        o       o                     o
    --2--       --4--               --4--
*/

#include "spark_wiring.h"

#include "IMU.h"
#include "Motor.h"
#include "PID.h"


// Motor configuration (X or PLUS)
#define QUADCOPTER_CONFIGURATION_PLUS   0
#define QUADCOPTER_CONFIGURATION_X      1

// Motor pins
#define QUADCOPTER_MOTOR_PIN_1 A5
#define QUADCOPTER_MOTOR_PIN_2 A4
#define QUADCOPTER_MOTOR_PIN_3 A0
#define QUADCOPTER_MOTOR_PIN_4 A1

// Roll, pitch and yaw PID factors
#define QUADCOPTER_ROLL_PID_KP  2
#define QUADCOPTER_ROLL_PID_KI  5
#define QUADCOPTER_ROLL_PID_KD  1

#define QUADCOPTER_PITCH_PID_KP 0.5
#define QUADCOPTER_PITCH_PID_KI 0
#define QUADCOPTER_PITCH_PID_KD 1

#define QUADCOPTER_YAW_PID_KP   2
#define QUADCOPTER_YAW_PID_KI   5
#define QUADCOPTER_YAW_PID_KD   1

// Flying state
#define QUADCOPTER_STATE_LANDED     0
#define QUADCOPTER_STATE_TAKEOFF    1
#define QUADCOPTER_STATE_FLYING     2
#define QUADCOPTER_STATE_LANDING    3


class Quadcopter {
public:
    // Create a quadcopter with the specified configuration
    Quadcopter(int motorConfiguration);

    void initialize();

    // Calibrate ("trim") the quadcopter when on a flat surface
    void trim();

    // Set desired movement
    void setMovement(double leftTilt, double frontTilt, double verticalSpeed, double angularSpeed);

    // Take off and hover
    void takeOff();

    // Gracefully land
    void land();

    // Process sensor data and movement commands, and update motors accordingly
    void process();

private:
    // Is the quadcopter configured in X or Plus format
    int motorConfiguration;

    // IMU for orientation readings
    IMU imu = IMU(IMU_FILTER_COMPLEMENTARY);

    // PID controllers for roll, pitch and yaw control
    PID rollPid  = PID(QUADCOPTER_ROLL_PID_KP, QUADCOPTER_ROLL_PID_KI, QUADCOPTER_ROLL_PID_KD, PID_DIRECTION_DIRECT);
    PID pitchPid = PID(QUADCOPTER_PITCH_PID_KP, QUADCOPTER_PITCH_PID_KI, QUADCOPTER_PITCH_PID_KD, PID_DIRECTION_REVERSE);
    PID yawPid   = PID(QUADCOPTER_YAW_PID_KP, QUADCOPTER_YAW_PID_KI, QUADCOPTER_YAW_PID_KD, PID_DIRECTION_DIRECT);

    // A quadcopter has four motors
    Motor motor1 = Motor(QUADCOPTER_MOTOR_PIN_1);
    Motor motor2 = Motor(QUADCOPTER_MOTOR_PIN_2);
    Motor motor3 = Motor(QUADCOPTER_MOTOR_PIN_3);
    Motor motor4 = Motor(QUADCOPTER_MOTOR_PIN_4);

    // Movement state
    double leftTilt = 0.0, frontTilt = 0.0;
    double verticalSpeed = 0.0, angularSpeed = 0.0;

    // Flying state
    int state = QUADCOPTER_STATE_LANDED;

    // Motor speeds
    int m1Speed = 0, m2Speed = 0, m3Speed = 0, m4Speed = 0;
};

#include "Logger.h"
#include "MPU6050.h"
#include "Motor.h"
#include "UserInput.h"

// Constants
#define USER_INPUT_UDP_PORT     5556
#define MOTOR_PIN_FRONT_LEFT    A5
#define MOTOR_PIN_FRONT_RIGHT   A4
#define MOTOR_PIN_BACK_LEFT     A0
#define MOTOR_PIN_BACK_RIGHT    A1

// Sensor inputs
MPU6050 *sensors = new MPU6050();

// Motors
Motor *motorFrontLeft = new Motor(MOTOR_PIN_FRONT_LEFT);
Motor *motorFrontRight = new Motor(MOTOR_PIN_FRONT_RIGHT);
Motor *motorBackLeft = new Motor(MOTOR_PIN_BACK_LEFT);
Motor *motorBackRight = new Motor(MOTOR_PIN_BACK_RIGHT);

// User input
UserInput *userInput = new UserInput(USER_INPUT_UDP_PORT);

// Control callbacks
void takeoff() {
    RGB.color(0, 255, 0);
}

void land() {
    RGB.color(255, 0, 0);
}

void move(float leftTilt, float frontTilt, float verticalSpeed, float angularSpeed) {
    /*Logger::log("Move: %f, %f, %f, %f", leftTilt, frontTilt, verticalSpeed, angularSpeed);*/
}

void setup() {
    // Enable serial logging
    Logger::init();

    // Set up spark LED
    RGB.control(true);
    RGB.color(255, 0, 0);

    // Initialize user input and attach callbacks
    userInput->init();
    userInput->takeoff = &takeoff;
    userInput->land = &land;
    userInput->move = &move;

    // Initialize sensors
    sensors->init();

    // Spin up all motors for testing
    motorFrontLeft->setSpeed(255);
    motorFrontRight->setSpeed(255);
    motorBackLeft->setSpeed(255);
    motorBackRight->setSpeed(255);
}

void loop() {
    int16_t accelX, accelY, accelZ;
    int16_t gyroX, gyroY, gyroZ;

    // Get the spark's IP address
    /*IPAddress myIp = WiFi.localIP();*/
    /*Logger::log("IP: %d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);*/

    // Read control input
    userInput->read();

    // Read sensors
    sensors->getMotion(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
    /*Logger::log("Accel: x=%d y=%d z=%d", accelX, accelY, accelZ);*/
    /*Logger::log("Gyro: x=%d y=%d z=%d", gyroX, gyroY, gyroZ);*/

    delay(10);
}

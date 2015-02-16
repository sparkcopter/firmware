#include "MPU6050.h"
#include "Motor.h"
#include "UserInput.h"

// Constants
#define USER_INPUT_TCP_PORT     23
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
UserInput *userInput = new UserInput(USER_INPUT_TCP_PORT);

void setup() {
    // Setup USB serial output
    Serial.begin(9600);

    // Initialize sensors
    sensors->init();

    // Spin up all motors for testing
    motorFrontLeft->setSpeed(255);
    motorFrontRight->setSpeed(255);
    motorBackLeft->setSpeed(255);
    motorBackRight->setSpeed(255);

    // Prints out the local IP over Serial.
    Serial.println(WiFi.localIP());
}

void loop() {
    int16_t accelX, accelY, accelZ;
    int16_t gyroX, gyroY, gyroZ;

    sensors->getMotion(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);

    //
    // Accelerometer data
    //

    // Positive X = "Tilting left"
    // Negative X = "Tilting right"
    // Positive Y = "Tilting backward"
    // Negative Y = "Tilting forward"
    // Positive Z = "Tilting up"
    // Negative Z = "Tilting down"

    Serial.print("Accelerometer: ");
    Serial.print("x=");
    Serial.print(accelX);
    Serial.print(" y=");
    Serial.print(accelY);
    Serial.print(" z=");
    Serial.print(accelZ);
    Serial.println();


    //
    // Gyroscope data
    //

    // Negative X = "Moving up"
    // Positive X = "Moving down"
    // Negative Y = "Moving left"
    // Positive Y = "Moving right"
    // Positive Z = "Facing up"
    // Negative Z = "Facing down"

    Serial.print("Gyroscope: ");
    Serial.print("x=");
    Serial.print(gyroX);
    Serial.print(" y=");
    Serial.print(gyroY);
    Serial.print(" z=");
    Serial.print(gyroZ);
    Serial.println();


    delay(100);
}

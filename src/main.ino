#include "MPU6050.h"
#include "Motor.h"

// Sensor inputs
MPU6050 *sensors = new MPU6050();

// Motors
Motor *motorFrontLeft = new Motor(A5);
Motor *motorFrontRight = new Motor(A4);
Motor *motorBackLeft = new Motor(A0);
Motor *motorBackRight = new Motor(A1);

void setup() {
    // Setup USB serial output
    Serial.begin(9600);

    // Initialize sensors
    sensors->init();

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

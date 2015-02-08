#include "MPU6050.h"

// Sensor inputs
MPU6050 *sensors = new MPU6050();

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

    Serial.print("Accelerometer: ");
    Serial.print("x=");
    Serial.print(accelX);
    Serial.print(" y=");
    Serial.print(accelY);
    Serial.print(" z=");
    Serial.print(accelZ);
    Serial.println();

    // Negative X = "Tilting right"
    // Positive X = "Tilting left"
    // Negative Y = "Tilting forward"
    // Positive Y = "Tilting backward"
    // Positive Z = "Facing up"
    // Negative Z = "Facing down"

    //
    // Gyroscope data
    //

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

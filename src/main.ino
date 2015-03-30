#include "Logger.h"
#include "IMU.h"
#include "Motor.h"
#include "UserInput.h"

// Constants
#define USER_INPUT_UDP_PORT     5556
#define MOTOR_PIN_FRONT_LEFT    A5
#define MOTOR_PIN_FRONT_RIGHT   A4
#define MOTOR_PIN_BACK_LEFT     A0
#define MOTOR_PIN_BACK_RIGHT    A1

// Sensor inputs
IMU imu;

// Motors
Motor motorFrontLeft = Motor(MOTOR_PIN_FRONT_LEFT);
Motor motorFrontRight = Motor(MOTOR_PIN_FRONT_RIGHT);
Motor motorBackLeft = Motor(MOTOR_PIN_BACK_LEFT);
Motor motorBackRight = Motor(MOTOR_PIN_BACK_RIGHT);

// User input
UserInput userInput = UserInput(USER_INPUT_UDP_PORT);

unsigned long printTimer = millis();
unsigned long lastTrim = 0;

// Control callbacks
volatile bool emergency = false;
volatile bool takeoff = false;

void ref(bool pEmergency, bool pTakeoff) {
    Logger::debug("Prev: %d %d", emergency, takeoff);
    Logger::debug("Ref: %d %d", pEmergency, pTakeoff);
    // Takeoff state has changed
    if(takeoff != pTakeoff) {
        if(pTakeoff) {
            Logger::debug("Changed state to 'takeoff'");
            RGB.color(0, 255, 0);
        } else {
            Logger::debug("Changed state to 'land'");
            RGB.color(255, 0, 0);
        }

        takeoff = pTakeoff;
    }

    // Emergency state has changed
    if(emergency != pEmergency) {
        // TODO: Something
        emergency = pEmergency;
    }
}

void pcmd(bool progressive, bool combinedYaw, float leftTilt, float frontTilt, float verticalSpeed, float angularSpeed) {
    // TODO
}

void ftrim() {
    unsigned long now = millis();
    if(lastTrim == 0 || now > lastTrim + 5000) {
        imu.calibrate();
        RGB.color(255, 255, 0);
        lastTrim = now;
    }
}

void setup() {
    // Enable I2C bus
    Wire.begin();

    // Enable serial logging
    Logger::init();

    // Set up spark LED
    RGB.control(true);
    RGB.color(255, 0, 0); // Red LED - landed and not calibrated

    // Initialize user input and attach callbacks
    userInput.init();
    userInput.ref = &ref;
    userInput.pcmd = &pcmd;
    userInput.ftrim = &ftrim;

    // Initialize sensors
    imu.initialize();

    // Spin up all motors for testing
    motorFrontLeft.setSpeed(255);
    motorFrontRight.setSpeed(255);
    motorBackLeft.setSpeed(255);
    motorBackRight.setSpeed(255);
}

void loop() {
    // Debug info
    if(millis() > printTimer + 5000) {
        IPAddress myIp = WiFi.localIP();
        Logger::debug("IP: %d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
        printTimer = millis();
    }

    // Read control input
    userInput.read();

    // Read sensors
    float yaw, pitch, roll;
    imu.getYawPitchRoll(&yaw, &pitch, &roll);

    delay(10);
}

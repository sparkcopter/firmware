#include "Logger.h"
#include "IMU.h"
#include "Motor.h"
#include "UserInput.h"

// Constants
#define MOTOR_PIN_FRONT_LEFT    A0
#define MOTOR_PIN_FRONT_RIGHT   A1
#define MOTOR_PIN_BACK_RIGHT    A4
#define MOTOR_PIN_BACK_LEFT     A5

// Sensor inputs
IMU imu;

// Motors
Motor motorFrontLeft = Motor(MOTOR_PIN_FRONT_LEFT);
Motor motorFrontRight = Motor(MOTOR_PIN_FRONT_RIGHT);
Motor motorBackLeft = Motor(MOTOR_PIN_BACK_LEFT);
Motor motorBackRight = Motor(MOTOR_PIN_BACK_RIGHT);

// User input
UserInput userInput;

unsigned long printTimer = millis();
unsigned long lastTrim = 0;

// Control callbacks
volatile bool emergency = false;
volatile bool takeoff = false;

void ref(bool pEmergency, bool pTakeoff) {
    // Takeoff state has changed
    if(takeoff != pTakeoff) {
        if(pTakeoff) {
            Logger::debug("Changed state to 'takeoff'");
            RGB.color(0, 255, 0);

            motorFrontLeft.setSpeed(255);
            motorFrontRight.setSpeed(255);
            motorBackLeft.setSpeed(255);
            motorBackRight.setSpeed(255);
        } else {
            Logger::debug("Changed state to 'land'");
            RGB.color(255, 0, 0);

            motorFrontLeft.setSpeed(0);
            motorFrontRight.setSpeed(0);
            motorBackLeft.setSpeed(0);
            motorBackRight.setSpeed(0);
        }

        takeoff = pTakeoff;
    }

    // Emergency state has changed
    if(emergency != pEmergency) {
        emergency = pEmergency;
    }
}

void pcmd(bool progressive, bool combinedYaw, float leftTilt, float frontTilt, float verticalSpeed, float angularSpeed) {
    // Allow testing the individual motors
    if(leftTilt > 0) {
        motorFrontLeft.setSpeed((int)(255 * leftTilt));
    } else if(leftTilt < 0) {
        motorBackLeft.setSpeed((int)(-255 * leftTilt));
    }

    if(frontTilt > 0) {
        motorFrontRight.setSpeed((int)(255 * frontTilt));
    } else if(frontTilt < 0) {
        motorBackRight.setSpeed((int)(-255 * frontTilt));
    }
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
}

void loop() {
    // Read control input
    userInput.read();
    imu.update();

    // Read sensors
    float yaw, pitch, roll;
    imu.getYawPitchRoll(&yaw, &pitch, &roll);

    // Debug info
    if(millis() > printTimer + 50) {
        /*IPAddress myIp = WiFi.localIP();
        Logger::debug("IP: %d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);*/

        Logger::debug("yaw/pitch/roll: %f/%f/%f", yaw, pitch, roll);

        printTimer = millis();
    }

    delay(10);
}

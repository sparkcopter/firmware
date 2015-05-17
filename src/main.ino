#include "Logger.h"
#include "NavData.h"
#include "Quadcopter.h"
#include "UserInput.h"


//
// Set up our component parts
//

// Quadcopter
Quadcopter quadcopter = Quadcopter(QUADCOPTER_CONFIGURATION_X);

// User input
UserInput userInput = UserInput();


//
// Define control callbacks
//

// Takeoff and land commands
volatile bool takeoff = false;
void ref(bool pEmergency, bool pTakeoff) {
    if(takeoff != pTakeoff) {
        if(pTakeoff) {
            quadcopter.takeOff();;
        } else {
            quadcopter.land();
        }

        takeoff = pTakeoff;
    }
}

// Movement commands
void pcmd(bool progressive, bool combinedYaw, float leftTilt, float frontTilt, float verticalSpeed, float angularSpeed) {
    quadcopter.setMovement(leftTilt, frontTilt, verticalSpeed, angularSpeed);
}

// Trim command
void ftrim() {
    // TODO: de-bounce multiple trim command sent in quick succession
    quadcopter.trim();
}


//
// Spark's setup and loop commands
//

void setup() {
    // Enable I2C bus
    Wire.setSpeed(CLOCK_SPEED_400KHZ);
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

    // Initialize quadcopter
    quadcopter.initialize();

    // Initialize navigation data stream
    NavData::getInstance().init();
}

void loop() {
    // Read control input
    userInput.read();

    // Process user inputs and sensor data
    quadcopter.process();

    // Send latest nav data
    NavData::getInstance().checkForClient();
    NavData::getInstance().send();
}

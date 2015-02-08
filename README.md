Spark Core Quadcopter
=====================

[WIP] Firmware and designs for a from-scratch [Spark Core](http://spark.io) based quadcopter.


Hardware
--------

-   [Spark core](http://spark.io/firmware)
-   [MPU-6050 accelerometer/gyroscope](http://playground.arduino.cc/Main/MPU-6050)
-   Hubsan x4 motors
-   Hubsan x4 propellers
-   3.7v LIPO battery
-   3d-printed frame


Firmware
--------

-   Sensor reading: Read values from gyroscope and accelerometer
-   Control inputs: Read simple commands from the network
-   Motor control: Adjust motor speeds based on sensor values and control inputs


Notes
-----

### Motors:

-   Clockwise: Red + / Blue -
-   Counter Clockwise: White + / Black -

### MPU-6050

-   D0 <-> SDA
-   D1 <-> SCL

### Links

-   http://docs.spark.io/firmware/
-   https://ghowen.me/build-your-own-quadcopter-autopilot/
-   https://itp.nyu.edu/physcomp/labs/motors-and-transistors/using-a-transistor-to-control-high-current-loads-with-an-arduino/
-   https://itp.nyu.edu/physcomp/wp-content/uploads/LabHighCurrentArduinoMotor_1_diode_schem.png
-   http://www.reprise.com/host/circuits/transistor_pinouts.asp

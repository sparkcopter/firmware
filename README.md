[WIP] Firmware for from-scratch quadcopter
==========================================

Components
----------
-   Spark core
-   MPU-6050 accelerometer/gyroscope (GY-521)
-   Hubsan x4 motors
-   Hubsan x4 propellers
-   3.7v LIPO battery
-   3d-printed frame

Code modules
------------
-   Sensor reading: Read values from gyroscope and accelerometer
-   Control inputs: Read simple commands from the network
-   Motor control: Adjust motor speeds based on sensor values and control inputs

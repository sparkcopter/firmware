Sparkcopter Firmware
====================

Firmware and code tutorial for the Sparkcopter, a [Spark Core](http://spark.io) based quadcopter. Currently a work in progress.


Contents
--------
-   [Compiling](#compiling)
-   [Tutorial](#tutorial)
    -   [Reading Sensors](#reading-sensors) - read values from the gyroscope and accelerometer
    -   [Sensor Calibration](#sensor-calibration) - calibrate raw sensor readings
    -   [Sensor Fusion](#sensor-fusion) - combine sensor readings with an IMU
    -   [Motor Control](#motor-control) - control motor speed with PWM/analog output
    -   [Pilot Input](#user-input) - read flight input data over the network
    -   [Stabilized Control](#stabilized-control) - combine IMU data with flight inputs using a PID controller
-   [License](#license)


Compiling
---------

-   **Install the Spark CLI**

    ```shell
    npm install -g spark-cli
    spark cloud login
    ```

-   **Compile and flash remotely**

    ```shell
    SPARK_NAME=spark-name make clean install
    ```

-   **Compile and flash via USB**

    ```shell
    make clean install-usb
    ```


Tutorial
--------

### Reading Sensors

Our hardware contains an MPU-6050 chip, which has an on-board gyroscope and accelerometer.

- TODO: How to read these raw values?


### Sensor Calibration

- TODO: Why calibrate sensors?
- TODO: How to calibrate


### Sensor Fusion

- TODO: Why combine gyroscope and accelerometer values?
- TODO: Various techniques available
- TODO: Chosen technique (madgwick)


### Motor Control

- TODO: Introduction to brushed motors
- TODO: What is pulse width modulation
- TODO: Spark's analogWrite


### Pilot Input

- TODO: Wifi vs 2.4ghz
- TODO: List of flight commands
- TODO: What is UDP
- TODO: AR Drone protocol


### Stabilized Control

- TODO: What is a PID controller
- TODO: Combining gyro rotation rate with desired rotation rate (rate control)
- TODO: Combining sensor angles with desired angles (stabilized control)


License
-------

The Sparkcopter firmware free software released under the MIT License. See [LICENSE.txt](LICENSE.txt) for details.

Sparkcopter Firmware
====================

Firmware and code tutorial for the Sparkcopter, a [Spark Core](http://spark.io) based quadcopter. Currently a work in progress.


Contents
--------
-   [Compiling](#compiling)
-   [Tutorial](#tutorial)
    -   [Reading Sensors](#reading-sensors) - read values from the gyroscope and accelerometer
    -   [Scaling Sensor Values](#scaling-sensor-values) - converting to Gs and degrees per second
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

Our hardware contains an MPU-6050 chip, which has an on-board gyroscope and accelerometer. The MPU-6050's sensor data is accessible over the [I2C bus](http://en.wikipedia.org/wiki/I%C2%B2C), via the `D0/SDA` and `D1/SCL` pins on the Spark Core.

Raw I2C access is possible using Spark's [Wire](http://docs.spark.io/firmware/#communication-wire) library, but for easier access we'll use Jeff Rowberg's excellent [i2cdevlib](https://github.com/jrowberg/i2cdevlib) library which includes some [convenient wrapper functions](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050) for accessing sensor data from our MPU-6050.

> Note: The i2cdevlib library doesn't yet support the Spark Core out of the box, so check out my [my pull request](https://github.com/jrowberg/i2cdevlib/pull/156) which adds support.

Raw sensor values can then easily be read as follows:

```c++
// Initialize sensors
MPU6050 sensors = MPU6050();
sensors.initialize();

// Read raw sensor values
int16_t rax, ray, raz, rgx, rgy, rgz;
sensors.getMotion6(&rax, &ray, &raz, &rgx, &rgy, &rgz);
```


### Scaling Sensor Values

Sensor values returned by `getMotion6` are raw 16-bit values, so we'll need to convert them into [Gs](http://en.wikipedia.org/wiki/G-force) for accelerometer data, and [degrees per second](http://en.wikipedia.org/wiki/Rotational_speed) for gyroscope data.

To convert our raw data, we'll need to get the configured resolution of the sensors:

```c++
// Get scaling factors for sensors
float accelScale = 2048.0 * pow(2, 3 - sensors.getFullScaleAccelRange());
float gyroScale = 16.4 * pow(2, 3 - sensors.getFullScaleGyroRange());
```

Then we can convert our raw values to the appropriate units:

```c++
// Convert accelerometer values into Gs
float ax = rax/accelScale;
float ay = ray/accelScale;
float az = raz/accelScale;

// Convert gyroscope values into degrees per second
float gx = rgx/accelScale;
float gy = rgy/accelScale;
float gz = rgz/accelScale;
```


### Sensor Calibration

The sensors in the MPU-6050 come with some factory calibration, but to get accurate readings we'll need to calibrate further ourselves. The MPU-6050 allows for setting new "offsets" for both the accelerometer and gyroscope. Calibrating the sensors is important to ensure we have orientation data for steady flight.

To find our "baseline" readings, we'll take a number of sensor readings on a flat surface, take the average, then store them back in the MPU-6050. Check out `IMU::calibrate` in [IMU.cpp](https://github.com/sparkcopter/firmware/blob/master/src/IMU.cpp) for the full code.


### Sensor Fusion

As an accelerometer measures all forces that are working on the object, it will also see a lot more than just the gravity vector. Every small force working on the object will disturb our measurement completely.

Gyroscope measurements have the tendency to drift, not returning to zero when the system returns to its original position. The gyroscope data is reliable only on the short term, as it starts to drift on the long term.

Sensor fusion gives us the "best of both worlds". On the short term, we use the data from the gyroscope, because it is very precise and not susceptible to external forces. On the long term, we use the data from the accelerometer, as it does not drift.

There are various techniques available to combine this data, including the famous [Kalman Filter](http://en.wikipedia.org/wiki/Kalman_filter), but we'll be using the faster [Madgwick Direct Cosine Matrix Filter](http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/).


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

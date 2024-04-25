# P3\_BROWN Buoyant Station-Keeping Building Prototype Project

This project uses a pi pico w to control many motors autonomously using data retrieved by a mpu6050 IMU.

## Current Features:
- read acellerometer and gyrometer info from mpu6050 sensor
- control stepper motor direction and speed

## HOW TO:
- building instructions:
1. Plug in pico
2. Run the following to build and upload
$ ./build.sh && ./upload.sh
(this should run cmake and make to build the uf2 binary and upload it to the pico)
(make sure the pico sdk and pico mount path is correct and that the build folder exists)
3. Then view output with UART or USB depending on CMakeLists.txt config using minicom or another serial com viewer

## TODO:
- use sensor readings to operate motors
- calculate accurate interpretation of position and rotation given data from sensors



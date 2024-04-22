#!/usr/bin/bash
export PICO_SDK_PATH="/home/marcus/pi/pico/pico-sdk"
cd build
cmake .. -DPICO_BOARD=pico_w
make

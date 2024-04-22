#!/usr/bin/bash
serial_path=/dev/ttyACM1
minicom -b 115200 -o -D $serial_path

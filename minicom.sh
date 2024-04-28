#!/usr/bin/bash
serial_path=/dev/ttyUSB0
sudo minicom -b 115200 -D $serial_path

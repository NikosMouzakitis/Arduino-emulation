#!/bin/bash
avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:build-uno/Arduino-emulation.hex:i
minicom -D /dev/ttyACM0 -b 9600

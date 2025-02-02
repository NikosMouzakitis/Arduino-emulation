#!/bin/bash
qemu-system-avr -cpu atmega328p -bios /dev/null -serial mon:stdio -drive file=blinky.hex,format=ihex


BOARD_TAG    = uno
MCU           = atmega328p
F_CPU         = 16000000
ARDUINO_DIR   = /usr/share/arduino
AVR_TOOLS_DIR = /usr
AVRDUDE_CONF  = /usr/share/arduino/hardware/tools/avr/etc/avrdude.conf
ARDUINO_LIBS = avr-context #for context-switching.
#DEBUG=1
#CFLAGS += -g
include /usr/share/arduino/Arduino.mk


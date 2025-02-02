# Makefile

# Path to Arduino installation
ARDUINO_PATH = /usr/share/arduino/hardware/arduino
ARDUINO_CORE = $(ARDUINO_PATH)/cores/arduino
ARDUINO_LIB = $(ARDUINO_PATH)/libraries
ARDUINO_VARIANTS = $(ARDUINO_PATH)/variants/standard  # Update this path if using a different board

# Include the Arduino core files and variants
CXXFLAGS += -I$(ARDUINO_CORE) -I$(ARDUINO_LIB) -I$(ARDUINO_VARIANTS)

# Set the AVR toolchain
CC = avr-gcc
CXX = avr-g++
MCU = atmega328p
F_CPU = 16000000UL
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS = -mmcu=$(MCU)

# Link the math library if needed
LIBS += -lm

# Source files
SRC = blinky.cpp

# Output file
OUT = blinky.hex

# Compile and link
$(OUT): $(SRC)
	$(CXX) $(CFLAGS) -I$(ARDUINO_CORE) -I$(ARDUINO_LIB) -I$(ARDUINO_VARIANTS) -o blinky.elf $(SRC) $(LDFLAGS)
	avr-objcopy -O ihex -R .eeprom blinky.elf $(OUT)

clean:
	rm -f *.elf *.hex


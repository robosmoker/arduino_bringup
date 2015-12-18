# Makefile fragment made by /home/pi/bin/mkarduino
ARDUINO_DIR    = /usr/share/arduino
ARDMK_DIR      = Arduino-Makefile
AVR_TOOLS_DIR  = /usr
AVRDUDE_CONF   = ./avrdude_gpio.conf

USER_LIB_PATH  = libraries

TARGET         = arduino_bringup
ARDUINO_LIBS   = EventManager/EventManager Wire

BOARDS_TXT = ./boards.txt
BOARD_TAG = atmega328_9600_8
ARDUINO_PORT = /dev/ttyUSB0

BOOTLOADER_PATH = $(PWD)
BOOTLOADER_FILE = optiboot_atmega328_9600_8.hex

ISP_PROG = pi_1
ISP_PORT = /dev/null

include Arduino-Makefile/Arduino.mk

# Makefile fragment made by /home/pi/bin/mkarduino
ARDUINO_DIR    = /usr/share/arduino
ARDMK_DIR      = Arduino-Makefile
AVR_TOOLS_DIR  = /usr

USER_LIB_PATH  = libraries

TARGET         = arduino_bringup
#ARDUINO_LIBS   = 

BOARD_TAG = nano328
ARDUINO_PORT = /dev/ttyUSB1

include Arduino-Makefile/Arduino.mk

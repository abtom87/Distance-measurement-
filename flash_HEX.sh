#!/bin/bash
# Usage--> $ ./flash.hex Release/[name_of_the_hex_file].hex
clear
avrdude -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:$1

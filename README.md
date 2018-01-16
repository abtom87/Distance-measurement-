# Distance measurement using HC-SR04 and ATMEGA328p.
Distance measurement using HC-SR04 and ATMEGA328p.
In this project i have measured distance in centimetres, with the help of HC-SR04 Ultrasound sensor, ATMEGA328p microcontroller, LCD Display via I2C bus. 

## Principle: ##
Timer2 of ATMEGA328p is used to generate a Trigger pulse of 20uS, the ultrasonic module sends out a 8cycle burst of 40khz which hits the object surface and returns back to raise an echo pulse. The pulse-width of this pulse is proportional to the distance between the module and Object. 

Input capture module of the ATMEGA was used to capture the time between rising and falling edges of 
the echo pulse. The prescaler of this unit was chosen, such that the resolution of pulse-width is 16uS.

The Timer count (OCR2A) of Timer2 was chosen such that a trigger pulse signal of width 10uS is sent, and there is a
pause (pin stays Digtal Low) for around 1.4mS. 
 



## Hardware used: ##
- Arduino-UNO
- HC-SR04 Ultrasonice sensor
- LCD Display with integrated PCf8574T chip enabling serial (I2C from uC) to parallel conversion.
- 2.2k Pullup resistors for I2C bus.

## SW and Toolchain: ##
- Software was written entirely in C,
- Developed in EClipse CDT,
- AVR-GCC compiler was used, 
- AVRDUDE was used to program the hex onto the target,
- Operating System - Ubuntu(Linux)

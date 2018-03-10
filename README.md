# hidjoy2spi

A small application that reads input from your linux joystick api, e.g. `/dev/input/js0`
and pipes it into the SPI bus on the Raspberry PI.

It is to be used with the sister project [xpad-avr](https://github.com/ripdajacker/xpad-avr)
to redirect arbitrary joystick input into the original Xbox.
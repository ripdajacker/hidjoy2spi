# hidjoy2spi

A small application that reads input from your linux joystick api, e.g. `/dev/input/js0`
and pipes it into the SPI bus on the Raspberry PI.

It is to be used with the sister project [xpad-avr](https://github.com/ripdajacker/xpad-avr)
to redirect arbitrary joystick input into the original Xbox.

The project is licensed under the [GNU GPL v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt).

## Step by step

You need a Raspberry PI or other SPI-capable linux computer + an atmega32u4.

0. Install the prerequisites:

    sudo apt install git cmake gcc g++ build-essential

1. Checkout and build the project:

    git clone https://github.com/ripdajacker/hidjoy2spi.git
    cd hidjoy2spi
    cmake .
    make
    
2. Wire the SPI bus of the Raspberry to the SPI pins on your atmega32u4 like seen [here](https://github.com/ripdajacker/xpad-avr/blob/master/xpad-avr-wiring.pdf).

3. Connect an Xbox One or wired 360 gamepad to the Raspberry (you will need an OTG adapter if it is a Raspberry Pi Zero).

4. Flash the newest `XpadEmu.hex` from the [xpad-avr](https://github.com/ripdajacker/xpad-avr/releases) project (Windows users: use [AVRDUDESS](https://github.com/zkemble/AVRDUDESS), Linux/Mac users can use `avrdude`).

5. Run the hid2joy application like so:

    ./hid_spi_redirect /dev/input/js0 /dev/spidev0.1

6. Your inputs are now converted.

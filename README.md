# OCTANE
## Custom quadcopter firmware

### About:
This project is a challenge I set to myself to try and make a quadcopter firmware (How hard can it be?).
By no means is this a replacement of other firmwares and should not be considered one.

### Specs:
 - Runs on an STM32F103C8T6 microcontroller (The Blue Pill board specifically)
 - Compile time options for faster code
 - Currentlly supports PPM, Oneshot125, Oneshot42, Multishot (and DShot in the future?)
 - "Dotstar" style SPI addressable LEDs for direction and state lights
 - USART for more information
 - (Planning on) common 2.4Ghz nRF24 module as communication interface
 - Telemetry data
 - more when I am not bored ...

### Bulding:
This is the complete Eclipse MCU project folder except the actual build files
Just git clone, import, build

### Flashing:
Flashing requires the st-link v2 module and a GUI or console program to flash.

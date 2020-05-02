# A Hardware Watchdog Timer for restarting your unresponsive device

## Overview

This code implements a standalone watchdog timer, allowing the target device
to be restarted (or other action taken), when no input is received after a
set amount of time (defined in AWT_TIMEOUT).

This was originally implemented with the objective of restarting an Android
phone that was repurposed as an IP camera. Given that the device could occasionally
shutdown due to lack of battery power, or crash due to software bugs, and because 
it would be important to maximize its availability without human intervention, 
with that in mind the idea of creating this device came to fruition.

If you have curiosity, the IP camera project was described in this blog post:

http://creationfactory.blogspot.pt/2016/03/repurposing-old-android-phone.html

The original idea was to have this Watchdog Timer (WDT) device close the circuit 
from the power button of the Android device, when there is no incoming pulse 
for a set amount of time. 

On the other hand the Android device would be responsible for sending a pulse
(e.g. through a cron expression) after a set amount of time. This pulse is 
received via the GP2 pin. For example if the AWT_TIMEOUT parameter would be 
set to 120 seconds, we would set the Android device to send pulses within that 
period, e.g. every 30 seconds.

If for any reason the target device (Android phone) is completely irresponsive, 
the assumption is that it will not be able to respond with the pulse. Once the
WDT timer expires, the microcontroller will set the GP3 pin high for enough
time to cause the target device to restart (this may depend on the phone or
other device this circuit is to be used with). This time is defined by the 
RESET_PRESS_DURATION constant.

## Building and flashing

This project can be built on the MPLAB X IDE. It requires the XC8 toolchain for
compiling the firmware image.

For building it can simply be opened in the IDE, and launch the project build.
Alternatively you can the programming of the device directly, if you have 
a PIC ready to be flashed in your programmer. It should be possible to use 
practically any Microchip programmer to flash the device (e.g. a PICkit 2 or 3).

You probably can have success with one of the Arduino based programmers that 
are around, but I haven't personally tested.

## Pin mappings of the PIC12F683

Below are the pin mappings for this firmware:


```
                      +---------------+
    +5V         -   --|(VDD)|   |(VSS)|--   - GND
                      |     \---/     |
    N/A         -   --|(GP5)     (AN0)|--   - N/A
                      |               |
    N/A         -   --|(GP4)     (GP1)|--   - N/A
                      |               |
    PWR_BTN     <   --|(GP3)     (GP2)|--   < WDT_RESET
                      +---------------+
```

## Potential improvements

 * Define a separate timer, in order to wait longer for devices that take a long
 time to boot (above AWT_TIMEOUT);

## License

Author: Luis Teixeira (https://creationfactory.co)

Licence and copyright notice:

Copyright 2020 Luis Teixeira

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, softwar
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

/**
 * main.h
 *
 * Main module definition file
 *
 * Author: Luis Teixeira (teixeluis@gmail.com)
 *
 *
 *
 * Licence and copyright notice:
 *
 * Copyright 2014 Luis Teixeira
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, softwar
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef _MAIN_H
#define _MAIN_H

#include <pic12f683.h>
#include "pic.h"

#define TMR1_OFFSET     0x0BDC    // initializes to 3036, so that at T/8 = 125 KHz it will overflow after 0.5 secs
//#define TMR1_OFFSET             0x8000  // initializes to 32768, so that at T = 32.768 KHz TMR1 will overflow after 1 sec

#define AWT_TIMEOUT             120      // 120 second timeout for the watchdog
#define RESET_PRESS_DURATION    8000   // ms to keep power button "pressed" for resetting Android device

unsigned int timerCount;

/**
 * Procedure for initialization of features within the PIC such as the AD
 * converters and timers.
 *
 */
void initSys(void);

void initTimer(void);

void stopTimer(void);

void resetAndroid(void);
void disableInterrupts(void);
void enableInterrupts(void);

/**
 * The interrupt service routine for handling the interrupt caused by the shutter
 * timer (once every 0.5 seconds).
 */

#pragma interrupt isr            // declare function as high priority isr

void interrupt isr(void);

#endif
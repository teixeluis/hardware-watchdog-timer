/*
 * main.h
 *
 * Main module of the "AndroidWatchdogTimer" firmware.
 *
 * This source code implements a standalone watchdog timer for the Android
 * based camera project documented here:
 * 
 * http://creationfactory.blogspot.pt/2016/03/repurposing-old-android-phone.html
 *
 * The objective of this WDT is to trigger the Android device power button when 
 * there is no incoming pulse for a set amount of time. This time should be equal
 * or greater than the pulse repetition rate on the Android device.
 * 
 * As such this WDT can serve the purpose of either waking up the Android device
 * after it have been shutdown due to lack of battery power, or to restart the
 * Android device upon a hardware or software crash.
 * 
 * The source code is accompanied with an application schematic diagram to
 * help in the construction of a complete unit using the PIC12F683 microcontroller
 * and this firmware.
 *
 * The diagram below shows the pin mappings of the microntroller to this concrete
 * application:
 *
 *                       +---------------+
 *     +5V         -   --|(VDD)|   |(VSS)|--   - GND
 *                       |     \---/     |
 *     N/A         -   --|(GP5)     (AN0)|--   - N/A
 *                       |               |
 *     N/A         -   --|(GP4)     (GP1)|--   - N/A
 *                       |               |
 *     PWR_BTN     <   --|(GP3)     (GP2)|--   < WDT_RESET
 *                       +---------------+
 *
 * Author: Luis Teixeira (teixeluis@gmail.com)
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

#include "main.h"
#include "common.h"

void initSys(void) {
    TRISIO = 0b000100;          // GP2 (pin 5) set as  input. Remaining pins
                                // set as outputs.

    ANSEL = 0b0000000;        // selected Fosc/8 (smallest value allowed for 4 MHz clock)
                                // and no ADC inputs active.
    
    GPIO = 0;                   // reset GPIO bits

    // configure timer1:

    TMR1CS = 0; // select internal Fosc/4 clock source
    T1CONbits.T1CKPS = 0b11; // enable 1:8 prescale
    
    //T1OSCEN = 1; // use the 32768 Hz oscillator for TMR1
    //TMR1CS = 1; // select LP oscillator
    //T1CONbits.T1CKPS = 0b00; // enable 1:1 prescale
}

void initTimer(void) {
    TMR1ON = 0;
    TMR1IE = 0;
    TMR1IF = 0;

    TMR1 = TMR1_OFFSET;
    
    timerCount = 0;

    // Enable timer1:
    //enableInterrupts();
    
    TMR1ON = 1;
    
    TMR1IE = 1; // re-enable timer0 interrupts
    PEIE = 1;
    GIE = 1;
    INTE = 1;   // re-enable interrupt pin interrupts
}

void disableInterrupts() {
    TMR1IE = 0; // disable timer1 interrupts
    PEIE = 0;
    GIE = 0;
    INTE = 0;   // disable pin change interrupts
}

void enableInterrupts() {
    TMR1IF = 0; // reset timer interrupt flag
    INTF = 0;   // reset interrupt pin flag
    
    TMR1IE = 1; // re-enable timer0 interrupts
    PEIE = 1;
    GIE = 1;
    INTE = 1;   // re-enable interrupt pin interrupts
}
void resetAndroid(void) {
    GP1 = 1;
    __delay_ms(RESET_PRESS_DURATION);
    GP1 = 0;
}

void interrupt isr(void) {
    //disableInterrupts();
    
    TMR1IE = 0; // disable timer1 interrupts
    PEIE = 0;
    GIE = 0;
    INTE = 0;   // disable pin change interrupts
    
    if(TMR1IF) { // Timer expired
        TMR1 = TMR1_OFFSET;

        if(timerCount < AWT_TIMEOUT * 2) {
            timerCount++;
        }
        else {
            //resetAndroid();
            
            GP1 = 1;
            __delay_ms(RESET_PRESS_DURATION);
            GP1 = 0;
            timerCount = 0;
        }
    }
    else if(INTF) { // A pin change event occured
        // as such timer is reset:
        TMR1 = TMR1_OFFSET;
        timerCount = 0;
   }
   
   TMR1IF = 0; // reset timer interrupt flag
   INTF = 0;   // reset interrupt pin flag

   TMR1IE = 1; // re-enable timer1 interrupts
   PEIE = 1;
   GIE = 1;
   INTE = 1;   // re-enable pin interrupts

   //enableInterrupts();
}

int main(void) {
    initSys();
    
    GP1 = 1;
    __delay_ms(25);
    GP1 = 0;
            
    initTimer();
    
    for(;;) {
        __delay_ms(195);
    }
}

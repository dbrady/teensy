/* LED Blink Example with USB Debug Channel for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_debug_only.h"
#include "print.h"

#define LED_CONFIG  (DDRD |= (1<<6))
#define LED_ON    (PORTD &= ~(1<<6))
#define LED_OFF   (PORTD |= (1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define DIT 80    /* unit time for morse code */


// Blink the 7 output pins of port D. Looks best if you actually
// connect LEDs to the pins, but that's just me.

// NOTE: If you hook an output LED to D7, it must be disconnected to
// reprogram the device. This is because pin D7 on the chip serves
// double-duty as the reset pin. When D7 is pulled high, the button on
// the device changes meaning from reprogram to reset.

// The circuit:

// Connect a 1k resistor from +5V to LED0+, then LED0- to pin D0.
// Repeat for pins D1 - D7.
// Disconnect LED7 to reprogram.

// Rotate speed in uS
#define SPEED 100000

int main(void)
{
    int i;

    // set for 16 MHz clock, and make sure the LED is off
    CPU_PRESCALE(0);

    // Configure all 8 bits of Port for output
    DDRD = 255;

    // Set all of Port D high (to turn off all LEDs)
    PORTD = 255;

    // initialize the USB, but don't want for the host to
    // configure.  The first several messages sent will be
    // lost because the PC hasn't configured the USB yet,
    // but we care more about blinking than debug messages!
    usb_init();

    while (1) {
        for (i=0; i<256; ++i) {
            PORTD = 255-i;
            _delay_us(SPEED);
        }
    }
}


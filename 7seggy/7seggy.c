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

// NOTE 1: If you hook an output LED to D7, it must be disconnected to
// reprogram the device. This is because pin D7 on the chip serves
// double-duty as the reset pin. When D7 is pulled high, the button on
// the device changes meaning from reprogram to reset.

// NOTE 2: This circuit sort of evolved on the breadboard as I built
// it. There's some stupid mistakes that are left in because it was
// easier than rewiring the whole circuit. For example, having each
// segment share a common resistor means that the current available to
// each segment is determined by the number of segments currently
// turned on. This means that the number 1 will be bright while the
// number 8 will be noticeably dimmer.

// The circuit:

// Note: I'm using a weird pre-1980's calculator display that uses
// multiplexed 7-seg digits. Because of its age, it is common cathode
// instead of the modern common anode devices. If you're using a
// modern device, use buffer/line drivers instead of inverters, and
// swap the rails on the LEDs (connect the common anode to +V instead
// of the common cathode to GND).


// CRAP CRAP CRAP CRAP. I reversed the bits in my circuit. It's easier
// to reprogram in software for now.

// OOOH. Unintentional advantage to reversing the bits in software:
// Pin D7 is now the decimal point, and can be left disconnected (for
// reprogramming) in this circuit.

// Mk I Circuit: 

// Connect each pins 0-7 of port D to inverter inputs, and the
// inverter outputs to the anodes of the 7-seg display. Don't forgit
// bit 7, that's the decimal point.

// The Mk I Circuit does not do digit multiplexing. Connect the common
// cathode of the display to GND through a 240-ohm resistor. Note that
// the common resistor means that available current will fluctuate
// based on the number of lit elements. See the Improvements section.

// Improvements:

// - Multiplexing.
// - Anode resistors.

// Rotate speed in uS
#define SPEED 1000000

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
        int nums[16] = { 
0x3f, // 0: 00111111
0x06, // 1: 00000110
0x5b, // 2: 01011011
0x4f, // 3: 01001111
0x66, // 4: 01100110
0x6d, // 5: 01101101
0x7d, // 6: 01111101
0x07, // 7: 00000111
0x7f, // 8: 01111111
0x6f, // 9: 01101111
0x77, // A: 01110111
0x7c, // b: 01111100
0x58, // c: 01011000
0x5e, // d: 01011110
0x79, // E: 01111001
0x71, // F: 01110001
 };
        for (i=0; i<16; ++i) {
            PORTD = ~nums[i];
            _delay_us(SPEED);
        }
    }
}


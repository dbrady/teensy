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
#include "7segment.h"

#define LED_CONFIG  (DDRD |= (1<<6))
#define LED_ON    (PORTD &= ~(1<<6))
#define LED_OFF   (PORTD |= (1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CYCLE_MIN (-10)
#define CYCLE_MAX (30)
#define CYCLE_DELAY (10)



const unsigned char char_table[];

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

/* ======================================================================
 * TODO:
 * ----------------------------------------------------------------------
 * 
 * 1. Swap out the 240-ohm resistors for 60-ohm resistors. With all
 *    six displays running they are very dim. 60-ohms will make it
 *    brighter, but if only one digit gets turned on, it won't get
 *    burned out, and if all 8 segments are turned on, it won't burn
 *    out the USB port. :-)
 * 
 * 2. Now that I have digit select working, get a multiplexing demo
 *    working.
 * 
 * 3. Refactor. Take the multiplexing  stuff and make it so that
 *    there's separate update and render actions. That way we can set
 *    the string to be displayed, and then play animation games with
 *    it.
 *
 * ====================================================================== */

// Improvements:

// - Multiplexing. I've got the wire, and I've got the available
//   ports. I've even got an extra hex inverter available to use as
//   the line driver. (Though a buffer might have been better to keep
//   the logic cleaner.)
// 
// x Anode resistors. There is a current problem. Heh, see what I did
//   there? "Current" problem? AAAAANYWAY, in a hurry to wire up the
//   circuit, a chose to put a single resistor on the anode. I need to
//   change the circuit to have resistors on each define.
// 
// - Scrolling text. The demo for this is pretty obvious. :-)
// 
// - Duty Cycling! Note: It is essential that anode resistors be wired
//   up first, so that baseline brightness is constant. Otherwise a
//   dim 1 might by the same brightness as a bright 8.
// 
//   - Stage One: Display Cycling. Ramp total display brightness up
//     and down. Amaze your friends!
// 
//     Demo: Set display dark. Put "GEt" in digits 1-3, ramp up to
//     full brightness over 3 seconds. Hold for 2 seconds, then ramp
//     to black over 1 second. Wait 1 second. Put "OUt" in digits 4-6,
//     ramp up to full brightness over 3 seconds, hold 2, ramp down
//     over 1. Wait 3 seconds. Repeat.
// 
//   - Stage Two: Interdigit Duty Cycling. Display separate digits in
//     differing brightnesses.
// 
//     Demo 1: "Flag". Set display dark. Put "dUUUdE" in digits 1-6.
//     Now ramp in the whole word, but stagger each letter. So each
//     digit ramps to full brightness over 2 seconds, holds for 3
//     seconds, and ramps down over 2 seconds, but each digit is
//     staggered 250ms behind the previous one. So at 1s, digit 1 is
//     at 50%, digit 2 is at 37.5%, digit 3 is at 25%, digit 4 is at
//     12.5%, and digits 5 and 6 are off. 
// 
//     Demo 2: "Lotto". Set display dark. Put a random 6-digit number
//     in the digits. For each digit { wait 2 seconds, ramp up the
//     digit to full brightness over 1 second }. Hold the number at
//     full brightness for 10 seconds, then repeat. (Note: Requires a
//     PRNG. LPRNGs are easy, and technically it only needs to do
//     digits 0-9.)
// 
//     Demo 3: "Nixie". Put 000000 in the display. Now start counting
//     up from 0, one per second. At 800ms, begin fading the old
//     number out and the new number in.

// Rotate speed in uS
#define SPEED 500000

void select_digit(int digit) {
    PORTD = 1 << digit;
}

void set_digit(int value) {
    PORTB = ~value;
}

void display_digit(int value, int position) {
    select_digit(position);
    set_digit(value);
}

/* TODO:

- Write display_number, displaying a number between -99999 and 999999

- Ask the system to provide some reflection:

  - Display sizeof(int)

  - Display INT_MAX

  - Display INT_MIN

- Write a RNG

 */

void display_number(unsigned long number) {
    int digit;
    unsigned char code;
    number %= 1000000;

    for (digit=5; digit>=0; --digit) {
        code = pgm_read_byte(char_table + 15 + (number % 10));
        display_digit(code, digit);
        number /= 10;
        _delay_us(CYCLE_DELAY);
    }
}

/* TODO:

- write display_digit(int digit, int position=5)

- write display_string(char* string)

 */

int main(void)
{
    int i;
    int digit;

    // set for 16 MHz clock, and make sure the LED is off
    CPU_PRESCALE(0);

    // Configure all 8 bits of Ports B and D for output
    DDRB = 0xFF;
    DDRD = 0x3F;

    // Set all of Port D high (to turn off all LEDs)
    PORTB = 0xFF;
    PORTD = 0x3F;

    // initialize the USB, but don't want for the host to
    // configure.  The first several messages sent will be
    // lost because the PC hasn't configured the USB yet,
    // but we care more about blinking than debug messages!
    usb_init();

    int chars[36] = {
        DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5,
        DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B,
        DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F, DIGIT_G, DIGIT_H,
        DIGIT_I, DIGIT_J, DIGIT_K, DIGIT_L, DIGIT_M, DIGIT_N,
        DIGIT_O, DIGIT_P, DIGIT_Q, DIGIT_R, DIGIT_S, DIGIT_T,
        DIGIT_U, DIGIT_V, DIGIT_W, DIGIT_X, DIGIT_Y, DIGIT_Z
    };

    digit = 0;

    int message[6] = {
        DIGIT_D, DIGIT_U, DIGIT_U, DIGIT_U, DIGIT_D, DIGIT_E
    };

    int duty_cycle[6] = {
        18, 15, 12, 9, 6, 3, 0
    };
    int cycle_dir[6] = {
        1, 1, 1, 1, 1, 1
    };

    int cycles = 0;

#define MODE_SHOW_STRING
    /* #define MODE_DUUUDE */

    while (1) {
#ifdef MODE_SHOW_STRING
        display_number(987654);
#endif
#ifdef MODE_DUUUDE
        for (int cycle=0; cycle<CYCLE_MAX; ++cycle) {
            for (digit=0; digit<6; ++digit) {
                display_digit( (cycle < duty_cycle[digit]) ? message[digit] : DIGIT_OFF, digit);
                _delay_us(CYCLE_DELAY);
            }
        }
        if (++cycles > 10) {
            cycles = 0;
            for (digit=0; digit<6; ++digit) {
                duty_cycle[digit] += cycle_dir[digit];
                if ((duty_cycle[digit] > CYCLE_MAX && cycle_dir[digit] > 0) ||
                    (duty_cycle[digit] < CYCLE_MIN && cycle_dir[digit] < 0)) {
                    cycle_dir[digit] *= -1;
                }
            }
        }
#endif
    }
}

const unsigned char PROGMEM char_table[] = {
    0x00, /* DIGIT_SPACE */
    0x61, /* DIGIT_BANG */
    0x44, /* DIGIT_QUOTE */
    0x00, /* DIGIT_HASH */
    0x00, /* DIGIT_DOLLAR */
    0x00, /* DIGIT_PERCENT */
    0x00, /* DIGIT_AMP */
    0x40, /* DIGIT_APOS */
    0x70, /* DIGIT_OPAREN */
    0x1c, /* DIGIT_CPAREN */
    0x00, /* DIGIT_PLUS */
    0x30, /* DIGIT_COMMA */
    0x02, /* DIGIT_MINUS */
    0x01, /* DIGIT_PERIOD */
    0x4a, /* DIGIT_SLASH */
    0x3f, /* DIGIT_0 */
    0x06, /* DIGIT_1 */
    0x5b, /* DIGIT_2 */
    0x4f, /* DIGIT_3 */
    0x66, /* DIGIT_4 */
    0x6d, /* DIGIT_5 */
    0x7d, /* DIGIT_6 */
    0x07, /* DIGIT_7 */
    0x7f, /* DIGIT_8 */
    0x6f, /* DIGIT_9 */
    0x00, /* DIGIT_COLON */
    0x00, /* DIGIT_SEMICOLON */
    0x00, /* DIGIT_LT */
    0x48, /* DIGIT_EQUAL */
    0x00, /* DIGIT_GT */
    0xd3, /* DIGIT_QUESTION */
    0x6f, /* DIGIT_AT */
    0x77, /* DIGIT_A */
    0x7c, /* DIGIT_B */
    0x58, /* DIGIT_C */
    0x5e, /* DIGIT_D */
    0x79, /* DIGIT_E */
    0x71, /* DIGIT_F */
    0x3d, /* DIGIT_G */
    0x74, /* DIGIT_H */
    0x06, /* DIGIT_I */
    0x1e, /* DIGIT_J */
    0x72, /* DIGIT_K */
    0x38, /* DIGIT_L */
    0x37, /* DIGIT_M */
    0x54, /* DIGIT_N */
    0x5c, /* DIGIT_O */
    0x73, /* DIGIT_P */
    0x3b, /* DIGIT_Q */
    0x50, /* DIGIT_R */
    0x6d, /* DIGIT_S */
    0x78, /* DIGIT_T */
    0x1c, /* DIGIT_U */
    0x1c, /* DIGIT_V */
    0x7e, /* DIGIT_W */
    0x64, /* DIGIT_X */
    0x6e, /* DIGIT_Y */
    0x5b, /* DIGIT_Z */
};
    

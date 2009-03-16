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

void morse_character(char c);
void morse_P(const char *s);
const unsigned char morse_code_table[];

#define CYCLE_MAX 10000
#define CYCLE_STEP 50
#define CYCLE_FREQ (CYCLE_MAX / CYCLE_STEP)

int main(void)
{
    int duty_cycle = 0;
    int i = 0;
    int cycle_direction = 1;

    // set for 16 MHz clock, and make sure the LED is off
    CPU_PRESCALE(0);
    LED_CONFIG;
    LED_OFF;

    // initialize the USB, but don't want for the host to
    // configure.  The first several messages sent will be
    // lost because the PC hasn't configured the USB yet,
    // but we care more about blinking than debug messages!
    usb_init();

    while (1) {
        i = 0;
        while (++i < 5) {
            LED_ON;
            _delay_us(CYCLE_MAX - (CYCLE_FREQ * duty_cycle));
            LED_OFF;
            _delay_us((CYCLE_FREQ * duty_cycle));
        }
        duty_cycle += cycle_direction;
        if ((cycle_direction == 1 && duty_cycle > CYCLE_STEP ) || 
            (cycle_direction == -1 && duty_cycle < 0 )) {
            cycle_direction *= -1;
        }
    }
}

// blink a single character in Morse code
void morse_character(char c)
{
    unsigned char code, count;

    if (c == ' ') {
        print("Space\n");
        _delay_ms(DIT * 7);
        return;
    }
    if (c < '0' || c > 'Z') {
        print("Opps, unsupported character: ");
        pchar(c);
        print("\n");
        return; 
    } 
    print("Char ");
    pchar(c);
    pchar(':');
    code = pgm_read_byte(morse_code_table + (c - '0'));
    for (count = code & 0x07; count > 0; count--) {
        LED_ON;
        if (code & 0x80) {
            print(" dah");
            _delay_ms(DIT * 3);
        } else {
            print(" dit");
            _delay_ms(DIT);
        }
        LED_OFF;
        _delay_ms(DIT);
        code = code << 1;
    }
    print("\n");
    _delay_ms(DIT * 2);
}

// blink an entire message in Morse code
// the string must be in flash memory (using PSTR macro)
void morse_P(const char *s)
{
    char c;
  
    while (1) {
        c = pgm_read_byte(s++);
        if (!c) break;
        morse_character(c);
    }
    print("\n");
}

// lookup table for all 26 letters.  Upper 5 bits are the pulses
// to send (MSB first), and the lower 3 bits are the number of
// bits to send for this letter.
const unsigned char PROGMEM morse_code_table[] = {
    0xF8 + 5, // 0: ----- 1111 1101
    0x78 + 5, // 1: .---- 0111 1101
    0x38 + 5, // 2: ..--- 0011 1101
    0x18 + 5, // 3: ...-- 0001 1101
    0x08 + 5, // 4: ....- 0000 1101
    0x00 + 5, // 5: ..... 0000 0101
    0x80 + 5, // 6: _.... 1000 0101
    0xC0 + 5, // 7: --... 1100 0101
    0xE0 + 5, // 8: ---.. 1110 0101
    0xF0 + 5, // 9: ----. 1111 0101
    0x00, // :
    0x00, // ;
    0x00, // <
    0x00, // =
    0x00, // >
    0x00, // ?
    0x00, // @
    0x40 + 2, // A: .-
    0x80 + 4, // B: -...
    0xA0 + 4, // C: -.-.
    0x80 + 3, // D: -..
    0x00 + 1, // E: .
    0x20 + 4, // F: ..-.
    0xC0 + 3, // G: --.
    0x00 + 4, // H: ....
    0x00 + 2, // I: ..
    0x70 + 4, // J: .---
    0xA0 + 3, // K: -.-
    0x40 + 4, // L: .-..
    0xC0 + 2, // M: --
    0x80 + 2, // N: -.
    0xE0 + 3, // O: ---
    0x60 + 4, // P: .--.
    0xD0 + 4, // Q: --.-
    0x40 + 3, // R: .-.
    0x00 + 3, // S: ...
    0x80 + 1, // T: -
    0x20 + 3, // U: ..-
    0x10 + 4, // V: ...-
    0x60 + 3, // W: .--
    0x90 + 4, // X: -..-
    0xB0 + 4, // Y: -.--
    0xC0 + 4, // Z: --..
};


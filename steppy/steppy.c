/* throbby.c - make the onboard LED ramp smoothly from glowing to
 * dark. Based on the blink.c example from PJRC.com.
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


/* TODO: Once stepping is working, abstract out direction, windings
   per revolution, then rpm */

#define MOTOR_POLES 4
#define WINDING_DELAY 25000
#define CYCLE_PAUSE 1000000
#define WINDINGS_PER_REVOLUTION 48

/* 1 or -1 */
#define DIRECTION 1

unsigned char next_step(unsigned int s) {
    if (DIRECTION == 1) {
        s <<= 1;
        if (s > (1  << MOTOR_POLES)) {
            s = 0;
        }
    } else {
        if (s == 0) {
            s = 1 << MOTOR_POLES;
        } else {
            s >>= 1;
        }
    }
    return s;
}

int main(void) {
    int i;
    int j;
    /* m = motor windings; n = display */
    unsigned char m = 0;
    unsigned char n = 0;
    

    // set for 16 MHz clock, and make sure the LED is off
    CPU_PRESCALE(0);
    LED_CONFIG;
    LED_OFF;

    DDRD = 0x0F;
    DDRB = 0xF0;
    PORTD = 0;

    // initialize the USB, but don't want for the host to
    // configure.  The first several messages sent will be
    // lost because the PC hasn't configured the USB yet,
    // but we care more about blinking than debug messages!
    usb_init();

    while (1) {
        for(j=0; j<WINDINGS_PER_REVOLUTION/4; ++j) {
            for(i=0; i<4; ++i) {
                m = next_step(m);
                n = ~(1 << (7-m));
                PORTD = m;
                PORTB = n;
                _delay_us(WINDING_DELAY);
            }
        }
        _delay_us(CYCLE_PAUSE);
    }
}

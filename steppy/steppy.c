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

#define WINDING_DELAY 25000
#define CYCLE_PAUSE 1000000
#define WINDINGS_PER_REVOLUTION 48

int main(void)
{
    int i;
    int j;

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
                PORTD = 1 << i;
                PORTB = ~(1 << (7-i));
                _delay_us(WINDING_DELAY);
            }
        }
        _delay_us(CYCLE_PAUSE);
/*         while (++i < 5) { */
/*             LED_ON; */
/*             _delay_us(CYCLE_MAX - (CYCLE_FREQ * duty_cycle)); */
/*             LED_OFF; */
/*             _delay_us((CYCLE_FREQ * duty_cycle)); */
/*         } */
/*         duty_cycle += cycle_direction; */
/*         if ((cycle_direction == 1 && duty_cycle > CYCLE_STEP ) ||  */
/*             (cycle_direction == -1 && duty_cycle < 0 )) { */
/*             cycle_direction *= -1; */
/*         } */
    }
}

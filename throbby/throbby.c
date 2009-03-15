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
#define DIT 80    /* unit time for morse code */

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

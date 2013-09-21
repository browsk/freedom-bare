#ifndef _DELAY_H
#define _DELAY_H

/**
 * delay for the specified number of milliseconds
 *
 * @param delay_val The number of milliseconds to delay for
 *
 */
void delay_ms(int delay_val);

/**
 * initialise the timer (PIT)
 *
 */
void init_timer();

#endif

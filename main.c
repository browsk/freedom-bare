#include <stdio.h>
#include "freedom.h"
#include "uart.h"

extern void timer_init();
//extern void init_led_io(void);
extern void enable_interrupts();
extern void delay_ms(int val);

int main()
{
    int r = 0;
    int g = 100;
    int b = 0;
    int temp = 1;

    enable_interrupts();

    timer_init();

//  RGB_LED(0,0,0);

    delay_ms(2000);
    RGB_LED(100, 0, 0);

    uart0_write_string("blah");
    uart0_write_string("blah\r\n");

    for (;;) {
	delay_ms(1000);
	RGB_LED(r, g, b);
	temp = r;
	r = b;
	b = g;
	g = temp;
	uart0_write_string
	    ("123456789012345678901234567890123456789012345678901234567890\n\r");
	uart0_write_string("1234567890\r\n");
	uart0_write_string("blah\r\n");
    }
}

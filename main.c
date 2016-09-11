#include <stdio.h>
#include <stdbool.h>
#include "freedom.h"
#include "uart.h"

extern void timer_init();
//extern void init_led_io(void);
extern void enable_interrupts();
extern void delay_ms(int val);
extern void start_tasks();

extern uint8_t read_device_id();
extern void i2c_init();

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

bool command_avail = false;
static char rx_buf[128];

void read_callback(char c)
{
    static uint8_t rx_buf_count = 0;

    uart0_write_char(c);
    
    if (rx_buf_count == ARRAYSIZE(rx_buf) - 2)
    {
	rx_buf_count = 0;
	uart0_write_string("overflow\n");
    }

    rx_buf[rx_buf_count++] = c;

    if (c == '\n' || c == '\r')
    {
	if (c == '\r')
	    rx_buf[rx_buf_count++] = '\n';
	
	rx_buf[rx_buf_count] = '\0';
	rx_buf_count = 0;

	command_avail = true;
    }
}

int main()
{
    int r = 0;
    int g = 100;
    int b = 0;
    int temp = 1;

    enable_interrupts();

    timer_init();
    i2c_init();

//  RGB_LED(0,0,0);

    delay_ms(2000);
    RGB_LED(100, 0, 0);

    uart0_register_read_callback(read_callback);
    
    uart0_write_string("blah");
    uart0_write_string("blah bye...\r\n");

    //    char buf[80];
//    start_tasks();

    for (;;) {
        delay_ms(100);
        RGB_LED(r, g, b);
        temp = r;
        r = b;
        b = g;
        g = temp;

	//        uint8_t id = read_device_id();
	//        sprintf(buf, "\nID:%u\n\r", (unsigned)id);
	//        uart0_write_string(buf);

	//        uart0_write_string
	//            ("123456789012345678901234567890123456789012345678901234567890\n\r");
	//        uart0_write_string("---------\r\n");
	//        uart0_write_string("blah\r\n");

	if (command_avail)
	{
	    uart0_write_string("Got: ");
	    uart0_write_string(rx_buf);
	    command_avail = false;
	}
    }
}

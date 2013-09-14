#include <stdio.h>
#include "freedom.h"
#include "uart.h"

extern void init_led_io(void);

void main()
{
  int i;
  int r = 0;
  int g = 100;
  int temp = 1;

  RGB_LED(100,0,0);

  uart0_write_string("blah\n\r");

  printf("hello");
	for (;;)
  {
    for (i = 0; i < 100000000 && temp > 0 ;i++)
      temp = temp * 2;

    temp = r;
    r = g;
    g = temp;
    RGB_LED(0,100,100);
    //printf("0");
  }
}

#ifndef _UART_H
#define _UART_H

extern void uart0_init();

extern void uart0_write_char(char c);

extern void write_string(const char *str);
#endif

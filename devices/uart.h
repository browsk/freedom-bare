#ifndef _UART_H
#define _UART_H

#include "buffer.h"

extern void uart0_init();

extern void uart0_write_char(char c);

extern void uart0_write_string(const char *str);

#endif

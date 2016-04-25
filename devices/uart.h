#ifndef _UART_H
#define _UART_H

#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void uart0_init();

extern void uart0_write_char(char c);

extern void uart0_write_string(const char *str);

#ifdef __cplusplus
}
#endif

#endif

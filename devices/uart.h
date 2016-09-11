#ifndef _UART_H
#define _UART_H

#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern void uart0_init();

    extern void uart0_write_char(char c);

    extern void uart0_write_string(const char *str);

    extern char uart0_read_char();

    typedef void(*read_callback_t)(const char);

    extern void uart0_register_read_callback(read_callback_t callback);
    
#ifdef __cplusplus
}
#endif

#endif

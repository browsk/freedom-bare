#include "MKL25Z4.h"
#include "freedom.h"
#include "core_cm0plus.h"

#include "uart.h"
#include <string.h>

struct ring_buffer tx_buf;

void __attribute__ ((interrupt)) uart0_isr()
{
    if (is_empty(&tx_buf)) {
	UART0->C2 &= ~UART0_C2_TCIE_MASK;
    } else {
	uint8_t c;
	if (get_byte(&tx_buf, &c))
	    UART0->D = c;
    }
}

void uart0_write_string(const char *str)
{
    int count = strlen(str);

    while (count > 0) {
	count -= add_bytes(&tx_buf, (const uint8_t *) str, count);
	UART0->C2 |= UART0_C2_TCIE_MASK;
    }
}

void uart0_write_char(char c)
{
    uint8_t ready_mask = UART_S1_TDRE_MASK;	// |
    // UART_S1_TC_MASK;
    while ((UART0->S1 & ready_mask) != ready_mask);

    UART0->D = c;
}

void enable_interrupts(void)
{
    // Configure NVIC
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn, 3);
    NVIC_EnableIRQ(UART0_IRQn);
}

void uart0_init()
{
    int baud_rate = 115200;

    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    // set UART0 clock source to MCGFLLCLK
    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    // enable clock to UART0
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

    // Set PTA1 and PTA2 to ALT2 to enable UART0 TX and RX pins
    PORTA->PCR[1] = PORT_PCR_MUX(2);
    PORTA->PCR[2] = PORT_PCR_MUX(2);

    RGB_LED(100, 0, 0);

    UART0->C1 = 0;
    // disable TX and RX
    UART0->C2 = 0;
    // all interrupts disabled - standard operation
    UART0->C3 = 0;

    UART0->S2 = 0;

    // oversampling ratio set to 4
    const uint8_t osr = 16;
    UART0->C4 |= UARTLP_C4_OSR(osr - 1);

    uint16_t divisor =
	(int16_t) (((float) MCGFLLCLK / osr) / (float) baud_rate);

    UART0->BDH = (divisor >> 8) & UARTLP_BDH_SBR_MASK;
    UART0->BDL = (divisor & UARTLP_BDL_SBR_MASK);

    RGB_LED(100, 100, 100);

    // enable TX and RX
    UART0->C2 = UART_C2_TE_MASK;	// | UART_C2_RE_MASK;

    RGB_LED(100, 0, 100);

    uart0_write_char('@');

    RGB_LED(100, 100, 100);

    init_ring_buffer(&tx_buf);
    // enable_interrupts();
}

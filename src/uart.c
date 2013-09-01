#include "MKL25Z4.h"
#include "freedom.h"
#include "uart.h"

void uart0_write_string(const char *str)
{
  char c;

  while(c = *str++)
    uart0_write_char(c);
}

void uart0_write_char(char c)
{
  uint8_t ready_mask = UART_S1_TDRE_MASK;// | UART_S1_TC_MASK;
  while((UART0_S1 & ready_mask) != ready_mask);

  UART0_D = c;
}

void uart0_init()
{
  int baud_rate = 115200;

  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

  // set UART0 clock source to MCGFLLCLK
  SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
  SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);  
  // enable clock to UART0
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK; 

  // Set PTA1 and PTA2 to ALT2 to enable UART0 TX and RX pins
  PORTA_PCR1 = PORT_PCR_MUX(2);
  PORTA_PCR2 = PORT_PCR_MUX(2);

  RGB_LED(100,0,0);

  UART0_C1 = 0;
  // disable TX and RX
  UART0_C2 = 0;
  // all interrupts disabled - standard operation
  UART0_C3 = 0;
      
  UART0_S2 = 0;   

  // oversampling ratio set to 4
  const uint8_t osr = 16;
  UART0_C4 |= UARTLP_C4_OSR(osr - 1);

  uint16_t divisor = (int16_t)(((float)MCGFLLCLK / osr) / (float)baud_rate);

  UART0_BDH = (divisor >> 8) & UARTLP_BDH_SBR_MASK;
  UART0_BDL = (divisor & UARTLP_BDL_SBR_MASK);

  RGB_LED(100,100,100);

  // enable TX and RX
  UART0_C2 = UART_C2_TE_MASK;// | UART_C2_RE_MASK;

  RGB_LED(100,0,100);

  uart0_write_char('@');

  RGB_LED(100,100,100);

}

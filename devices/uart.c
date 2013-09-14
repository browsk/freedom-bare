#include "MKL25Z4.h"
#include "freedom.h"
#include "uart.h"

#include "core_cm0plus.h"

void __attribute__ ((interrupt)) uart0_isr()
{
  uart0_write_char('*');
}

void uart0_write_string(const char *str)
{
  char c;

  while(c = *str++)
    uart0_write_char(c);
}

void uart0_write_char(char c)
{
  uint8_t ready_mask = UART_S1_TDRE_MASK;// | UART_S1_TC_MASK;
  while((UART0->S1 & ready_mask) != ready_mask);

  UART0->D = c;
}

static void enable_interrupts(void)
{
  // Configure NVIC
/*
  NVIC_EnableIRQ(12);
  // disable pending interrupts
  NVIC_ICPR |= (1 << 12);
  // set priority
  NVIC_IPR3 |= NVIC_IP_PRI_12(3);

  NVIC_ISER |= (1 << 12);
*/
  // configure UART0 interrupts


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

  RGB_LED(100,0,0);

  UART0->C1 = 0;
  // disable TX and RX
  UART0->C2 = 0;
  // all interrupts disabled - standard operation
  UART0->C3 = 0;
      
  UART0->S2 = 0;   

  // oversampling ratio set to 4
  const uint8_t osr = 16;
  UART0->C4 |= UARTLP_C4_OSR(osr - 1);

  uint16_t divisor = (int16_t)(((float)MCGFLLCLK / osr) / (float)baud_rate);

  UART0->BDH = (divisor >> 8) & UARTLP_BDH_SBR_MASK;
  UART0->BDL = (divisor & UARTLP_BDL_SBR_MASK);

  RGB_LED(100,100,100);

  // enable TX and RX
  UART0->C2 = UART_C2_TE_MASK;// | UART_C2_RE_MASK;

  RGB_LED(100,0,100);

  uart0_write_char('@');

  RGB_LED(100,100,100);

  enable_interrupts();
}

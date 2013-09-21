#include "MKL25Z4.h"
#include "freedom.h"
#include "core_cm0plus.h"

extern void uart0_write_string(const char *str);

extern void uart0_write_char(char c);

static volatile int delay_val = 0;

void pit_handler()
{
  PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
  delay_val--;
}

void timer_init()
{
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
  PIT->MCR = 0;
  PIT->CHANNEL[0].LDVAL = 24000 - 1;
  PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;

  // Configure NVIC
  NVIC_ClearPendingIRQ(PIT_IRQn);
  NVIC_SetPriority(PIT_IRQn, 1);
  NVIC_EnableIRQ(PIT_IRQn);
  RGB_LED(0,0,100);
}

void delay_ms(int val)
{
  delay_val = val;

  while(delay_val != 0);
}

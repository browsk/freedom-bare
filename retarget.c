#include "uart.h"
#include "MKL25Z4.h"

extern void init_led_io();

void _ttywrch(int ch);
static void init_clocks(void)
{   
    // Enable clock gate to Port A module to enable pin routing (PORTA=1)
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    // Divide-by-2 for clock 1 and clock 4 (OUTDIV1=1, OUTDIV4=1)   
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x01) | SIM_CLKDIV1_OUTDIV4(0x01);

    // System oscillator drives 32 kHz clock for various peripherals (OSC32KSEL=0)
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL(0x03);

    // Select PLL as a clock source for various peripherals (PLLFLLSEL=1)
    // Clock source for TPM counter clock is MCGFLLCLK or MCGPLLCLK/2
    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM->SOPT2 = (SIM->SOPT2 & ~(SIM_SOPT2_TPMSRC(0x02))) | SIM_SOPT2_TPMSRC(0x01);
                  
    /* PORTA_PCR18: ISF=0,MUX=0 */
    /* PORTA_PCR19: ISF=0,MUX=0 */            
    PORTA->PCR[18] &= ~(PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07));
    PORTA->PCR[19] &= ~(PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07));                                                   
    /* Switch to FBE Mode */
    
    /* OSC0_CR: ERCLKEN=0,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
    OSC0->CR = 0;                                                   
    /* MCG_C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
    MCG->C2 = (MCG_C2_RANGE0(0x02) | MCG_C2_EREFS0_MASK);
    /* MCG->C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
    MCG->C1 = (MCG_C1_CLKS(0x02) | MCG_C1_FRDIV(0x03));
    /* MCG->C4: DMX32=0,DRST_DRS=0 */
    MCG->C4 &= ~((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));
    /* MCG->C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=1 */
    MCG->C5 = MCG_C5_PRDIV0(0x01);                                                   
    /* MCG->C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
    MCG->C6 = 0;
    
    // Check that the source of the FLL reference clock is 
    // the external reference clock.
    while((MCG->S & MCG_S_IREFST_MASK) != 0)
        ;

    while((MCG->S & MCG_S_CLKST_MASK) != 8)      // Wait until external reference
        ;
    
    // Switch to PBE mode
    //   Select PLL as MCG source (PLLS=1)
    MCG->C6 = MCG_C6_PLLS_MASK;
    while((MCG->S & MCG_S_LOCK0_MASK) == 0)      // Wait until PLL locked
        ;
    
    // Switch to PEE mode
    //    Select PLL output (CLKS=0)
    //    FLL external reference divider (FRDIV=3)
    //    External reference clock for FLL (IREFS=0)
    MCG->C1 = MCG_C1_FRDIV(0x03);
    while((MCG->S & MCG_S_CLKST_MASK) != 0x0CU)  // Wait until PLL output
        ;
}


void retarget_init()
{
  init_clocks();

  init_led_io();
  // Initialize UART
  uart0_init();
}

int _write (int fd, char *ptr, int len)
{
  uart0_write_char('$');
  /* Write "len" of char from "ptr" to file id "fd"
   * Return number of char written.
   * Need implementing with UART here. */
  int n = 0;

  while(n++ != len)
  {
    _ttywrch(*ptr++);
  }
  return n;
}

int _read (int fd, char *ptr, int len)
{
  /* Read "len" of char to "ptr" from file id "fd"
   * Return number of char read.
   * Need implementing with UART here. */
  return len;
}

void _ttywrch(int ch) {
  /* Write one char "ch" to the default console
   * Need implementing with UART here. */
  uart0_write_char(ch);
}

/* SystemInit will be called before main */
void SystemInit()
{
    SIM->COPC = 0; 
    retarget_init();
}


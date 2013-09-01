#include "MKL25Z4.h"
#include "freedom.h"

void init_led_io(void)
{
    // Turn on clock gating to PortB module (red and green LEDs) and 
    // PortD module (blue LED)  
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

    PORTB_PCR18 = PORT_PCR_MUX(3);  // TPM2_CH0 enable on PTB18 (red)
    PORTB_PCR19 = PORT_PCR_MUX(3);  // TPM2_CH1 enable on PTB19 (green)
    PORTD_PCR1  = PORT_PCR_MUX(4);  // TPM0_CH1 enable on PTD1  (blue)

    RGB_LED(0,0,0);                 // Off
    
    TPM0_MOD  = 99;
    TPM0_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2_MOD  = 99;
    TPM2_C0SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

    TPM2_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
    TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}


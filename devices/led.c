#include "MKL25Z4.h"
#include "freedom.h"

void init_led_io(void)
{
    // Turn on clock gating to PortB module (red and green LEDs) and 
    // PortD module (blue LED) 
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    PORTB->PCR[18] = PORT_PCR_MUX(3);	// TPM2_CH0 enable on PTB18 (red)
    PORTB->PCR[19] = PORT_PCR_MUX(3);	// TPM2_CH1 enable on PTB19
    // (green)
    PORTD->PCR[1] = PORT_PCR_MUX(4);	// TPM0_CH1 enable on PTD1 (blue)

    RGB_LED(0, 0, 0);		// Off

    TPM0->MOD = 99;
    TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2->MOD = 99;
    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

    TPM2->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);	/* Edge Aligned PWM
						 * running from BUSCLK / 1 
						 */
    TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);	/* Edge Aligned PWM
						 * running from BUSCLK / 1 
						 */
}

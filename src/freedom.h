//
// freedom.h -- Definitions for Freescale Freedom development board
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//

#include "MKL25Z4.h"                    // CPU definitions

#define CORE_CLOCK          48000000    // Core clock speed

#define MCGFLLCLK   48000000

static inline void RGB_LED(int red, int green, int blue) {
    TPM2->CONTROLS[0].CnV  = red;
    TPM2->CONTROLS[1].CnV  = green;
    TPM0->CONTROLS[1].CnV  = blue;
}

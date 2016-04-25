#pragma once

#include "freedom.h"

typedef struct {
    int16_t X;
    int16_t Y;
    int16_t Z;
} Accel;

Accel GetAccelerations();

    .thumb
    .syntax unified

    .text

    .global  context_switch

context_switch:
    stm     r1!, {r3-r12}
    str     sp, [r0,#0]
    ldm     r2!, {r3-r12}
    ldr     sp, [r1,#0]
    mov     pc, lr

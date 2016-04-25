#include "MKL25Z4.h"
#include "freedom.h"
#include "uart.h"

#define ADDR (0x1d << 1)
#define I2C_READ  1
#define I2C_WRITE 0

void i2c_wait() {
    while((I2C0->S & I2C_S_IICIF_MASK) == 0);
    I2C0->S |= I2C_S_IICIF_MASK;
}

void i2c_init()
{
    // Enable clocks
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;

    // Configure GPIO for I2C
    PORTE->PCR[24] = PORT_PCR_MUX(5);
    PORTE->PCR[25] = PORT_PCR_MUX(5);

    I2C0->F = I2C_F_MULT(1) | I2C_F_ICR(7);
    I2C0->C1 = I2C_C1_IICEN_MASK;// | I2C_C1_MST_MASK;
}

void write_byte(uint8_t byte)
{
    I2C0->D = byte;
    i2c_wait();
}

uint8_t read_byte()
{
    uint8_t val = I2C0->D;
    (void)val;

    i2c_wait();

    return I2C0->D;
}

uint8_t read_device_id()
{
    uart0_write_char('0');
    uart0_write_char('+');

    I2C0->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;

    uart0_write_char('1');

    // send device address
    write_byte(ADDR | I2C_WRITE);

    uart0_write_char('2');

    write_byte(0x0D);

    I2C0->C1 |= I2C_C1_RSTA_MASK;

    write_byte(ADDR | I2C_READ);

    I2C0->C1 &= ~I2C_C1_TX_MASK;
    I2C0->C1 |= I2C_C1_TXAK_MASK;

    uint8_t val = I2C0->D;
    (void)val;

    while((I2C0->S & I2C_S_IICIF_MASK) == 0);
    I2C0->S |= I2C_S_IICIF_MASK;

    I2C0->C1 = I2C_C1_IICEN_MASK;

    return I2C0->D;
}

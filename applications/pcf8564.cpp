#include "soft_iic.hpp"
#include <rtthread.h>

#define PCF_ADDR 0x20
#define PCF_SDA_PIN GET_PIN(B, 3)
#define PCF_SCL_PIN GET_PIN(B, 4)
soft_iic pcf;

void pcf_init()
{
    pcf = soft_iic(
        PCF_SDA_PIN,
        PCF_SCL_PIN,
        PCF_ADDR);
    pcf.init();
}

void pcf_write(uint8_t data)
{
    pcf.write_data(data);
}

uint8_t pcf_read()
{
    return pcf.read_char();
}
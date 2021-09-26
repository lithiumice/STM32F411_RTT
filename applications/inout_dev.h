#ifndef __INOUT_DEV__
#define __INOUT_DEV__ 1
#include "my_def.h"

void pcf_init();
void pcf_write(uint8_t data);
uint8_t pcf_read();

void mcp_digit_set(uint16_t value);
void mcp_init();

void ntc_init();
int ntc_get_temp();

#endif // __INOUT_DEV__
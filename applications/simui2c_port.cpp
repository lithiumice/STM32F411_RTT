
// void PinOutInitSCL()
// {
//     rt_pin_mode(SCL_PIN, PIN_MODE_OUTPUT);
// }
// void PinOutInitSDA()
// {
//     rt_pin_mode(SDA_PIN, PIN_MODE_OUTPUT);
// }
// void PinInInitSDA()
// {
//     rt_pin_mode(SDA_PIN, PIN_MODE_INPUT);
// }
// void PinSetSCL(unsigned char value)
// {
//     rt_pin_write(SCL_PIN, value);
// }
// void PinSetSDA(unsigned char value)
// {
//     rt_pin_write(SDA_PIN, value);
// }
// unsigned char PinGetSDA()
// {
//     return rt_pin_read(SDA_PIN);
// }
// void Delayus(unsigned int value)
// {
//     rt_hw_us_delay(value);
// }

// SimuI2C sim_iic = {
//     PinOutInitSCL,
//     PinOutInitSDA,
//     PinInInitSDA,
//     PinSetSCL,
//     PinSetSDA,
//     PinGetSDA,
//     Delayus,
//     7};

// SimuI2C_Init(&sim_iic);
// rt_thread_mdelay(100);
// SimuI2C_Start(&sim_iic);
// SimuI2C_WriteByte(&sim_iic, (0x20 << 1) | 0x00);
// SimuI2C_WaitAck(&sim_iic);
// SimuI2C_WriteByte(&sim_iic, 0x55);
// SimuI2C_WaitAck(&sim_iic);
// SimuI2C_Stop(&sim_iic);

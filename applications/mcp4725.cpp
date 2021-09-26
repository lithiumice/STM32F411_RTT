#include "soft_iic.hpp"
#include <rtthread.h>
#include "inout_dev.h"

#define MCP_SDA_PIN GET_PIN(B, 5)
#define MCP_SCL_PIN GET_PIN(B, 6)
#define MCP4725_ADDR 0x60
#define MCP4726_CMD_UPDATEEDAC (64)
#define MCP4726_CMD_WRITEDAC (0x40)
#define MCP4726_CMD_WRITEDACEEPROM (0x60)
#define VREF 3.30

soft_iic mcp;

void mcp_init()
{
    mcp = soft_iic(
        MCP_SDA_PIN,
        MCP_SCL_PIN,
        MCP4725_ADDR);
    mcp.init();
}

void mcp_set(uint16_t value)
{
    mcp.start();
    mcp.send_char(0xC0);
    mcp.send_char(MCP4726_CMD_UPDATEEDAC);
    mcp.send_char((value) >> 4);
    // the 8 most significant bits...
    mcp.send_char((0xFF & value) << 4);
    // the 4 least significant bits...
    mcp.stop();
}

//voltage: 0-3.3V
void mcp_vol_set(float voltage)
{
    mcp_digit_set(voltage * 4096 / VREF);
}

//value: 0-4096
void mcp_digit_set(uint16_t value)
{
    mcp.start();
    mcp.send_char(0xC0);
    mcp.send_char((0x0F00 & value) >> 8);
    mcp.send_char((0x00FF & value));
    mcp.stop();
}

//value: 0-4096
void mcp_digit_set(uint16_t value, bool writeToEEPROM)
{
    // uint8_t data[3] = {
    //     writeToEEPROM ? MCP4726_CMD_WRITEDACEEPROM : MCP4726_CMD_WRITEDAC,
    //     value / 16,
    //     (value % 16) << 4};
    mcp.start();
    mcp.send_char(0xC0);

    if (writeToEEPROM)
        mcp.send_char(MCP4726_CMD_WRITEDACEEPROM);
    else
        mcp.send_char(MCP4726_CMD_WRITEDAC);

    mcp.send_char(value / 16);
    mcp.send_char((value % 16) << 4);
    mcp.stop();
}

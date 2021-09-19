#ifndef _SPI_H_
#define _SPI_H_
#include <rtthread.h>
#include <drv_gpio.h>
#include "my_def.h"

#define MOSI_PIN GET_PIN(A, 7) //SDI
#define SCLK_PIN GET_PIN(A, 5) //SCK

#define CS_PIN GET_PIN(B, 2)
#define RS_PIN GET_PIN(B, 0) //DC
#define RST_PIN GET_PIN(B, 1)

#define SPI_MOSI_SET rt_pin_write(MOSI_PIN, PIN_HIGH)
#define SPI_SCLK_SET rt_pin_write(SCLK_PIN, PIN_HIGH)

#define SPI_MOSI_CLR rt_pin_write(MOSI_PIN, PIN_LOW)
#define SPI_SCLK_CLR rt_pin_write(SCLK_PIN, PIN_LOW)

#define LCD_CS_SET rt_pin_write(CS_PIN, PIN_HIGH)
#define LCD_RS_SET rt_pin_write(RS_PIN, PIN_HIGH)
#define LCD_RST_SET rt_pin_write(RST_PIN, PIN_HIGH)

#define LCD_CS_CLR rt_pin_write(CS_PIN, PIN_LOW)
#define LCD_RS_CLR rt_pin_write(RS_PIN, PIN_LOW)
#define LCD_RST_CLR rt_pin_write(RST_PIN, PIN_LOW)

#define delay_ms(x) rt_thread_mdelay(x)

void SPIv_WriteData(u8 Data);
void SPIv_Init(void);

extern DMA_HandleTypeDef hdma_spi1_tx;
extern SPI_HandleTypeDef hspi1;

#endif

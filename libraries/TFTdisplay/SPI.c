#include "SPI.h"
#include "main.h"
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
I2C_HandleTypeDef hi2c1;

static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

u8 SPI1_ReadWriteByte(u8 TxData)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&hspi1, &TxData, &Rxdata, 1, 1000);
  return Rxdata;
}

void SPIv_WriteData(u8 TxData)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&hspi1, &TxData, &Rxdata, 1, 1000);
}

// void SPIv_WriteData(u8 Data)
// {
//   unsigned char i = 0;
//   for (i = 8; i > 0; i--)
//   {
//     if (Data & 0x80)
//       SPI_MOSI_SET;
//     else
//       SPI_MOSI_CLR;

//     SPI_SCLK_CLR;
//     SPI_SCLK_SET;
//     Data <<= 1;
//   }
// }

void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
  __HAL_SPI_DISABLE(&hspi1);
  hspi1.Instance->CR1 &= 0XFFC7;
  hspi1.Instance->CR1 |= SPI_BaudRatePrescaler;
  __HAL_SPI_ENABLE(&hspi1);
}

void SPIv_Init(void)
{
  // rt_pin_mode(MOSI_PIN, PIN_MODE_OUTPUT);
  // rt_pin_mode(SCLK_PIN, PIN_MODE_OUTPUT);
  MX_SPI1_Init();
  MX_DMA_Init();

  rt_pin_mode(CS_PIN, PIN_MODE_OUTPUT);
  rt_pin_mode(RS_PIN, PIN_MODE_OUTPUT);
  rt_pin_mode(RST_PIN, PIN_MODE_OUTPUT);
}

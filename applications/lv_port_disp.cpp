#include "lvgl/lvgl.h"
#include "lv_port.h"

#define DISP_BUF_SIZE (LV_HOR_RES_MAX * LV_VER_RES_MAX / 2)

static lv_disp_drv_t *disp_drv_p;
static lv_color_t lv_disp_buf1[DISP_BUF_SIZE];
static lv_color_t lv_disp_buf2[DISP_BUF_SIZE];

static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    disp_drv_p = disp;
    int16_t w = (area->x2 - area->x1 + 1);
    int16_t h = (area->y2 - area->y1 + 1);
    uint32_t size = w * h * sizeof(lv_color_t);

    LCD_SetWindows(area->x1, area->y1, area->x2, area->y2);

    // digitalWrite_LOW(TFT_CS_Pin);
    // digitalWrite_HIGH(TFT_DC_Pin);
    rt_pin_write(CS_PIN, PIN_LOW);
    rt_pin_write(RS_PIN, PIN_HIGH);

    // HAL_SPI_Transmit_DMA(&hspi1, (u8 *)color_p, size);

    HAL_SPI_Transmit(&hspi1, (u8 *)color_p, size, 1000);

    // LCD_SetPixel(x, y, (*((u16 *)color_p)));
    // color_p++;

    rt_pin_write(CS_PIN, PIN_HIGH);
    lv_disp_flush_ready(disp_drv_p);
}

void lv_port_disp_init()
{
    // disp_spi_dma_init();
    // __HAL_RCC_DMA2_CLK_ENABLE();
    // HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, lv_disp_buf1, lv_disp_buf2, DISP_BUF_SIZE);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}

extern "C"
{
    void SPI1_IRQHandler(void)
    {
        HAL_SPI_IRQHandler(&hspi1);
    }

    void DMA2_Stream3_IRQHandler(void)
    {
        HAL_DMA_IRQHandler(&hdma_spi1_tx);

        // if (__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TCIF2_6) != RESET)
        // {
        //     rt_pin_write(CS_PIN, PIN_HIGH);
        //     lv_disp_flush_ready(disp_drv_p); /* tell lvgl that flushing is done */
        //     __HAL_DMA_CLEAR_FLAG(&hdma_spi1_tx, DMA_FLAG_TCIF2_6);
        // }
    }

    void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
    {
        // HAL_SPI_Receive_DMA(&hspi1, recv_buf, 5);
        rt_pin_write(CS_PIN, PIN_HIGH);
        lv_disp_flush_ready(disp_drv_p); /* tell lvgl that flushing is done */
    }

    void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
    {
        // HAL_SPI_Receive_DMA(&hspi1, recv_buf, 5);
        rt_pin_write(CS_PIN, PIN_HIGH);
        lv_disp_flush_ready(disp_drv_p); /* tell lvgl that flushing is done */
    }
}

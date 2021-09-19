/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string>
#include "util.h"

#include "GUI.h"
#include "LCD.h"
#include "lvgl/lvgl.h"
#include "lv_port.h"

// #include "lv_examples/lv_tutorial/lv_tutorial_hello_world.h"
// #include "lv_tutorial_hello_world.h"
#include "benchmark.h"

using namespace std;

#define LED0_PIN GET_PIN(C, 13)

uint32_t get_enc();
void encoder_init();
uint32_t enc1;

int main(void)
{
    int count = 1;
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    encoder_init();

    LCD_Init();
    LCD_ShowString(0, 0, 12, (u8 *)"hello", 1);
    std::string tmp = "enc:";

    lv_init();
    lv_port_disp_init();
    // lv_tutorial_hello_world();
    benchmark_create();
    benchmark_start();

    while (1)
    {
        // lv_timer_handler();
        lv_task_handler();
        lv_tick_inc(5);
    }

    while (count++)
    {
        enc1 = get_enc();
        // LCD_ShowString(8, 0, 12, (u8 *)(tmp + std::to_string(enc1)).data(), 1);
        // LCD_ShowString(0, 12, 12, (u8 *)(tmp + abs(enc1) + "   ").data(), 0);
        rt_pin_write(LED0_PIN, PIN_HIGH);
        // rt_thread_mdelay(200);
        rt_pin_write(LED0_PIN, PIN_LOW);
        // rt_thread_mdelay(200);
    }

    return RT_EOK;
}

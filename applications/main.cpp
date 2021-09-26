#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string>
#include "util.h"

#include "lvgl/lvgl.h"
#include "lv_port.h"

#include "tasks_manage.h"
#include "sys_config.h"
#include "inout_dev.h"

// #include "lv_examples/lv_tutorial/lv_tutorial_hello_world.h"
// #include "lv_tutorial_hello_world.h"
#include "benchmark.h"

using namespace std;

#define LED0_PIN GET_PIN(C, 13)

uint32_t get_enc();
void encoder_init();
uint32_t enc1;

void display_thread(void *param)
{
    while (1)
    {
        //enc1 = get_enc();
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
int main(void)
{

    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    encoder_init();
    LCD_Init();
    pcf_init();
    mcp_init();
    ntc_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    // lv_tutorial_hello_world();
    benchmark_create();
    benchmark_start();

    xTaskReg(display_thread, ByteUnit(512), RT_MAIN_THREAD_PRIORITY);

    while (1)
    {
        lv_task_handler();
        // lv_tick_inc(10);
    }

    return RT_EOK;
}

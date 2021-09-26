#include "rtthread.h"
#include "sys_config.h"

void init()
{
    rt_kprintf(
        "\r\n+=================== WatchX OS =====================+\r\n"
        "@FreeRTOS Kernel "
        "tskKERNEL_VERSION_NUMBER"
        "-Hardware:" WX_HARDWARE_VERSION "\r\n"
        "-Software:" WX_SOFTWARE_VERSION "\r\n"
        " " WX_NAME
        "(" __DATE__
        " " __TIME__
        " Build) init...\r\n");
}
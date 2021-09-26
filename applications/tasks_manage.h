#ifndef __TASKSMANAGE_H
#define __TASKSMANAGE_H

// #include "FreeRTOS.h"
// #include "task.h"
// #include "timers.h"
// #include "semphr.h"
// #include "queue.h"
// #include "event_groups.h"
// #include "cpu_utils.h"

/*Ext Function*/
#define TSK_DEBUG_PRINTF(format, ...) rt_kprintf(format, ##__VA_ARGS__)
#define KByte2Word(kb) ((int)(kb * 1024 / 4))
#define Kb2Byte(b) ((int)(b * 1024))
#define Byte2Kb(b) ((int)(b / 1024))
#define ByteUnit(b) (b)

#define uxTaskGetFreeStackByte(TaskHandle_x) (uxTaskGetStackHighWaterMark(TaskHandle_x) * 4)

//task: func name to loop
#define xTaskReg(task, stack, priority)                                                         \
    do                                                                                          \
    {                                                                                           \
        rt_thread_t tid_##task = rt_thread_create(#task, task, RT_NULL, stack, priority, 5);    \
        if (tid_##task != NULL)                                                                 \
            rt_thread_startup(tid_##task);                                                      \
        TSK_DEBUG_PRINTF("Create: task:%s, stack:%d, priority:%d\r\n", #task, stack, priority); \
    } while (0)

#define xTimerReg(func, time)                                              \
    do                                                                     \
    {                                                                      \
        TimerHandle_t xTimer = xTimerCreate(#func, time, pdTRUE, 0, func); \
        if (xTimer)                                                        \
            xTimerStart(xTimer, 0);                                        \
        TSK_DEBUG_PRINTF("Create: timer:%s, time:%d\r\n", #func, time);    \
    } while (0)

/*Task Functions*/

/*TaskHandle*/

/*Timer Functions*/

/*TimerHandle*/

/*QueueHandle*/

#endif

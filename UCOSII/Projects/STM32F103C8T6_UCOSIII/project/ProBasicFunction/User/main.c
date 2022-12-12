
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "includes.h"

// 任务优先级
#define START_TASK_PRIO 3
// 任务堆栈大小
#define START_STK_SIZE 512
// 任务控制块
OS_TCB StartTaskTCB;
// 任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
// 任务函数
void start_task(void *p_arg);
// 任务优先级
#define LED1_TASK_PRIO 4
// 任务堆栈大小
#define LED1_STK_SIZE 128
// 任务控制块
OS_TCB Led1TaskTCB;
// 任务堆栈
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);

int main()
{
    OS_ERR err;
    LedInit();
    OSInit(&err); // 初始化 UCOSIII
    // 创建开始任务
    OSTaskCreate((OS_TCB *)&StartTaskTCB,                           // 任务控制块
                 (CPU_CHAR *)"start task",                          // 任务名字
                 (OS_TASK_PTR)start_task,                           // 任务函数
                 (void *)0,                                         // 传递给任务函数的参数
                 (OS_PRIO)START_TASK_PRIO,                          // 任务优先级
                 (CPU_STK *)&START_TASK_STK[0],                     // 任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE / 10,                 // 任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,                      // 任务堆栈大小
                 (OS_MSG_QTY)0,                                     // 任务内部消息队列能够接收的最大消息数目, 为 0 时禁止接收消息
                 (OS_TICK)0,                                        // 当使能时间片轮转时的时间片长度，为 0 时为默认长度，
                 (void *)0,                                         // 用户补充的存储区
                 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, // 任务选项
                 (OS_ERR *)&err);                                   // 存放该函数错误时的返回值
    OSStart(&err);                                                  // 开启 UCOSIII
    while (1)
        ;
}
// 开始任务函数
void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_INT32U cpu_clk_freq;
    CPU_INT32U cnts;
    CPU_SR_ALLOC();
    p_arg = p_arg;
    CPU_Init();
    cpu_clk_freq = BSP_CPU_ClkFreq();                    /* Determine SysTick reference freq. */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz; /*Determine nbr SysTick increments */
    OS_CPU_SysTickInit(cnts);                            /* Init uC/OS periodic time src (SysTick). */
    Mem_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err); // 统计任务
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN // 如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif
#if OS_CFG_SCHED_ROUND_ROBIN_EN // 当使用时间片轮转的时候
    // 使能时间片轮转调度功能,时间片长度为 1 个系统时钟节拍，既1 * 5 = 5ms
    OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif
    OS_CRITICAL_ENTER(); // 进入临界区
    // 创建 LED1 任务
    OSTaskCreate((OS_TCB *)&Led1TaskTCB,
                 (CPU_CHAR *)"led1 task",
                 (OS_TASK_PTR)led1_task,
                 (void *)0,
                 (OS_PRIO)LED1_TASK_PRIO,
                 (CPU_STK *)&LED1_TASK_STK[0],
                 (CPU_STK_SIZE)LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED1_STK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)0,
                 (void *)0,
                 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *)&err);
    OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err); // 挂起开始任务
    OS_CRITICAL_EXIT();                            // 进入临界区
}
// led1 任务函数
void led1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while (1)
    {
        LED_BIT_GPIOC_PIN13 = !LED_BIT_GPIOC_PIN13;
        OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err); // 延时 200ms
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <os.h>
#include "stm32f10x.h"

extern uint32_t SystemCoreClock;

/*$PAGE*/
/*
**********************************************************************************************************
*                                       INITIALIZE A TASK'S STACK
*
* Description: This function is called by OS_Task_Create() or OSTaskCreateExt() to initialize the stack
*              frame of the task being created. This function is highly processor specific.
*
* Arguments  : p_task       Pointer to the task entry point address.
*
*              p_arg        Pointer to a user supplied data area that will be passed to the task
*                               when the task first executes.
*
*              p_stk_base   Pointer to the base address of the stack.
*
*              stk_size     Size of the stack, in number of CPU_STK elements.
*
*              opt          Options used to alter the behavior of OS_Task_StkInit().
*                            (see OS.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when task starts executing.
*
*              2) All tasks run in Thread mode, using process stack.
**********************************************************************************************************
*/

CPU_STK  *OSTaskStkInit (OS_TASK_PTR    p_task,
                         void          *p_arg,
                         CPU_STK       *p_stk_base,
                         CPU_STK_SIZE   stk_size)
{
    CPU_STK  *p_stk;

    p_stk = &p_stk_base[stk_size];                          /* Load stack pointer                                     */
                                                            /* Registers stacked as if auto-saved on exception        */
    // *--p_stk = (CPU_STK)0x01000000u;                        /* xPSR                                                   */
    // *--p_stk = (CPU_STK)p_task;                             /* Entry Point                                            */
    // *--p_stk = (CPU_STK)OS_TaskReturn;                      /* R14 (LR)                                               */
    // *--p_stk = (CPU_STK)0x12121212u;                        /* R12                                                    */
    // *--p_stk = (CPU_STK)0x03030303u;                        /* R3                                                     */
    // *--p_stk = (CPU_STK)0x02020202u;                        /* R2                                                     */
    // *--p_stk = (CPU_STK)p_stk_limit;                        /* R1                                                     */
    // *--p_stk = (CPU_STK)p_arg;                              /* R0 : argument                                          */
    //                                                         /* Remaining registers saved on process stack             */
    // *--p_stk = (CPU_STK)0x11111111u;                        /* R11                                                    */
    // *--p_stk = (CPU_STK)0x10101010u;                        /* R10                                                    */
    // *--p_stk = (CPU_STK)0x09090909u;                        /* R9                                                     */
    // *--p_stk = (CPU_STK)0x08080808u;                        /* R8                                                     */
    // *--p_stk = (CPU_STK)0x07070707u;                        /* R7                                                     */
    // *--p_stk = (CPU_STK)0x06060606u;                        /* R6                                                     */
    // *--p_stk = (CPU_STK)0x05050505u;                        /* R5                                                     */
    // *--p_stk = (CPU_STK)0x04040404u;                        /* R4                                                     */

    /* 异常发生时自动保存的寄存器 */
    /* *--p_stk == *(--p_stk)  STM32 中使用的是满减栈， */
    /* 其他还有满增栈，空减栈，空增栈  */
    *--p_stk = (CPU_STK)0x01000000u;    /* xPSR的bit24必须置1  */
    *--p_stk = (CPU_STK)p_task;         /* R15(PC)任务的入口地址*/
    *--p_stk = (CPU_STK)0x14141414u;    /* R14 (LR)            */
    *--p_stk = (CPU_STK)0x12121212u;    /* R12                 */
    *--p_stk = (CPU_STK)0x03030303u;    /* R3                  */
    *--p_stk = (CPU_STK)0x02020202u;    /* R2                  */
    *--p_stk = (CPU_STK)0x01010101u;    /* R1                  */
    *--p_stk = (CPU_STK)p_arg;          /* R0 : 任务形参*/
    /* 异常发生时需手动保存的寄存器 */
    *--p_stk = (CPU_STK)0x11111111u;    /* R11                 */
    *--p_stk = (CPU_STK)0x10101010u;    /* R10                 */
    *--p_stk = (CPU_STK)0x09090909u;    /* R9                  */
    *--p_stk = (CPU_STK)0x08080808u;    /* R8                  */
    *--p_stk = (CPU_STK)0x07070707u;    /* R7                  */
    *--p_stk = (CPU_STK)0x06060606u;    /* R6                  */
    *--p_stk = (CPU_STK)0x05050505u;    /* R5                  */
    *--p_stk = (CPU_STK)0x04040404u;    /* R4                  */

    return (p_stk);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          SYS TICK HANDLER
*
* Description: Handle the system tick (SysTick) interrupt, which is used to generate the uC/OS-II tick
*              interrupt.
*
* Arguments  : None.
*
* Note(s)    : 1) This function MUST be placed on entry 15 of the Cortex-M3 vector table.
*********************************************************************************************************
*/

void  OS_CPU_SysTickHandler (void)
{
    OSTimeTick();                                           /* Call uC/OS-III's OSTimeTick()                          */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         INITIALIZE SYS TICK
*
* Description: Initialize the SysTick.
*
* Arguments  : cnts         Number of SysTick counts between two OS tick interrupts.
*
* Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
*********************************************************************************************************
*/
#if 0/* 不用μC/OS-III自带的 */
void  OS_CPU_SysTickInit (CPU_INT32U  cnts)
{
    CPU_INT32U  prio;

    /* 填写 SysTick 的重载计数值 */
    CPU_REG_NVIC_ST_RELOAD = cnts - 1u;

    /* 设置 SysTick 中断优先级 */
    prio  = CPU_REG_NVIC_SHPRI3;
    prio &= DEF_BIT_FIELD(24, 0);
    prio |= DEF_BIT_MASK(OS_CPU_CFG_SYSTICK_PRIO, 24);

    CPU_REG_NVIC_SHPRI3 = prio;

    /* 启用 SysTick 的时钟源和启动计数器 */
    CPU_REG_NVIC_ST_CTRL |= CPU_REG_NVIC_ST_CTRL_CLKSOURCE |
                            CPU_REG_NVIC_ST_CTRL_ENABLE;
    /* 启用 SysTick 的定时中断 */
    CPU_REG_NVIC_ST_CTRL |= CPU_REG_NVIC_ST_CTRL_TICKINT;
}

#else/* 直接使用头文件ARMCM3.h里面现有的寄存器定义和函数来实现 */
void  OS_CPU_SysTickInit (CPU_INT32U  ms)
{
    /* 设置重装载寄存器的值 */
    SysTick->LOAD  = ms * SystemCoreClock / 1000 - 1;

    /* 配置中断优先级为最低 */
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    /* 复位当前计数器的值 */
    SysTick->VAL   = 0;

    /* 选择时钟源、启用中断、启用计数器 */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}
#endif

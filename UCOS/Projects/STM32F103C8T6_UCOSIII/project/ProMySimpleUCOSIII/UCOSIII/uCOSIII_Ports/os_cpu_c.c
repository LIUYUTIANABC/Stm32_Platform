/*$PAGE*/
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <os.h>

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

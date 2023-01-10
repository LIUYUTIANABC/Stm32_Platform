/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TIME MANAGEMENT
*
* File    : OS_TIME.C
* By      : JJL
* Version : V3.03.01
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
************************************************************************************************************************
*/

#define  MICRIUM_SOURCE
#include <os.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *os_time__c = "$Id: $";
#endif


/*
************************************************************************************************************************
*                                                  DELAY TASK 'n' TICKS
*
* Description: This function is called to delay execution of the currently running task until the specified number of
*              system ticks expires.  This, of course, directly equates to delaying the current task for some time to
*              expire.  No delay will result if the specified delay is 0.  If the specified delay is greater than 0
*              then, a context switch will result.
*
* Arguments  : dly       is a value in 'clock ticks' that the task will either delay for or, the target match value
*                        of the tick counter (OSTickCtr).  Note that specifying 0 means the task is not to delay.
*
*                        depending on the option argument, the task will wake up when OSTickCtr reaches:
*
*                            OS_OPT_TIME_DLY      : OSTickCtr + dly
*                            OS_OPT_TIME_TIMEOUT  : OSTickCtr + dly
*                            OS_OPT_TIME_MATCH    : dly
*                            OS_OPT_TIME_PERIODIC : OSTCBCurPtr->TickCtrPrev + dly
*
*              opt       specifies whether 'dly' represents absolute or relative time; default option marked with *** :
*
*                        *** OS_OPT_TIME_DLY        specifies a relative time from the current value of OSTickCtr.
*                            OS_OPT_TIME_TIMEOUT    same as OS_OPT_TIME_DLY.
*                            OS_OPT_TIME_MATCH      indicates that 'dly' specifies the absolute value that OSTickCtr
*                                                   must reach before the task will be resumed.
*                            OS_OPT_TIME_PERIODIC   indicates that 'dly' specifies the periodic value that OSTickCtr
*                                                   must reach before the task will be resumed.
*
*              p_err     is a pointer to a variable that will contain an error code from this call.
*
*                            OS_ERR_NONE            the call was successful and the delay occurred.
*                            OS_ERR_OPT_INVALID     if you specified an invalid option for this function.
*                            OS_ERR_SCHED_LOCKED    can't delay when the scheduler is locked.
*                            OS_ERR_TIME_DLY_ISR    if you called this function from an ISR.
*                            OS_ERR_TIME_ZERO_DLY   if you specified a delay of zero.
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeDly (OS_TICK   dly)
{
    /* 设置延时时间 */
    OSTCBCurPtr->TaskDelayTicks = dly;
    /* 进行任务调度 */
    OSSched();                                              /* Find next task to run!                                 */
}



/*$PAGE*/
/*
************************************************************************************************************************
*                                                 PROCESS SYSTEM TICK
*
* Description: This function is used to signal to uC/OS-III the occurrence of a 'system tick' (also known as a
*              'clock tick').  This function should be called by the tick ISR.
*
* Arguments  : none
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeTick (void)
{
    /* 任务调度 */
    OSSched();
}

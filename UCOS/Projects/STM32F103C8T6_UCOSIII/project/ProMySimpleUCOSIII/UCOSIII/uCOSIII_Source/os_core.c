#define  MICRIUM_SOURCE
#include <os.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *os_core__c = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                    INITIALIZATION
*
* Description: This function is used to initialize the internals of uC/OS-III and MUST be called prior to
*              creating any uC/OS-III object and, prior to calling OS_Start().
*
* Arguments  : p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE    Initialization was successful
*                                Other          Other OS_ERR_xxx depending on the sub-functions called by OSInit().
* Returns    : none
************************************************************************************************************************
*/

void  OSInit (OS_ERR  *p_err)
{
    /* 配置OS初始状态为停止态 */
    OSRunning                       =  OS_STATE_OS_STOPPED; /* Indicate that multitasking not started                 */

    /* 初始化两个全局TCB，这两个TCB用于任务切换 */
    OSTCBCurPtr                     = (OS_TCB *)0;          /* Initialize OS_TCB pointers to a known state            */
    OSTCBHighRdyPtr                 = (OS_TCB *)0;

    /* 初始化两个全局TCB，这两个TCB用于任务切换 */
    OS_RdyListInit();                                       /* Initialize the Ready List                              */

    /* 初始化空闲任务 */
    OS_IdleTaskInit(p_err);
    if(*p_err != OS_ERR_NONE)
    {
        return;
    }
}


/*$PAGE*/
/*
************************************************************************************************************************
*                                                      SCHEDULER
*
* Description: This function is called by other uC/OS-III services to determine whether a new, high priority task has
*              been made ready to run.  This function is invoked by TASK level code and is not used to reschedule tasks
*              from ISRs (see OSIntExit() for ISR rescheduling).
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : 1) Rescheduling is prevented when the scheduler is locked (see OSSchedLock())
************************************************************************************************************************
*/
/* 任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换 */
void  OSSched (void)
{
#if 0/* 非常简单的任务调度：两个任务轮流执行 */
    if ( OSTCBCurPtr == OSRdyList[0].HeadPtr )
    {
        OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
    }
    else
    {
        OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
    }
#endif

    /* 如果当前任务是空闲任务，那么就去尝试执行任务1或者任务2，
    看看他们的延时时间是否结束，如果任务的延时时间均没有到期，
    那就返回继续执行空闲任务 */
    if ( OSTCBCurPtr == &OSIdleTaskTCB )
    {
        if (OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
        {
            OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
        }
        else if (OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
        {
            OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
        }
        else
        {
            /* 任务延时均没有到期则返回，继续执行空闲任务 */
            return;
        }
    }
    else
    {
        /*如果是task1或者task2的话，检查下另外一个任务,
        如果另外的任务不在延时中，就切换到该任务
        否则，判断下当前任务是否应该进入延时状态，
        如果是的话，就切换到空闲任务。否则就不进行任何切换 */
        if (OSTCBCurPtr == OSRdyList[0].HeadPtr)
        {
            if (OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
            {
                OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
            }
            else if (OSTCBCurPtr->TaskDelayTicks != 0)
            {
                OSTCBHighRdyPtr = &OSIdleTaskTCB;
            }
            else
            {
            /* 返回，不进行切换，因为两个任务都处于延时中 */
            return;
            }
        }
        else if (OSTCBCurPtr == OSRdyList[1].HeadPtr)
        {
            if (OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
            {
                OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
            }
            else if (OSTCBCurPtr->TaskDelayTicks != 0)
            {
                OSTCBHighRdyPtr = &OSIdleTaskTCB;
            }
            else
            {
                /* 返回，不进行切换，因为两个任务都处于延时中 */
                return;
            }
        }
    }

    /* 任务切换 */
    OS_TASK_SW();
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                 START MULTITASKING
*
* Description: This function is used to start the multitasking process which lets uC/OS-III manages the task that you
*              created.  Before you can call OSStart(), you MUST have called OSInit() and you MUST have created at least
*              one application task.
*
* Argument(s): p_err      is a pointer to a variable that will contain an error code returned by this function.
*
*                             OS_ERR_FATAL_RETURN    OS was running and OSStart() returned.
*                             OS_ERR_OS_RUNNING      OS is already running, OSStart() has no effect
*
* Returns    : none
*
* Note(s)    : 1) OSStartHighRdy() MUST:
*                 a) Call OSTaskSwHook() then,
*                 b) Load the context of the task pointed to by OSTCBHighRdyPtr.
*                 c) Execute the task.
*
*              2) OSStart() is not supposed to return.  If it does, that would be considered a fatal error.
************************************************************************************************************************
*/

void  OSStart (OS_ERR  *p_err)
{
    if (OSRunning == OS_STATE_OS_STOPPED) {
        /* 手动配置任务1先运行 */
        OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
        /* 启动任务切换，不会返回 */
        OSStartHighRdy();                                   /* Execute target specific code to start task             */
        /* 不会运行到这里，运行到这里表示发生了致命的错误 */
       *p_err           = OS_ERR_FATAL_RETURN;              /* OSStart() is not supposed to return                    */
    } else {
       *p_err           = OS_ERR_OS_RUNNING;                /* OS is already running                                  */
    }
}


/*$PAGE*/
/*
************************************************************************************************************************
*                                                      IDLE TASK
*
* Description: This task is internal to uC/OS-III and executes whenever no other higher priority tasks executes because
*              they are ALL waiting for event(s) to occur.
*
* Arguments  : p_arg    is an argument passed to the task when the task is created.
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) OSIdleTaskHook() is called after the critical section to ensure that interrupts will be enabled for at
*                 least a few instructions.  On some processors (ex. Philips XA), enabling and then disabling interrupts
*                 doesn't allow the processor enough time to have interrupts enabled before they were disabled again.
*                 uC/OS-III would thus never recognize interrupts.
*
*              3) This hook has been added to allow you to do such things as STOP the CPU to conserve power.
************************************************************************************************************************
*/

void  OS_IdleTask (void  *p_arg)
{
    p_arg = p_arg;                                          /* Prevent compiler warning for not using 'p_arg'         */

    /* 空闲任务什么都不做，只对全局变量OSIdleTaskCtr ++ 操作 */
    for (;;)
    {
        OSIdleTaskCtr++;
    }
}


/*$PAGE*/
/*
************************************************************************************************************************
*                                               INITIALIZE THE IDLE TASK
*
* Description: This function initializes the idle task
*
* Arguments  : p_err    is a pointer to a variable that will contain an error code returned by this function.
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/
void  OS_IdleTaskInit (OS_ERR  *p_err)
{
    /* 初始化空闲任务计数器 */
    OSIdleTaskCtr = (OS_IDLE_CTR)0;
                                                            /* ---------------- CREATE THE IDLE TASK ---------------- */
    /* 创建空闲任务 */
    OSTaskCreate ((OS_TCB*)      &OSIdleTaskTCB,
                (OS_TASK_PTR ) OS_IdleTask,
                (void *)       0,
                (CPU_STK*)     OSCfg_IdleTaskStkBasePtr,
                (CPU_STK_SIZE) OSCfg_IdleTaskStkSize,
                (OS_ERR *)     &p_err);
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                    INITIALIZATION
*                                               READY LIST INITIALIZATION
*
* Description: This function is called by OSInit() to initialize the ready list.  The ready list contains a list of all
*              the tasks that are ready to run.  The list is actually an array of OS_RDY_LIST.  An OS_RDY_LIST contains
*              three fields.  The number of OS_TCBs in the list (i.e. .NbrEntries), a pointer to the first OS_TCB in the
*              OS_RDY_LIST (i.e. .HeadPtr) and a pointer to the last OS_TCB in the OS_RDY_LIST (i.e. .TailPtr).
*
*              OS_TCBs are doubly linked in the OS_RDY_LIST and each OS_TCB points pack to the OS_RDY_LIST it belongs
*              to.
*
*              'OS_RDY_LIST  OSRdyTbl[OS_CFG_PRIO_MAX]'  looks like this once initialized:
*
*                               +---------------+--------------+
*                               |               | TailPtr      |-----> 0
*                          [0]  | NbrEntries=0  +--------------+
*                               |               | HeadPtr      |-----> 0
*                               +---------------+--------------+
*                               |               | TailPtr      |-----> 0
*                          [1]  | NbrEntries=0  +--------------+
*                               |               | HeadPtr      |-----> 0
*                               +---------------+--------------+
*                                       :              :
*                                       :              :
*                                       :              :
*                               +---------------+--------------+
*                               |               | TailPtr      |-----> 0
*          [OS_CFG_PRIO_MAX-1]  | NbrEntries=0  +--------------+
*                               |               | HeadPtr      |-----> 0
*                               +---------------+--------------+
*
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_RdyListInit (void)
{
    OS_PRIO       i;
    OS_RDY_LIST  *p_rdy_list;

    for (i = 0u; i < OS_CFG_PRIO_MAX; i++) {                /* Initialize the array of OS_RDY_LIST at each priority   */
        p_rdy_list = &OSRdyList[i];
        // p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
        p_rdy_list->HeadPtr    = (OS_TCB   *)0;
        p_rdy_list->TailPtr    = (OS_TCB   *)0;
    }
}

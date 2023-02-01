#include <os.h>

/*$PAGE*/
/*
************************************************************************************************************************
*                                               INITIALIZE TCB FIELDS
*
* Description: This function is called to initialize a TCB to default values
*
* Arguments  : p_tcb    is a pointer to the TCB to initialize
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TaskInitTCB (OS_TCB  *p_tcb)
{
    p_tcb->StkPtr             = (CPU_STK       *)0;
    p_tcb->StkSize            = (CPU_STK_SIZE   )0u;

    p_tcb->TaskDelayTicks     = (OS_TICK       )0u;

    p_tcb->Prio               = (OS_PRIO        )OS_PRIO_INIT;

    p_tcb->NextPtr            = (OS_TCB        *)0;
    p_tcb->PrevPtr            = (OS_TCB        *)0;
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                    CREATE A TASK
*
* Description: This function is used to have uC/OS-III manage the execution of a task.  Tasks can either be created
*              prior to the start of multitasking or by a running task.  A task cannot be created by an ISR.
*
* Arguments  : p_tcb          is a pointer to the task's TCB
*
*              p_name         is a pointer to an ASCII string to provide a name to the task.
*
*              p_task         is a pointer to the task's code
*
*              p_arg          is a pointer to an optional data area which can be used to pass parameters to
*                             the task when the task first executes.  Where the task is concerned it thinks
*                             it was invoked and passed the argument 'p_arg' as follows:
*
*                                 void Task (void *p_arg)
*                                 {
*                                     for (;;) {
*                                         Task code;
*                                     }
*                                 }
*
*              prio           is the task's priority.  A unique priority MUST be assigned to each task and the
*                             lower the number, the higher the priority.
*
*              p_stk_base     is a pointer to the base address of the stack (i.e. low address).
*
*              stk_limit      is the number of stack elements to set as 'watermark' limit for the stack.  This value
*                             represents the number of CPU_STK entries left before the stack is full.  For example,
*                             specifying 10% of the 'stk_size' value indicates that the stack limit will be reached
*                             when the stack reaches 90% full.
*
*              stk_size       is the size of the stack in number of elements.  If CPU_STK is set to CPU_INT08U,
*                             'stk_size' corresponds to the number of bytes available.  If CPU_STK is set to
*                             CPU_INT16U, 'stk_size' contains the number of 16-bit entries available.  Finally, if
*                             CPU_STK is set to CPU_INT32U, 'stk_size' contains the number of 32-bit entries
*                             available on the stack.
*
*              q_size         is the maximum number of messages that can be sent to the task
*
*              time_quanta    amount of time (in ticks) for time slice when round-robin between tasks.  Specify 0 to use
*                             the default.
*
*              p_ext          is a pointer to a user supplied memory location which is used as a TCB extension.
*                             For example, this user memory can hold the contents of floating-point registers
*                             during a context switch, the time each task takes to execute, the number of times
*                             the task has been switched-in, etc.
*
*              opt            contains additional information (or options) about the behavior of the task.
*                             See OS_OPT_TASK_xxx in OS.H.  Current choices are:
*
*                                 OS_OPT_TASK_NONE            No option selected
*                                 OS_OPT_TASK_STK_CHK         Stack checking to be allowed for the task
*                                 OS_OPT_TASK_STK_CLR         Clear the stack when the task is created
*                                 OS_OPT_TASK_SAVE_FP         If the CPU has floating-point registers, save them
*                                                             during a context switch.
*                                 OS_OPT_TASK_NO_TLS          If the caller doesn't want or need TLS (Thread Local
*                                                             Storage) support for the task.  If you do not include this
*                                                             option, TLS will be supported by default.
*
*              p_err          is a pointer to an error code that will be set during this call.  The value pointer
*                             to by 'p_err' can be:
*
*                                 OS_ERR_NONE                    if the function was successful.
*                                 OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the task after you called
*                                                                   OSSafetyCriticalStart().
*                                 OS_ERR_NAME                    if 'p_name' is a NULL pointer
*                                 OS_ERR_PRIO_INVALID            if the priority you specify is higher that the maximum
*                                                                   allowed (i.e. >= OS_CFG_PRIO_MAX-1) or,
*                                                                if OS_CFG_ISR_POST_DEFERRED_EN is set to 1 and you tried
*                                                                   to use priority 0 which is reserved.
*                                 OS_ERR_STK_INVALID             if you specified a NULL pointer for 'p_stk_base'
*                                 OS_ERR_STK_SIZE_INVALID        if you specified zero for the 'stk_size'
*                                 OS_ERR_STK_LIMIT_INVALID       if you specified a 'stk_limit' greater than or equal
*                                                                   to 'stk_size'
*                                 OS_ERR_TASK_CREATE_ISR         if you tried to create a task from an ISR.
*                                 OS_ERR_TASK_INVALID            if you specified a NULL pointer for 'p_task'
*                                 OS_ERR_TCB_INVALID             if you specified a NULL pointer for 'p_tcb'
*
* Returns    : A pointer to the TCB of the task created.  This pointer must be used as an ID (i.e handle) to the task.
************************************************************************************************************************
*/
/*$PAGE*/
void  OSTaskCreate (OS_TCB        *p_tcb,
                    OS_TASK_PTR    p_task,
                    void          *p_arg,
                    OS_PRIO        prio,
                    CPU_STK       *p_stk_base,
                    CPU_STK_SIZE   stk_size,
                    OS_TICK       time_quanta,
                    OS_ERR        *p_err)
{
    CPU_STK       *p_sp;
    // CPU_STK       *p_stk_limit;
    CPU_SR_ALLOC();

    /* 初始化TCB为默认值 */
    OS_TaskInitTCB(p_tcb);
                                                            /* --------------- CLEAR THE TASK'S STACK --------------- */
    p_sp = OSTaskStkInit(p_task,
                         p_arg,
                         p_stk_base,
                         stk_size);

    p_tcb->Prio = prio;

                                                            /* -------------- INITIALIZE THE TCB FIELDS ------------- */
    p_tcb->StkPtr        = p_sp;                            /* Save the new top-of-stack pointer                      */
    p_tcb->StkSize       = stk_size;                        /* Save the stack size (in number of CPU_STK elements)    */
    p_tcb->TaskDelayTicks = 0;

    /* 时间片相关初始化 */
    p_tcb->TimeQuanta    = time_quanta;
#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
    p_tcb->TimeQuantaCtr = time_quanta;
#endif

    /* 进入临界段 */
    OS_CRITICAL_ENTER();

    /* 将任务添加到就绪列表 */
    OS_PrioInsert(p_tcb->Prio);
    OS_RdyListInsertTail(p_tcb);

    /* 退出临界段 */
    OS_CRITICAL_EXIT();

    // if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Return if multitasking has not started                 */
    //     OS_CRITICAL_EXIT();
    //     return;
    // }

    // OSSched();
   *p_err = OS_ERR_NONE;
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                   SUSPEND A TASK
*
* Description: This function is called to suspend a task.  The task can be the calling task if 'p_tcb' is a NULL pointer
*              or the pointer to the TCB of the calling task.
*
* Arguments  : p_tcb    is a pointer to the TCB to suspend.
*                       If p_tcb is a NULL pointer then, suspend the current task.
*
*              p_err    is a pointer to a variable that will receive an error code from this function.
*
*                           OS_ERR_NONE                      if the requested task is suspended
*                           OS_ERR_SCHED_LOCKED              you can't suspend the current task is the scheduler is
*                                                            locked
*                           OS_ERR_TASK_SUSPEND_ISR          if you called this function from an ISR
*                           OS_ERR_TASK_SUSPEND_IDLE         if you attempted to suspend the idle task which is not
*                                                            allowed.
*                           OS_ERR_TASK_SUSPEND_INT_HANDLER  if you attempted to suspend the idle task which is not
*                                                            allowed.
*
* Note(s)    : 1) You should use this function with great care.  If you suspend a task that is waiting for an event
*                 (i.e. a message, a semaphore, a queue ...) you will prevent this task from running when the event
*                 arrives.
************************************************************************************************************************
*/
#if OS_CFG_TASK_SUSPEND_EN > 0u
void   OSTaskSuspend (OS_TCB  *p_tcb,
                    OS_ERR  *p_err)
{
    CPU_SR_ALLOC();

#if 0/* 屏蔽开始 */
    #ifdef OS_SAFETY_CRITICAL
    /* 安全检查，OS_SAFETY_CRITICAL_EXCEPTION()函数需要用户自行编写 */
    if (p_err == (OS_ERR *)0)
    {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    /* 不能在ISR程序中调用该函数 */
    if (OSIntNestingCtr > (OS_NESTING_CTR)0)
    {
        *p_err = OS_ERR_TASK_SUSPEND_ISR;
        return;
    }
#endif

    /* 不能挂起空闲任务 */
    if (p_tcb == &OSIdleTaskTCB)
    {
        *p_err = OS_ERR_TASK_SUSPEND_IDLE;
        return;
    }

#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
    /* 不能挂起中断处理任务 */
    if (p_tcb == &OSIntQTaskTCB)
    {
        *p_err = OS_ERR_TASK_SUSPEND_INT_HANDLER;
        return;
    }
#endif

#endif/* 屏蔽结束 */

    CPU_CRITICAL_ENTER();

    /* 是否挂起自己 */
    if (p_tcb == (OS_TCB *)0) {
        p_tcb = OSTCBCurPtr;
    }

    // if (p_tcb == OSTCBCurPtr) {
    //     /* 如果调度器锁住则不能挂起自己 */
    //     if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {
    //             CPU_CRITICAL_EXIT();
    //             *p_err = OS_ERR_SCHED_LOCKED;
    //             return;
    //         }
    // }

    *p_err = OS_ERR_NONE;

    /* 根据任务的状态来决定挂起的动作 */
    switch (p_tcb->TaskState)
    {
        case OS_TASK_STATE_RDY:
            // OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT();
            p_tcb->TaskState  =  OS_TASK_STATE_SUSPENDED;
            p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
            OS_RdyListRemove(p_tcb);
            OS_CRITICAL_EXIT_NO_SCHED();
            break;

        case OS_TASK_STATE_DLY:
            p_tcb->TaskState  = OS_TASK_STATE_DLY_SUSPENDED;
            p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
            CPU_CRITICAL_EXIT();
            break;

        case OS_TASK_STATE_PEND:
            p_tcb->TaskState  = OS_TASK_STATE_PEND_SUSPENDED;
            p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
            CPU_CRITICAL_EXIT();
            break;

        case OS_TASK_STATE_PEND_TIMEOUT:
            p_tcb->TaskState  = OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED;
            p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
            CPU_CRITICAL_EXIT();
            break;

        case OS_TASK_STATE_SUSPENDED:
        case OS_TASK_STATE_DLY_SUSPENDED:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
            p_tcb->SuspendCtr++;
            CPU_CRITICAL_EXIT();
            break;

        default:
            CPU_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
            return;
    }

    /* 任务切换 */
    OSSched();
}
#endif


/*$PAGE*/
/*
************************************************************************************************************************
*                                               RESUME A SUSPENDED TASK
*
* Description: This function is called to resume a previously suspended task.  This is the only call that will remove an
*              explicit task suspension.
*
* Arguments  : p_tcb      Is a pointer to the task's OS_TCB to resume
*
*              p_err      Is a pointer to a variable that will contain an error code returned by this function
*
*                             OS_ERR_NONE                  if the requested task is resumed
*                             OS_ERR_STATE_INVALID         if the task is in an invalid state
*                             OS_ERR_TASK_RESUME_ISR       if you called this function from an ISR
*                             OS_ERR_TASK_RESUME_SELF      You cannot resume 'self'
*                             OS_ERR_TASK_NOT_SUSPENDED    if the task to resume has not been suspended
*
* Returns    : none
************************************************************************************************************************
*/
#if OS_CFG_TASK_SUSPEND_EN > 0u
void  OSTaskResume (OS_TCB  *p_tcb,
                    OS_ERR  *p_err)
{
    CPU_SR_ALLOC();


#if 0/* 屏蔽开始 */
#ifdef OS_SAFETY_CRITICAL
/* 安全检查，OS_SAFETY_CRITICAL_EXCEPTION()函数需要用户自行编写 */
if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
return;
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
/* 不能在ISR程序中调用该函数 */
if (OSIntNestingCtr > (OS_NESTING_CTR)0) {
        *p_err = OS_ERR_TASK_RESUME_ISR;
return;
    }
#endif


    CPU_CRITICAL_ENTER();
#if OS_CFG_ARG_CHK_EN > 0u
/* 不能自己恢复自己 */
if ((p_tcb == (OS_TCB *)0) ||
        (p_tcb == OSTCBCurPtr)) {
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_RESUME_SELF;
return;
    }
#endif

#endif/* 屏蔽结束 */

    CPU_CRITICAL_ENTER();

    *p_err  = OS_ERR_NONE;
    /* 根据任务的状态来决定挂起的动作 */
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
        case OS_TASK_STATE_DLY:
        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_TIMEOUT:
                CPU_CRITICAL_EXIT();
                *p_err = OS_ERR_TASK_NOT_SUSPENDED;
                break;
        case OS_TASK_STATE_SUSPENDED:
                // OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT();
                p_tcb->SuspendCtr--;
                if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) {
                    p_tcb->TaskState = OS_TASK_STATE_RDY;
                    OS_TaskRdy(p_tcb);
                }
                OS_CRITICAL_EXIT_NO_SCHED();
            break;
        case OS_TASK_STATE_DLY_SUSPENDED:
                p_tcb->SuspendCtr--;
                if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) {
                    p_tcb->TaskState = OS_TASK_STATE_DLY;
                }
                CPU_CRITICAL_EXIT();
            break;
        case OS_TASK_STATE_PEND_SUSPENDED:
                p_tcb->SuspendCtr--;
                if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) {
                    p_tcb->TaskState = OS_TASK_STATE_PEND;
                }
                CPU_CRITICAL_EXIT();
            break;
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
                p_tcb->SuspendCtr--;
                if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) {
                    p_tcb->TaskState = OS_TASK_STATE_PEND_TIMEOUT;
                }
                CPU_CRITICAL_EXIT();
            break;
        default:
                CPU_CRITICAL_EXIT();
                *p_err = OS_ERR_STATE_INVALID;
                return;
    }

    /* 任务切换 */
    OSSched();
}
#endif


/*$PAGE*/
/*
************************************************************************************************************************
*                                                     DELETE A TASK
*
* Description: This function allows you to delete a task.  The calling task can delete itself by specifying a NULL
*              pointer for 'p_tcb'.  The deleted task is returned to the dormant state and can be re-activated by
*              creating the deleted task again.
*
* Arguments  : p_tcb      is the TCB of the tack to delete
*
*              p_err      is a pointer to an error code returned by this function:
*
*                             OS_ERR_NONE                  if the call is successful
*                             OS_ERR_STATE_INVALID         if the state of the task is invalid
*                             OS_ERR_TASK_DEL_IDLE         if you attempted to delete uC/OS-III's idle task
*                             OS_ERR_TASK_DEL_INVALID      if you attempted to delete uC/OS-III's ISR handler task
*                             OS_ERR_TASK_DEL_ISR          if you tried to delete a task from an ISR
*
* Note(s)    : 1) 'p_err' gets set to OS_ERR_NONE before OSSched() to allow the returned error code to be monitored even
*                 for a task that is deleting itself. In this case, 'p_err' MUST point to a global variable that can be
*                 accessed by another task.
************************************************************************************************************************
*/
#if OS_CFG_TASK_DEL_EN > 0u
void  OSTaskDel (OS_TCB  *p_tcb,
                OS_ERR  *p_err)
{
    CPU_SR_ALLOC();

    /* 不允许删除空闲任务 */
    if (p_tcb == &OSIdleTaskTCB)
    {
        *p_err = OS_ERR_TASK_DEL_IDLE;
         return;
    }

    /* 删除自己 */
    if (p_tcb == (OS_TCB *)0)
    {
        CPU_CRITICAL_ENTER();
        p_tcb  = OSTCBCurPtr;
        CPU_CRITICAL_EXIT();
    }

    OS_CRITICAL_ENTER();

    /* 根据任务的状态来决定删除的动作 */
    switch (p_tcb->TaskState)
    {
        case OS_TASK_STATE_RDY:
            OS_RdyListRemove(p_tcb);
            break;

        case OS_TASK_STATE_SUSPENDED:
            break;

        /* 任务只是在延时，并没有在任何等待列表*/
        case OS_TASK_STATE_DLY:
        case OS_TASK_STATE_DLY_SUSPENDED:
            OS_TickListRemove(p_tcb);
            break;

        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
            OS_TickListRemove(p_tcb);
        #if 0/* 目前我们还没有实现等待列表，暂时先把这部分代码注释 */
            /* 看看在等待什么 */
            switch (p_tcb->PendOn) {
                case OS_TASK_PEND_ON_NOTHING:
                /* 任务信号量和队列没有等待队列，直接退出 */
                case OS_TASK_PEND_ON_TASK_Q:
                case OS_TASK_PEND_ON_TASK_SEM:
                    break;

                /* 从等待列表移除 */
                case OS_TASK_PEND_ON_FLAG:
                case OS_TASK_PEND_ON_MULTI:
                case OS_TASK_PEND_ON_MUTEX:
                case OS_TASK_PEND_ON_Q:
                case OS_TASK_PEND_ON_SEM:
                    OS_PendListRemove(p_tcb);
                    break;
                default:
                    break;
            }
        #endif
            break;
        default:
            OS_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
            return;
    }

    /* 初始化TCB为默认值 */
    OS_TaskInitTCB(p_tcb);
    /* 修改任务的状态为删除态，即处于休眠 */
    p_tcb->TaskState = (OS_STATE)OS_TASK_STATE_DEL;

    OS_CRITICAL_EXIT_NO_SCHED();
    *p_err = OS_ERR_NONE;

    /* 任务切换，寻找最高优先级的任务 */
    OSSched();
}
#endif/* OS_CFG_TASK_DEL_EN > 0u */

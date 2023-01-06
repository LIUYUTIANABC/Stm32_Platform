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
    OSRunning                       =  OS_STATE_OS_STOPPED; /* Indicate that multitasking not started                 */

    OSTCBCurPtr                     = (OS_TCB *)0;          /* Initialize OS_TCB pointers to a known state            */
    OSTCBHighRdyPtr                 = (OS_TCB *)0;

    OS_RdyListInit();                                       /* Initialize the Ready List                              */

    *p_err = OS_ERR_NONE;
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
     if ( OSTCBCurPtr == OSRdyList[0].HeadPtr )
    {
        OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
    }
    else
    {
        OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
    }

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

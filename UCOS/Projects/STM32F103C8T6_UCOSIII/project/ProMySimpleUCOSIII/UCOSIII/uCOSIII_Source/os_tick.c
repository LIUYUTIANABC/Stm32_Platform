/*
***********************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TICK MANAGEMENT
*
* File    : OS_TICK.C
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
const  CPU_CHAR  *os_tick__c = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                  LOCAL PROTOTYPES
************************************************************************************************************************
*/
/*$PAGE*/
/*
************************************************************************************************************************
*                                               INITIALIZE THE TICK LIST
*
* Description: This function initializes the tick handling data structures of uC/OS-III.
*
* Arguments  : none
*
* Returns    : None
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/
/* 初始化时基列表的数据域 */
void  OS_TickListInit (void)
{
    OS_TICK_SPOKE_IX   i;
    OS_TICK_SPOKE     *p_spoke;



    for (i = 0u; i < OSCfg_TickWheelSize; i++) {
        p_spoke                = (OS_TICK_SPOKE *)&OSCfg_TickWheel[i];
        p_spoke->FirstPtr      = (OS_TCB        *)0;
        p_spoke->NbrEntries    = (OS_OBJ_QTY     )0u;
        p_spoke->NbrEntriesMax = (OS_OBJ_QTY     )0u;
    }
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                ADD TASK TO TICK LIST
*
* Description: This function is called to place a task in a list of task waiting for either time to expire or waiting to
*              timeout on a pend call.
*
* Arguments  : p_tcb          is a pointer to the OS_TCB of the task to add to the tick list
*              -----
*
*              time           represents either the 'match' value of OSTickCtr or a relative time from the current
*                             value of OSTickCtr as specified by the 'opt' argument..
*
*                             relative when 'opt' is set to OS_OPT_TIME_DLY
*                             relative when 'opt' is set to OS_OPT_TIME_TIMEOUT
*                             match    when 'opt' is set to OS_OPT_TIME_MATCH
*                             periodic when 'opt' is set to OS_OPT_TIME_PERIODIC
*
*              opt            is an option specifying how to calculate time.  The valid values are:
*              ---
*                                 OS_OPT_TIME_DLY
*                                 OS_OPT_TIME_TIMEOUT
*                                 OS_OPT_TIME_PERIODIC
*                                 OS_OPT_TIME_MATCH
*
*              p_err          is a pointer to a variable that will contain an error code returned by this function.
*              -----
*                                 OS_ERR_NONE           the call was successful and the time delay was scheduled.
*                                 OS_ERR_TIME_ZERO_DLY  if delay is zero or already occurred.
*
* Returns    : None
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) This function is assumed to be called with interrupts disabled.
************************************************************************************************************************
*/
/* 将一个任务插入时基列表，根据延时时间的大小升序排列 */
void  OS_TickListInsert (OS_TCB *p_tcb,OS_TICK time)
{
    OS_TICK_SPOKE_IX   spoke;
    OS_TICK_SPOKE     *p_spoke;
    OS_TCB            *p_tcb0;
    OS_TCB            *p_tcb1;

    p_tcb->TickCtrMatch = OSTickCtr + time;
    p_tcb->TickRemain   = time;

    spoke   = (OS_TICK_SPOKE_IX)(p_tcb->TickCtrMatch % OSCfg_TickWheelSize);
    p_spoke = &OSCfg_TickWheel[spoke];

    /* 插入 OSCfg_TickWheel[spoke] 的第一个节点 */
    if (p_spoke->NbrEntries == (OS_OBJ_QTY)0u)
    {
        p_tcb->TickNextPtr   = (OS_TCB   *)0;
        p_tcb->TickPrevPtr   = (OS_TCB   *)0;
        p_spoke->FirstPtr    =  p_tcb;
        p_spoke->NbrEntries  = (OS_OBJ_QTY)1u;
    }
    /* 如果插入的不是第一个节点，则按照TickRemain大小升序排列 */
    else
    {
        /* 获取第一个节点指针 */
        p_tcb1 = p_spoke->FirstPtr;
        while (p_tcb1 != (OS_TCB *)0)
        {
            /* 计算比较节点的剩余时间 */
            p_tcb1->TickRemain = p_tcb1->TickCtrMatch - OSTickCtr;

            /* 插入比较节点的后面 */
            if (p_tcb->TickRemain > p_tcb1->TickRemain)
            {
                if (p_tcb1->TickNextPtr != (OS_TCB *)0)
                {
                    /* 寻找下一个比较节点 */
                    p_tcb1 =  p_tcb1->TickNextPtr;
                }
                else
                {  /* 在最后一个节点插入 */
                    p_tcb->TickNextPtr   = (OS_TCB *)0;
                    p_tcb->TickPrevPtr   =  p_tcb1;
                    p_tcb1->TickNextPtr  =  p_tcb;
                    p_tcb1               = (OS_TCB *)0;
                }
            }
            /* 插入比较节点的前面 */
            else
            {
                /* 在第一个节点插入 */
                if (p_tcb1->TickPrevPtr == (OS_TCB *)0) {
                    p_tcb->TickPrevPtr   = (OS_TCB *)0;
                    p_tcb->TickNextPtr   =  p_tcb1;
                    p_tcb1->TickPrevPtr  =  p_tcb;
                    p_spoke->FirstPtr    =  p_tcb;
                }
                else
                {
                    /* 插入两个节点之间 */
                    p_tcb0               =  p_tcb1->TickPrevPtr;
                    p_tcb->TickPrevPtr   =  p_tcb0;
                    p_tcb->TickNextPtr   =  p_tcb1;
                    p_tcb0->TickNextPtr  =  p_tcb;
                    p_tcb1->TickPrevPtr  =  p_tcb;
                }
                /* 跳出while循环 */
                p_tcb1 = (OS_TCB *)0;
            }
        }

        /* 节点成功插入 */
        p_spoke->NbrEntries++;
    }

    /* 刷新NbrEntriesMax的值 */
    if (p_spoke->NbrEntriesMax < p_spoke->NbrEntries)
    {
        p_spoke->NbrEntriesMax = p_spoke->NbrEntries;
    }

    /* 任务TCB中的TickSpokePtr回指根节点 */
    p_tcb->TickSpokePtr = p_spoke;
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                         REMOVE A TASK FROM THE TICK LIST
*
* Description: This function is called to remove a task from the tick list
*
* Arguments  : p_tcb          Is a pointer to the OS_TCB to remove.
*              -----
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) This function is assumed to be called with interrupts disabled.
************************************************************************************************************************
*/
/* 从时基列表中移除一个任务 */
void  OS_TickListRemove (OS_TCB  *p_tcb)
{
    OS_TICK_SPOKE  *p_spoke;
    OS_TCB         *p_tcb1;
    OS_TCB         *p_tcb2;

    /* 获取任务TCB所在链表的根指针 */
    p_spoke = p_tcb->TickSpokePtr;

    /* 确保任务在链表中 */
    if (p_spoke != (OS_TICK_SPOKE *)0)
    {
        /* 将剩余时间清零 */
        p_tcb->TickRemain = (OS_TICK)0u;

        /* 要移除的刚好是第一个节点 */
        if (p_spoke->FirstPtr == p_tcb)
        {
            /* 更新第一个节点，原来的第一个节点需要被移除 */
            p_tcb1            = (OS_TCB *)p_tcb->TickNextPtr;
            p_spoke->FirstPtr = p_tcb1;
            if (p_tcb1 != (OS_TCB *)0)
            {
                p_tcb1->TickPrevPtr = (OS_TCB *)0;
            }
        }
        /* 要移除的不是第一个节点 */
        else
        {
            /* 保存要移除的节点的前后节点的指针 */
            p_tcb1              = p_tcb->TickPrevPtr;
            p_tcb2              = p_tcb->TickNextPtr;

            /* 节点移除，将节点前后的两个节点连接在一起 */
            p_tcb1->TickNextPtr = p_tcb2;
            if (p_tcb2 != (OS_TCB *)0)
            {
                p_tcb2->TickPrevPtr = p_tcb1;
            }
        }

        /* 复位任务TCB中时基列表相关的字段成员 */
        p_tcb->TickNextPtr  = (OS_TCB        *)0;
        p_tcb->TickPrevPtr  = (OS_TCB        *)0;
        p_tcb->TickSpokePtr = (OS_TICK_SPOKE *)0;
        p_tcb->TickCtrMatch = (OS_TICK        )0u;

        /* 节点减1 */
        p_spoke->NbrEntries--;
    }
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                UPDATE THE TICK LIST
*
* Description: This function is called when a tick occurs and determines if the timeout waiting for a kernel object has
*              expired or a delay has expired.
*
* Arguments  : non
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/
void  OS_TickListUpdate (void)
{
    OS_TICK_SPOKE_IX   spoke;
    OS_TICK_SPOKE     *p_spoke;
    OS_TCB            *p_tcb;
    OS_TCB            *p_tcb_next;
    CPU_BOOLEAN        done;

    CPU_SR_ALLOC();

    /* 进入临界段 */
    OS_CRITICAL_ENTER();

    /* 时基计数器++ */
    OSTickCtr++;

    spoke    = (OS_TICK_SPOKE_IX)(OSTickCtr % OSCfg_TickWheelSize);
    p_spoke  = &OSCfg_TickWheel[spoke];

    p_tcb    = p_spoke->FirstPtr;
    done     = DEF_FALSE;

    while (done == DEF_FALSE)
    {
        if (p_tcb != (OS_TCB *)0)
        {
            p_tcb_next = p_tcb->TickNextPtr;

            p_tcb->TickRemain = p_tcb->TickCtrMatch - OSTickCtr;

            /* 节点延时时间到 */
            if (OSTickCtr == p_tcb->TickCtrMatch)
            {
                /* 让任务就绪 */
                OS_TaskRdy(p_tcb);
            }
            else
            {
                /* 如果第一个节点延时期未满，则退出while循环
                因为链表是根据升序排列的，第一个节点延时期未满，那后面的肯定未满 */
                done = DEF_TRUE;
            }

            /* 如果第一个节点延时期满，则继续遍历链表，看看还有没有延时期满的任务
            如果有，则让它就绪 */
            p_tcb = p_tcb_next;
        }
        else
        {
            done  = DEF_TRUE;
        }
    }

    /* 退出临界段 */
    OS_CRITICAL_EXIT();
}

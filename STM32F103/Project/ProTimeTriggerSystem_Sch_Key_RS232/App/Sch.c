#include "Sch.h"
#include "time.h"
#include "led.h"

//------------------------------------------------------------------------
//- 公用变量定义
//- 数据类型 sTask 和常数 SCH_MAX_TASKS 创建任务队列，被调度器引用
//------------------------------------------------------------------------
sTask SCH_tasks_G[SCH_MAX_TASKS];

//------------------------------------------------------------------------
//- 错误代码
//------------------------------------------------------------------------
#undef SCH_REPORT_ERRORS // 不使用错误代码
#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)
#define RETURN_NORMAL 0
#define RETURN_ERROR 1

tByte Error_code_G = 0;

//------------------------------------------------------------------------
//- 私有函数原型
//------------------------------------------------------------------------
static void SCH_Go_To_Sleep(void);

//------------------------------------------------------------------------
//- 私有变量
//------------------------------------------------------------------------
static tWord Error_tick_count_G = 0;  // 跟踪上一次错误记录以来的时间
static tByte Last_error_count_G = 0;  // 上次的错误代码

/*************************************************************************
* Name: SCH_Init_TIM4
* Function: 准备调度器数据结构并且设置定时器中断（1ms）
* Input:
* Output:
*************************************************************************/
void SCH_Init_TIM4(void)
{
    tByte i;
    for (i = 0; i < SCH_MAX_TASKS; i++)
    {
        // 删除所有任务
        SCH_Delete_Task(i);
    }
    // 复位全局错误变量
    Error_code_G = 0;
    // 设置定时器，这里使用 TIM4
    TIM4_Init(4-1, 18000-1); // 1ms 定时器 TIM4 中断
    // TIM4_Init(20-1, 18000-1); // 5ms 定时器 TIM4 中断
}

/*************************************************************************
* Name: SCH_Add_Task
* Function:
*       使用 const 修饰的全局变量位于内部 flash/ROM 中紧挨着代码段，值不会被修改
* Input:
*       Fn_p: 添加的被调度的函数的名词
*           注意：所有别调度的函数必须时“void,void”,即返回值类型为 void, 参数为 void
*       Delay： 在任务第一次被运行之前的间隔（时标）
*       Period： ‘period’ 如果为0，表示该函数将由 “Delay” 设定的时间，被调用一次
*                ‘period’ 如果非0，表示该函数将按 “Period” 设定的时间间隔，被重复调用
* Output:
*       返回值：返回任务在队列中的位置，如果小于 SCH_MAX_TASKS，则添加成功，如果等于
*              SCH_MAX_TASKS，则队列满了
*       注意：删除任务，需要用到这个返回值 SCH_Delete_Task()
* 例子：
*       Task_ID = SCH_Add_Task(Do_X, 1000, 0); 使函数 Do_X 在 1000 个调度器时标之后运行一次
*       Task_ID = SCH_Add_Task(Do_X, 0, 1000); 使函数 Do_X 每隔 1000 个调度器时标运行一次
*       Task_ID = SCH_Add_Task(Do_X, 300, 1000);
*       使函数 Do_X 每隔 1000 个调度器时标运行一次。且任务首次运行在第 300 个时标，然后1300，2300...
*************************************************************************/
tByte SCH_Add_Task(void (* pFunction)(), const tWord DELAY, const tWord PERIOD)
{
    tByte Index = 0;
    // 首先在队列中找一个空隙（如果有的话）
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }
    // 是否达到队列结尾？
    if (Index == SCH_MAX_TASKS)
    {
        // 任务队列已满
        // 设置全局错误变量
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        return SCH_MAX_TASKS;
    }
    // 运行到这里，说明队列有空间
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    return Index;  // 返回任务的位置（以便以后删除）
}

/*************************************************************************
* Name: SCH_Delete_Task
* Function: 从调度器删除任务，
* Input: 任务索引，由 SCH_Add_Task() 提供
* Output: RETURN_ERROR or RETURN_NORMAL
*************************************************************************/
tByte SCH_Delete_Task(const tByte TASK_INDEX)
{
    tByte Return_code;
    if (SCH_tasks_G[TASK_INDEX].pTask == 0)
    {
        // 这里没有任务
        // 设置全局错误变量
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        // 返回错误代码
        Return_code = RETURN_ERROR;
    }
    else
    {
        Return_code = RETURN_NORMAL;
    }
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;
    return Return_code;  // 返回状态
}

/*************************************************************************
* Name: SCH_Report_Status
* Function:
* Input:
* Output:
*************************************************************************/
void SCH_Report_Status(void)
{
#ifdef SCH_REPORT_ERRORS
    // 只在需要报告错误时适用
    // 检查新的错误代码
#endif
}

/*************************************************************************
* Name: SCH_Go_To_Sleep
* Function: 可以降低功耗，执行完任务，进入低功耗模式
*           如果使用看门狗的话，可能要禁止这个功能，需要根据具体硬件修改
* Input:
* Output:
*************************************************************************/
void SCH_Go_To_Sleep(void)
{
    ;
}

/*************************************************************************
* Name: SCH_Dispatch_Tasks
* Function: 调度程序，任务需要 SCH_Dispatch_Tasks 来运行，被 main 循环调用
* Input:
* Output:
*************************************************************************/
void SCH_Dispatch_Tasks(void)
{
    tByte Index;
    // 调度（运行）下一个任务（如果有任务就绪）
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if (SCH_tasks_G[Index].RunMe > 0)
        {
            (*SCH_tasks_G[Index].pTask)();  // 运行任务
            SCH_tasks_G[Index].RunMe -= 1;  // RunMe 标志复位/减1
            // 周期性的任务将自动的再次运行
            // 如果这是个‘单次’任务，将它从列表中删除
            if (SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
    SCH_Report_Status();  // 报告系统状况
    SCH_Go_To_Sleep();  // 这里调度器进行空闲模式
}

volatile unsigned int num = 0;
/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数, Sch 的刷新函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_IRQHandler(void)
{
    tByte Index;

    if(TIM_GetITStatus(TIM4,TIM_IT_Update))
    {
        // 计算单位为“时标”，只是我们用了 1ms
        for ( Index = 0; Index < SCH_MAX_TASKS; Index++)
        {
            // 检测这里是否有任务
            if (SCH_tasks_G[Index].pTask)
            {
                if (SCH_tasks_G[Index].Delay == 0)
                {
                    // 任务需要运行
                    SCH_tasks_G[Index].RunMe += 1;  // "RunMe" 标准加1
                    if (SCH_tasks_G[Index].Period)
                    {
                        // 调度周期性的任务再次运行
                        SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                    }
                }
                else
                {
                    // 还没有准备好运行，延迟减1
                    SCH_tasks_G[Index].Delay -= 1;
                }
            }
        }
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

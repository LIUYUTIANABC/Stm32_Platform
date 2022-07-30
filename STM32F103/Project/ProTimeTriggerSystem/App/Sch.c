#include "Sch.h"
#include "time.h"
#include "led.h"

//------------------------------------------------------------------------
//- 数据类型 sTask 和常数 SCH_MAX_TASKS 创建任务队列，被调度器引用
//------------------------------------------------------------------------
sTask SCH_tasks_G[SCH_MAX_TASKS];

//------------------------------------------------------------------------
//- 错误代码
//------------------------------------------------------------------------
tByte Error_code_G = 0;

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
}

/*************************************************************************
* Name: SCH_Delete_Task
* Function:
* Input:
* Output:
*************************************************************************/
void SCH_Delete_Task(tByte i)
{
    ;
}

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

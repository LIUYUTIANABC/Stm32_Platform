#ifndef _SCH_H
#define _SCH_H

// 调度器文件
// - 参考网址：https://blog.csdn.net/wowocpp/article/details/82998576

#include "system.h"

typedef unsigned char tByte;
typedef unsigned int tWord;
typedef unsigned long tLong;

//------------------------------------------------------------------------
//- 在程序运行期间的任一时刻允许的任务最大数目
//- 每个新建项目都必须调整
//------------------------------------------------------------------------
#define hSCH_MAX_TASKS (3)

//------------------------------------------------------------------------
//- 定义调度器的数据结构
//- 可能的话，存储在 DATA 区，以供快速存取
//- 每个任务的存储器总和是 7 个字节
//------------------------------------------------------------------------
typedef struct
{
    // 指向任务的指针（必须是一个 “void(void)” 函数）
    void (* pTask)(void);
    // 延迟（时标）直到函数将（下一次）运行
    tWord Delay;
    // 连续运行之间的间隔（时标）
    tWord Period;
    // 当任务需要运行时（由调度器）加 1
    tByte RunMe;
    // 如果任务是合作式的，设置为 1
    // 如果任务是抢占式的，设置为 0
    tByte Co_op;
} sTaskH;

//------------------------------------------------------------------------
//- 公用函数原型
//------------------------------------------------------------------------
void hSCH_Init_TIM4(void);
tByte hSCH_Delete_Task(const tByte TASK_INDEX);
tByte hSCH_Add_Task(void (* pFunction)(), const tWord DELAY, const tWord PERIOD);
void hSCH_Dispatch_Tasks(void);

#endif

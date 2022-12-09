#include "includes.h"

#define  TASK_STK_SIZE   512			//任务堆栈长度

OS_STK   TaskStartStk[TASK_STK_SIZE];		//定义任务堆栈区
INT16S   key;					//用于退出uCOS_II的键
INT8U	 x=0,y=0;				//字符显示位置

void  MyTask(void *data);			//声明一个任务
int main(void)
{
    char* s="M";				//定义要显示的字符

    // OSInit();					//初始化uCOS_II

    // PC_DOSSaveReturn();				//保存Dos环境
    // PC_VectSet(uCOS, OSCtxSw);			//安装uCOS_II中断

    // OSTaskCreate(MyTask,			//创建任务MyTask
	// 	s,				//给任务传递参数
	// 	&TaskStartStk[TASK_STK_SIZE - 1],//设置任务堆栈栈顶指针
	// 	0);				//使任务MyTask的优先级别为0
    // OSStart();					//启动uCOS_II的多任务管理
    // OS_ENTER_CRITICAL();
    // OS_EXIT_CRITICAL();
    printf("%s \n", s);
    return 1;
}



void  MyTask (void *pdata)
{
// #if OS_CRITICAL_METHOD == 3
//     OS_CPU_SR  cpu_sr;
// #endif

//     pdata = pdata;

//     OS_ENTER_CRITICAL();
//     PC_VectSet(0x08, OSTickISR);	//安装uCOS_II时钟中断向量
//     PC_SetTickRate(OS_TICKS_PER_SEC);	//设置uCOS_II时钟频率
//     OS_EXIT_CRITICAL();

//     OSStatInit();			//初始化uCOS_II的统计任务

//     for (;;)
// 	{
//         if (x>10)
// 		{
// 		   x=0;
// 		   y+=2;
// 		}

//         PC_DispChar(x, y,		//在x，y位置显示s中的字符
// 		*(char*)pdata,
// 		DISP_BGND_BLACK+DISP_FGND_WHITE );
//        		x += 1;

// 		//如果按下Esc键则退出uCOS_II
// 		if (PC_GetKey(&key) == TRUE)
// 		{
//             if (key == 0x1B)
// 	    {
//                 PC_DOSReturn();
//             }
//         }

//         OSTimeDlyHMSM(0, 0, 1, 0);	//等待
//     }
}



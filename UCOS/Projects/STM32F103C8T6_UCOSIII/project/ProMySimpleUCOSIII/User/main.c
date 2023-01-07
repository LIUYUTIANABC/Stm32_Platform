/*
*******************************************************************
*                          包含的头文件
*******************************************************************
*/
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "os.h"

/*
*******************************************************************
*                            宏定义
*******************************************************************
*/

/*
*******************************************************************
*                          全局变量
*******************************************************************
*/

uint32_t flag1;
uint32_t flag2;

/*
*******************************************************************
*                        TCB & STACK &任务声明
*******************************************************************
*/

#define TASK1_STK_SIZE 20
#define TASK2_STK_SIZE 20

static CPU_STK Task1Stk[TASK1_STK_SIZE];
static CPU_STK Task2Stk[TASK2_STK_SIZE];

static   OS_TCB    Task1TCB;
static   OS_TCB    Task2TCB;

void     Task1( void *p_arg );
void     Task2( void *p_arg );

/*
*******************************************************************
*                            函数声明
*******************************************************************
*/
void delay(uint32_t count);

/*
*******************************************************************
*                            main()函数
*******************************************************************
*/
/*
* 注意事项：1、该工程使用软件仿真，debug需选择 Ude Simulator
*         2、在Target选项卡里面把晶振Xtal(Mhz)的值改为25，默认是12，
*              改成25是为了跟system_ARMCM3.c中定义的__SYSTEM_CLOCK相同，
*              确保仿真的时候时钟一致
*/
int main(void)
{
    OS_ERR err;

    /* 初始化相关的全局变量 */
    OSInit(&err);

    /* 创建任务 */
    OSTaskCreate ((OS_TCB*)      &Task1TCB,
                (OS_TASK_PTR ) Task1,
                (void *)       0,
                (CPU_STK*)     &Task1Stk[0],
                (CPU_STK_SIZE) TASK1_STK_SIZE,
                (OS_ERR *)     &err);

    OSTaskCreate ((OS_TCB*)      &Task2TCB,
                (OS_TASK_PTR ) Task2,
                (void *)       0,
                (CPU_STK*)     &Task2Stk[0],
                (CPU_STK_SIZE) TASK2_STK_SIZE,
                (OS_ERR *)     &err);

    /* 将任务加入到就绪列表 */
    OSRdyList[0].HeadPtr = &Task1TCB;
    OSRdyList[1].HeadPtr = &Task2TCB;

    /* 启动OS，将不再返回 */
    OSStart(&err);
}

/*
*******************************************************************
*                           函数实现
*******************************************************************
*/
/* 软件延时 */
void delay (uint32_t count)
{
    for (; count!=0; count--);
}

/* 任务1 */
void Task1( void *p_arg )
{
    for ( ;; ) {
        flag1 = 1;
        delay( 100 );
        flag1 = 0;
        delay( 100 );

        /* 任务切换，这里是手动切换 */
        OSSched();
    }
}

/* 任务2 */
void Task2( void *p_arg )
{
    for ( ;; ) {
        flag2 = 1;
        delay( 100 );
        flag2 = 0;
        delay( 100 );

        /* 任务切换，这里是手动切换 */
        OSSched();
    }
}

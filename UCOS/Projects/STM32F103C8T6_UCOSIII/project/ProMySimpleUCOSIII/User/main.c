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
uint32_t flag3;

/* 定义三个全局变量 */
uint32_t TimeStart;
uint32_t TimeEnd;
uint32_t TimeUse;

/*
*******************************************************************
*                        TCB & STACK &任务声明
*******************************************************************
*/
#define  TASK1_STK_SIZE       128
#define  TASK2_STK_SIZE       128
#define  TASK3_STK_SIZE       128

static   CPU_STK   Task1Stk[TASK1_STK_SIZE];
static   CPU_STK   Task2Stk[TASK2_STK_SIZE];
static   CPU_STK   Task3Stk[TASK2_STK_SIZE];

static   OS_TCB    Task1TCB;
static   OS_TCB    Task2TCB;
static   OS_TCB    Task3TCB;

void     Task1( void *p_arg );
void     Task2( void *p_arg );
void     Task3( void *p_arg );

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

    LedInit();

     /* CPU初始化：1、初始化时间戳 */
    CPU_Init();

    /* 关闭中断 */
    CPU_IntDis();

    /* 配置SysTick 10ms 中断一次 */
    OS_CPU_SysTickInit (10);

    /* 初始化相关的全局变量 */
    OSInit(&err);

    /* 创建任务 */
    OSTaskCreate ((OS_TCB*)      &Task1TCB,
                (OS_TASK_PTR ) Task1,
                (void *)       0,
                (OS_PRIO)      1,
                (CPU_STK*)     &Task1Stk[0],
                (CPU_STK_SIZE) TASK1_STK_SIZE,
                (OS_TICK)      0,
                (OS_ERR *)     &err);

    OSTaskCreate ((OS_TCB*)      &Task2TCB,
                (OS_TASK_PTR ) Task2,
                (void *)       0,
                (OS_PRIO)      2,
                (CPU_STK*)     &Task2Stk[0],
                (CPU_STK_SIZE) TASK2_STK_SIZE,
                (OS_TICK)      1,
                (OS_ERR *)     &err);

    OSTaskCreate( (OS_TCB*)      &Task3TCB,
                (OS_TASK_PTR )  Task3,
                (void *)        0,
                (OS_PRIO)       3,
                (CPU_STK*)      &Task3Stk[0],
                (CPU_STK_SIZE)  TASK3_STK_SIZE,
                (OS_TICK)       1,
                (OS_ERR *)      &err );

#if 0
    /* 将任务加入到就绪列表 */
    OSRdyList[0].HeadPtr = &Task1TCB;
    OSRdyList[1].HeadPtr = &Task2TCB;
#endif

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

/* 测试临界段；测量关中断时间 */
void cpuIntTest(void)
{
    static u8 a = 0;
    static CPU_TS_TMR temp = 0;

    CPU_SR_ALLOC();

    a++;
    if (a >= 100)
    {
        a = 0;
        temp = CPU_IntDisMeasMaxGet();
        temp = CPU_IntDisMeasMaxCurReset();
        CPU_CRITICAL_ENTER();
        delay(20000000);
        CPU_CRITICAL_EXIT();
        temp = CPU_IntDisMeasMaxCurGet();
        temp = temp;
    }
}

/* 任务1 */
void Task1( void *p_arg )
{
    OS_ERR err;
    for ( ;; ) {
        // LED_BIT_GPIOC_PIN13 = 1;
        // TimeStart = OS_TS_GET();
        // OSTimeDly(2);
        // TimeEnd = OS_TS_GET();
        // TimeUse = TimeEnd - TimeStart;
        // LED_BIT_GPIOC_PIN13 = 0;
        // OSTimeDly(2);
        // cpuIntTest();

        LED_BIT_GPIOC_PIN13 = 1;
        OSTaskSuspend(&Task1TCB,&err);
        LED_BIT_GPIOC_PIN13 = 0;
        OSTaskSuspend(&Task1TCB,&err);
    }
}

/* 任务2 */
void Task2( void *p_arg )
{
    OS_ERR err;
    u32 i = 0;
    for ( ;; ) {
        LED_BIT_GPIOC_PIN14 = 1;
        OSTimeDly(2);
        // delay(0xFF);
        LED_BIT_GPIOC_PIN14 = 0;
        OSTimeDly(2);
        // delay(0xFF);
        OSTaskResume(&Task1TCB,&err);
        if(i++ > 50)
        {
            i = 0;
            OSTaskDel(&Task3TCB,&err);
        }
    }
}

void Task3( void *p_arg )
{
    for ( ;; ) {
        LED_BIT_GPIOC_PIN15 = 1;
        OSTimeDly(1);
        // delay(0xFF);
        LED_BIT_GPIOC_PIN15 = 0;
        OSTimeDly(1);
        // delay(0xFF);
    }
}

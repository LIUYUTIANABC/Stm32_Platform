#include "led.h"
#include "SysTick.h"

//------------------------------------------------------------------------
//- 私有常量定义
//------------------------------------------------------------------------
// 用于锁定机制
#define LOCKED 1
#define UNLOCKED 0

//------------------------------------------------------------------------
//- 私有变量定义
//------------------------------------------------------------------------
static unsigned char LED_state_G = 0;
static unsigned char LED_lock_G = UNLOCKED;

/*************************************************************************
* Name: LedInit
* Function:
* Input:
* Output:
*************************************************************************/
void LedInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*************************************************************************
* Name: LedDelay
* Function:
* Input:
* Output:
*************************************************************************/
void LedDelay(u32 i)
{
    while (i--)
        ;
}

/*************************************************************************
* Name: LedFlash
* Function:
* Input:
* Output:
*************************************************************************/
void LedFlash(void)
{
    LedDelay(0xFFFFF);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    LED_BIT_GPIOC_PIN14 = !LED_BIT_GPIOC_PIN14;
    LedDelay(0xFFFFF);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    LED_BIT_GPIOC_PIN14 = 1;
}

/*************************************************************************
* Name: Led_Short_Update
* Function: 有一个互斥的变量，该变量的作用是：
*           当 while 中运行的任务正常运行的时候，如果被中断，那么抢占式的任务，会马上退出
* Input:
* Output:
*************************************************************************/
void Led_Short_Update(void)
{
    // 该端口有一个锁
    // 如果被锁定，简单的返回
    if (LED_lock_G == LOCKED)
    {
        return;
    }
    // 端口空闲 - 锁定它
    LED_lock_G = LOCKED;
    // 反转端口
    LED_BIT_GPIOC_PIN14 = !LED_BIT_GPIOC_PIN14;
    // 放开该端口
    LED_lock_G = UNLOCKED;
}


/*************************************************************************
* Name: Led_Long_Update
* Function: 有一个互斥的变量，该变量的作用是：
*           当 while 中运行的任务正常运行的时候，如果被中断，那么抢占式的任务，会马上退出
*           演示长任务，运行时间 10s
* Input:
* Output:
*************************************************************************/
void Led_Long_Update(void)
{
    unsigned char i = 0;
    // 该端口有一个锁
    // 如果被锁定，简单的返回
    if (LED_lock_G == LOCKED)
    {
        return;
    }
    // 端口空闲 - 锁定它
    LED_lock_G = LOCKED;
    // 做长任务
    for (i = 0; i < 5; i++)
    {
        LED_BIT_GPIOC_PIN14 = 1;
        LED_BIT_GPIOC_PIN13 = 1;
        delay_ms(1000);
        LED_BIT_GPIOC_PIN14 = 0;
        LED_BIT_GPIOC_PIN13 = 0;
        delay_ms(1000);
    }
    // 放开该端口
    LED_lock_G = UNLOCKED;
}

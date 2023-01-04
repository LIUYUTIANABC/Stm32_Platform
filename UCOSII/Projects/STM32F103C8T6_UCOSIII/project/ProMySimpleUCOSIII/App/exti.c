#include "exti.h"
#include "SysTick.h"

/*******************************************************************************
 * 函 数 名         : My_EXTI_Init
 * 函数功能		   : 外部中断初始化
 * 输    入         : 无
 * 输    出         : 无
 *******************************************************************************/
void My_EXTI_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //选择GPIO管脚用作外部中断线路
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); //选择GPIO管脚用作外部中断线路
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2); //选择GPIO管脚用作外部中断线路

    // EXTI0 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;          // EXTI0中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

    // EXTI3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;          // EXTI3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

    // EXTI2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;          // EXTI2中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/*******************************************************************************
 * 函 数 名         : EXTI0_IRQHandler
 * 函数功能		   : 外部中断0函数
 * 输    入         : 无
 * 输    出         : 无
 *******************************************************************************/
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == 1)
    {
        delay_ms(10);
        if (KEY_EXTI_EXTI0 == 0)
        {
            LED_EXTI_TEST = !LED_EXTI_TEST;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
 * 函 数 名         : EXTI1_IRQHandler
 * 函数功能		   : 外部中断3函数
 * 输    入         : 无
 * 输    出         : 无
 *******************************************************************************/
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) == 1)
    {
        delay_ms(10);
        if (KEY_EXTI_EXTI1 == 0)
        {
            LED_EXTI_TEST = !LED_EXTI_TEST;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line1);
}

/*******************************************************************************
 * 函 数 名         : EXTI2_IRQHandler
 * 函数功能		   : 外部中断2函数
 * 输    入         : 无
 * 输    出         : 无
 *******************************************************************************/
void EXTI2_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) == 1)
    {
        delay_ms(10);
        if (KEY_EXTI_EXTI2 == 0)
        {
            LED_EXTI_TEST = !LED_EXTI_TEST;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line2);
}

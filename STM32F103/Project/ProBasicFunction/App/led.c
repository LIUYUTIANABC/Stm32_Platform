#include "led.h"

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

#include "led.h"

//------------------------------------------------------------------------
//- Public variable definitions
//------------------------------------------------------------------------
extern unsigned char Sw_pressed_G;

//------------------------------------------------------------------------
//- 私有变量定义
//------------------------------------------------------------------------
static unsigned char LED_state_G = 0;

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
 * Name: LedFlashUpdate
 * Function:
 * Input:
 * Output:
 *************************************************************************/
void LedFlashUpdate(void)
{
    // Do nothing if switch is not pressed
    if (0 == Sw_pressed_G)
    {
        return;
    }

    // Change the LED from OFF to ON (or vice versa)
    if (LED_state_G == 0)
    {
        LED_state_G = 1;
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        LED_BIT_GPIOC_PIN14 = 0;
    }
    else
    {
        LED_state_G = 0;
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        LED_BIT_GPIOC_PIN14 = 1;
    }
}

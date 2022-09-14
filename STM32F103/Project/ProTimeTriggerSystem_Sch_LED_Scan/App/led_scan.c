#include "led_scan.h"

// ------ Public variable definitions ------------------------------

// Lookup table - stored in code area
// See Port.H for connections and code details

u8 code LED_Table_G[20] =
    // 0     1     2     3     4     5     6     7     8     9
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
     // 0.    1.    2.    3.    4.    5.    6.    7.    8.    9.
     0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

// Global data formatted for display (initially 0,0,0,0)
u8 LED_Mx4_Data_G[4] = {0x3F, 0x3F, 0x3F, 0x3F};

// ------ Private variable definitions------------------------------

static u8 Digit_G;

/*************************************************************************
 * Name: LedScanInit
 * Function:
 * Input:
 * Output:
 *************************************************************************/
void LedScanInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
                                  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*------------------------------------------------------------------*-

  LED_MX4_Display_Update()

  Updates (four) multiplexed 8-segment LED displays.

  Usually aim to scheduled at around 3ms intervals: typically
  around a 1% CPU load on standard 8051.

-*------------------------------------------------------------------*/
void LED_MX4_Display_Update(void)
{
    // Increment the digit to be displayed
    if (++Digit_G == LED_NUM_DIGITS)
    {
        Digit_G = 0;
    }

    // Allows any pins to be used
    switch (Digit_G)
    {
    case 0:
    {
        LED_DIGIT_0 = 0;
        LED_DIGIT_1 = 0;
        LED_DIGIT_2 = 0;
        LED_DIGIT_3 = 1;
        break;
    }

    case 1:
    {
        LED_DIGIT_0 = 0;
        LED_DIGIT_1 = 0;
        LED_DIGIT_2 = 1;
        LED_DIGIT_3 = 0;
        break;
    }

    case 2:
    {
        LED_DIGIT_0 = 0;
        LED_DIGIT_1 = 1;
        LED_DIGIT_2 = 0;
        LED_DIGIT_3 = 0;
        break;
    }

    case 3:
    {
        LED_DIGIT_0 = 1;
        LED_DIGIT_1 = 0;
        LED_DIGIT_2 = 0;
        LED_DIGIT_3 = 0;
    }
    }

    LED_DATA_PORT = 255 - LED_Mx4_Data_G[Digit_G];
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

#include "led_scan.h"

// ------ Public variable definitions ------------------------------

// Lookup table - stored in code area
// See Port.H for connections and code details

const u8 LED_Table_G[20] =
    // 0     1     2     3     4     5     6     7     8     9
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
     // 0.    1.    2.    3.    4.    5.    6.    7.    8.    9.
     0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

// Global data formatted for display (initially 0,0,0,0)
u8 LED_Mx4_Data_G[4] = {0x3F, 0x3F, 0x3F, 0x3F};

// ------ Private variable definitions------------------------------

static u8 Digit_G;

// Time variables
static tByte Hou_G, Min_G, Sec_G;

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

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
                                  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                                  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
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
    u16 l_LedDataTemp = 0;
    u16 l_LedDataTemp_1 = 0;
    u16 l_LedDataTemp_2 = 0;
    u16 l_LedDataTemp_3 = 0;

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

    // LED_DATA_PORT = 255 - LED_Mx4_Data_G[Digit_G];
    l_LedDataTemp_3 = GPIO_ReadOutputData(GPIOB);
    l_LedDataTemp_1 = (u8)GPIO_ReadOutputData(GPIOB);
    l_LedDataTemp_2 = ((u16)(0xFF - LED_Mx4_Data_G[Digit_G]) << 8);
    l_LedDataTemp = ((u16)(0xFF - LED_Mx4_Data_G[Digit_G]) << 8) + (u8)GPIO_ReadOutputData(GPIOB);
    GPIO_Write(GPIOB, l_LedDataTemp);
}

/*------------------------------------------------------------------*-

  CLOCK_LED_Time_Update()

  Updates the global time variables.

  *** Must be scheduled once per second ***

-*------------------------------------------------------------------*/
void CLOCK_LED_Time_Update(void)
{
    u8 Min_update = 0;
    u8 Hou_update = 0;

    if (++Sec_G == 60)
    {
        Sec_G = 0;
        Min_update = 1;

        if (++Min_G == 60)
        {
            Min_G = 0;
            Hou_update = 1;

            if (++Hou_G == 24)
            {
                Hou_G = 0;
            }
        }
    }

    if (Min_update)
    {
        // Need to update the minutes data
        // (both digits)
        LED_Mx4_Data_G[1] = LED_Table_G[Min_G / 10];
        LED_Mx4_Data_G[0] = LED_Table_G[Min_G % 10];
    }

    // We don't display seconds in this version.
    // We simply use the seconds data to turn on and off the decimal
    // point between hours and minutes
    if ((Sec_G % 2) == 0)
    {
        LED_Mx4_Data_G[2] = LED_Table_G[Hou_G % 10];
    }
    else
    {
        LED_Mx4_Data_G[2] = LED_Table_G[(Hou_G % 10) + 10];
    }

    if (Hou_update)
    {
        // Need to update the 'tens of hours' data
        LED_Mx4_Data_G[3] = LED_Table_G[Hou_G / 10];
    }
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

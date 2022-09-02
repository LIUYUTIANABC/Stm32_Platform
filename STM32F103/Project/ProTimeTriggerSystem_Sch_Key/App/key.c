#include "key.h"

//------------------------------------------------------------------------
//- Public variable definitions
//------------------------------------------------------------------------
unsigned char Sw_pressed_G = 0; // The current switch status

//------------------------------------------------------------------------
//- 私有变量定义
//------------------------------------------------------------------------
// static unsigned char LED_state_G = 0;

//----------------------------------------------------------------------------------
// - Private constants
//----------------------------------------------------------------------------------

// Allows NO or NC switch to be used (or other wiring variations)
#define SW_PRESSED (0)

// SW_THRES must be > 1 for correct debounce behaviour
#define SW_THRES (3)

/*************************************************************************
 * Name: KeyInit
 * Function:
 * Input:
 * Output:
 *************************************************************************/
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*------------------------------------------------------------------*-

  SWITCH_Update()

  This is the main switch function.

  It should be scheduled every 50 - 500 ms.

-*------------------------------------------------------------------*/
void SWITCH_Update(void)
{
    static u8 Duration;

    if (KEY_PIN == SW_PRESSED)
    {
        Duration += 1;

        if (Duration > SW_THRES)
        {
            Duration = SW_THRES;

            Sw_pressed_G = 1; // Switch is pressed...
            return;
        }

        // Switch pressed, but not yet for long enough
        Sw_pressed_G = 0;
        return;
    }

    // Switch not pressed - reset the count
    Duration = 0;
    Sw_pressed_G = 0; // Switch not pressed...
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

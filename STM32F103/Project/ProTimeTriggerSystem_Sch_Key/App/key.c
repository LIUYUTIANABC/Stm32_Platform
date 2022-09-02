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

//----------------------------------------------------------------------------------
// No_Off switch function
//----------------------------------------------------------------------------------

// ------ Private variables ----------------------------------------

static u8 Sw_press_duration_G = 0;
static u8 Sw_blocked_G = 0;

/*------------------------------------------------------------------*-

  FUNCTION: SWITCH_ON_OFF_Update()

  This is the main on-off switch function.

  It should be scheduled every 50 - 500 ms.

-*------------------------------------------------------------------*/
void SWITCH_ON_OFF_Update(void)
{
    // If the switch is blocked, decrement the count and return
    // without checking the switch pin status.
    // This is done to give the user time to remove their finger
    // from the switch - otherwise if they keep their finger on
    // the switch for more than 0.4s the light will switch off again.

    if (Sw_blocked_G)
    {
        Sw_blocked_G--;
        return;
    }

    if (KEY_PIN == SW_PRESSED)
    {
        Sw_press_duration_G += 1;

        if (Sw_press_duration_G > SW_THRES)
        {
            Sw_press_duration_G = SW_THRES;

            // Change switch state
            if (Sw_pressed_G == 1)
            {
                Sw_pressed_G = 0; // Switch state changed to OFF
            }
            else
            {
                Sw_pressed_G = 1; // Switch state changed to ON
            }

            // Allow no other changes for ~1 second
            Sw_blocked_G = 5;
            return;
        }

        // Switch pressed, but not yet for long enough
        return;
    }

    // Switch not pressed - reset the count
    Sw_press_duration_G = 0;
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

//----------------------------------------------------------------------------------
// Multi_s
//----------------------------------------------------------------------------------

// ------ Public variables -----------------------------------------

u8 Sw_status_G; // The current switch status

// SW_THRES must be > 1 for correct debounce behaviour
#define SW_Multi_THRES (1)
#define SW_Multi_THRES_X2 (SW_THRES + SW_THRES + SW_THRES + SW_THRES)
#define SW_Multi_THRES_X3 (SW_THRES_X2 + SW_THRES_X2)

/*------------------------------------------------------------------*-

  SWITCH_MS_Update()

  This is the main switch function.  It should be scheduled every
  50 - 500 ms.

  Alters Sw_press_duration_G depending on duration of switch press.

-*------------------------------------------------------------------*/
void SWITCH_MS_Update(void)
{
    if (KEY_PIN == SW_PRESSED)
    {
        Sw_press_duration_G += 1;

        if (Sw_press_duration_G > (SW_Multi_THRES_X3))
        {
            Sw_press_duration_G = SW_Multi_THRES_X3;
            Sw_status_G = 3; // Switch has been pressed for a long time...
            return;
        }

        if (Sw_press_duration_G > (SW_Multi_THRES_X2))
        {
            Sw_status_G = 2; // Switch has been pressed for a medium time...
            return;
        }

        // SW_THRES must be > 1 for software debounce
        if (Sw_press_duration_G > SW_Multi_THRES)
        {
            Sw_status_G = 1; // Switch has been pressed for a short time...
            return;
        }

        // switch pressed, but not yet for long enough
        Sw_status_G = 0;
        return;
    }

    // Switch not pressed - reset the count
    Sw_press_duration_G = 0;
    Sw_status_G = 0; // Switch not pressed...
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

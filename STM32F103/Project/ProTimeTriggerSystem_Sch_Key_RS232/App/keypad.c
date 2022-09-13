/*------------------------------------------------------------------*-

   Keypad.C (v1.00)

  ------------------------------------------------------------------

   Simple keypad library, for a 3-column x 4-row keypad.

   Key arrangement is:   ---------
                          1  2  3
                          4  5  6
                          7  8  9
                          *  0  #
                         ---------

   Supports two function keys ('*' and '#').

   See Chapter 20 for details.


   COPYRIGHT
   ---------

   This code is from the book:

   PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont
   [Pearson Education, 2001; ISBN: 0-201-33138-1].

   This code is copyright (c) 2001 by Michael J. Pont.

   See book for copyright details and other information.

-*------------------------------------------------------------------*/

#include "pc_rs232.h"
#include "usart.h"
#include "SysTick.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// ------ Private function prototypes ------------------------------

u8 KEYPAD_Scan(u8 *const, u8 *const);

// ------ Private constants ----------------------------------------

#define KEYPAD_RECV_BUFFER_LENGTH 6

// Any valid character will do - must not match anything on keypad
#define KEYPAD_NO_NEW_DATA (u8)'-'

// ------ Private variables ----------------------------------------

static u8 KEYPAD_recv_buffer[KEYPAD_RECV_BUFFER_LENGTH + 1][2];

static u8 KEYPAD_in_read_index;    // Data in buffer that has been read
static u8 KEYPAD_in_waiting_index; // Data in buffer not yet read

static u8 Last_valid_key_G = KEYPAD_NO_NEW_DATA;

/*------------------------------------------------------------------*-

  KEYPAD_Init()

  Init the keypad.

-*------------------------------------------------------------------*/
void KEYPAD_Init(void)
{
    // MCU IO init
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    KEYPAD_in_read_index = 0;
    KEYPAD_in_waiting_index = 0;
}

/*------------------------------------------------------------------*-

  KEYPAD_Update()

  The main 'update' function for the keypad library.

  Must schedule this function (approx every 50 - 200 ms).

-*------------------------------------------------------------------*/
void KEYPAD_Update(void)
{
    u8 Key, FnKey;

    // Scan keypad here...
    if (KEYPAD_Scan(&Key, &FnKey) == 0)
    {
        // No new key data - just return
        return;
    }

    // Want to read into index 0, if old data has been read
    // (simple ~circular buffer)
    if (KEYPAD_in_waiting_index == KEYPAD_in_read_index)
    {
        KEYPAD_in_waiting_index = 0;
        KEYPAD_in_read_index = 0;
    }

    // Load keypad data into buffer
    KEYPAD_recv_buffer[KEYPAD_in_waiting_index][0] = Key;
    KEYPAD_recv_buffer[KEYPAD_in_waiting_index][1] = FnKey;

    if (KEYPAD_in_waiting_index < KEYPAD_RECV_BUFFER_LENGTH)
    {
        // Increment without overflowing buffer
        KEYPAD_in_waiting_index++;
    }
}

/*------------------------------------------------------------------*-

  KEYPAD_Get_Char_From_Buffer()

  The Update function copies data into the keypad buffer.
  This extracts data from the buffer.

-*------------------------------------------------------------------*/

u8 KEYPAD_Get_Data_From_Buffer(u8 *pKey, u8 *pFuncKey)
{
    // If there is new data in the buffer
    if (KEYPAD_in_read_index < KEYPAD_in_waiting_index)
    {
        *pKey = KEYPAD_recv_buffer[KEYPAD_in_read_index][0];
        *pFuncKey = KEYPAD_recv_buffer[KEYPAD_in_read_index][1];

        KEYPAD_in_read_index++;

        return 1;
    }

    return 0;
}

/*------------------------------------------------------------------*-

  KEYPAD_Clear_Buffer()

-*------------------------------------------------------------------*/
void KEYPAD_Clear_Buffer(void)
{
    KEYPAD_in_waiting_index = 0;
    KEYPAD_in_read_index = 0;
}

/*------------------------------------------------------------------*-

  KEYPAD_Scan()

  This function is called from scheduled keypad function.

  Must be edited as required to match your key labels.

  Includes two 'function keys' which may be used simultaneously
  with keys from any other column.

  Adapt as required!

-*------------------------------------------------------------------*/
u8 KEYPAD_Scan(u8 *pKey, u8 *pFuncKey)
{
    static u8 Old_Key;

    u8 Key = KEYPAD_NO_NEW_DATA;
    u8 Fn_key = (u8)0x00;

    KEY_C_0 = 0; // Scanning column 1
    delay_us(5);
    if (KEY_R_3 == 0)
        Key = '1';
    if (KEY_R_4 == 0)
        Key = '4';
    if (KEY_R_5 == 0)
        Key = '7';
    if (KEY_R_6 == 0)
        Fn_key = '*';
    KEY_C_0 = 1;

    KEY_C_1 = 0; // Scanning column 2
    delay_us(5);
    if (KEY_R_3 == 0)
        Key = '2';
    if (KEY_R_4 == 0)
        Key = '5';
    if (KEY_R_5 == 0)
        Key = '8';
    if (KEY_R_6 == 0)
        Key = '0';
    KEY_C_1 = 1;

    KEY_C_2 = 0; // Scanning column 3
    delay_us(5);
    if (KEY_R_3 == 0)
        Key = '3';
    if (KEY_R_4 == 0)
        Key = '6';
    if (KEY_R_5 == 0)
        Key = '9';
    if (KEY_R_6 == 0)
        Fn_key = '#';
    KEY_C_2 = 1;

    if (Key == KEYPAD_NO_NEW_DATA)
    {
        // No key pressed (or just a function key)
        Old_Key = KEYPAD_NO_NEW_DATA;
        Last_valid_key_G = KEYPAD_NO_NEW_DATA;

        return 0; // No new data
    }

    // A key has been pressed: debounce by checking twice
    if (Key == Old_Key)
    {
        // A valid (debounced) key press has been detected

        // Must be a new key to be valid - no 'auto repeat'
        if (Key != Last_valid_key_G)
        {
            // New key!
            *pKey = Key;
            Last_valid_key_G = Key;

            // Is the function key pressed too?
            if (Fn_key)
            {
                // Function key *is* pressed with another key
                *pFuncKey = Fn_key;
            }
            else
            {
                *pFuncKey = (u8)0x00;
            }

            return 1;
        }
    }

    // No new data
    Old_Key = Key;
    return 0;
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

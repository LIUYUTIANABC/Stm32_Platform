#include "pc_rs232.h"
#include "usart.h"
#include "keypad.h"

// ------ Private variables ----------------------------------------

u8 Count_G;

/*------------------------------------------------------------------*-

  Keypad_RS232_Init()

  Init function for simple library displaying keypad inputs
  over serial link.

-*------------------------------------------------------------------*/
void Keypad_RS232_Init(void)
{
    PC_LINK_IO_Write_String_To_Buffer("Keypad test code - READY\n");

    Count_G = 0;

    KEYPAD_Clear_Buffer();
}

/*------------------------------------------------------------------*-

  Keypad_RS232_Update()

  Function for displaying keypad inputs over serial link.

-*------------------------------------------------------------------*/
void Keypad_RS232_Update(void)
{
    u8 Key, FnKey;

    // Update the keypad buffer
    KEYPAD_Update();

    // Is there any new data in the keypad buffer?
    if (KEYPAD_Get_Data_From_Buffer(&Key, &FnKey) == 0)
    {
        // No new data.
        return;
    }

    // Function key has been pressed (with another key)
    if (FnKey)
    {
        PC_LINK_IO_Write_Char_To_Buffer('\n');
        PC_LINK_IO_Write_Char_To_Buffer(FnKey);
        PC_LINK_IO_Write_Char_To_Buffer(Key);
        PC_LINK_IO_Write_Char_To_Buffer('\n');

        Count_G = 0;
    }
    else
    {
        // An ordinary key (no function key) has been pressed
        PC_LINK_IO_Write_Char_To_Buffer(Key);

        if (++Count_G == 10)
        {
            PC_LINK_IO_Write_Char_To_Buffer('\n');
            Count_G = 0;
        }
    }
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

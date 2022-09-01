#include "pc_rs232.h"
#include "usart.h"

/*------------------------------------------------------------------*-

  MENU_Command_Processor()

  This function is the main menu 'command processor' function.

  Schedule this (say) once every 10 ms (approx.).

-*------------------------------------------------------------------*/
void MENU_Command_Processor(void)
{
    static u8 First_time_only = 0;
    u8 Ch;

    if (First_time_only == 0)
    {
        First_time_only = 1;
        MENU_Show_Menu();
    }

    // Check for user inputs
    PC_LINK_IO_Update();

    Ch = PC_LINK_IO_Get_Char_From_Buffer();

    if (Ch != PC_LINK_IO_NO_CHAR)
    {
        MENU_Perform_Task(Ch);
        MENU_Show_Menu();
    }
}

/*------------------------------------------------------------------*-

  MENU_Show_Menu()

  Display menu options on PC screen (via serial link)
  - edit as required to meet the needs of your application.

-*------------------------------------------------------------------*/
void MENU_Show_Menu(void)
{
    PC_LINK_IO_Write_String_To_Buffer("Menu:\n");
    PC_LINK_IO_Write_String_To_Buffer("a - x\n");
    PC_LINK_IO_Write_String_To_Buffer("b - x\n");
    PC_LINK_IO_Write_String_To_Buffer("c - x\n\n");
    PC_LINK_IO_Write_String_To_Buffer("? : ");
}

/*------------------------------------------------------------------*-

  MENU_Perform_Task()

  Perform the required user task
  - edit as required to match the needs of your application.

-*------------------------------------------------------------------*/
void MENU_Perform_Task(u8 c)
{
    // Echo the menu option
    PC_LINK_IO_Write_Char_To_Buffer(c);
    PC_LINK_IO_Write_Char_To_Buffer('\n');

    // Perform the task
    switch (c)
    {
    case 'a':
    case 'A':
    {
        Function_A();
        break;
    }

    case 'b':
    case 'B':
    {
        Function_B();
        break;
    }

    case 'c':
    case 'C':
    {
        Function_C();
    }
    }
}

/*------------------------------------------------------------------*-

  Placeholder function

-*------------------------------------------------------------------*/
void Function_A(void)
{
    PC_LINK_IO_Write_String_To_Buffer("\n* Doing A *\n\n");
}

/*------------------------------------------------------------------*-

  Placeholder function

-*------------------------------------------------------------------*/
void Function_B(void)
{
    PC_LINK_IO_Write_String_To_Buffer("\n* Doing B *\n\n");
}

/*------------------------------------------------------------------*-

  Placeholder function

-*------------------------------------------------------------------*/
void Function_C(void)
{
    PC_LINK_IO_Write_String_To_Buffer("\n* Doing C *\n\n");
}

// ------ Public variable definitions ------------------------------

u8 Hou_G = 0;
u8 Min_G = 0;
u8 Sec_G = 0;

/*------------------------------------------------------------------*-

  Elapsed_Time_RS232_Update()

  Function for displaying elapsed time on PC Screen.

  *** Must be scheduled once per second ***

-*------------------------------------------------------------------*/
void Elapsed_Time_RS232_Update(void)
{
    u8 Time_Str[30] = "\rElapsed time:               ";

    if (++Sec_G == 60)
    {
        Sec_G = 0;

        if (++Min_G == 60)
        {
            Min_G = 0;

            if (++Hou_G == 24)
            {
                Hou_G = 0;
            }
        }
    }

    Time_Str[15] = CHAR_MAP_G[Hou_G / 10];
    Time_Str[16] = CHAR_MAP_G[Hou_G % 10];

    Time_Str[18] = CHAR_MAP_G[Min_G / 10];
    Time_Str[19] = CHAR_MAP_G[Min_G % 10];

    Time_Str[21] = CHAR_MAP_G[Sec_G / 10];
    Time_Str[22] = CHAR_MAP_G[Sec_G % 10];

    // We don't display seconds in this version.
    // We simply use the seconds data to turn on and off the colon
    // (between hours and minutes)
    if ((Sec_G % 2) == 0)
    {
        Time_Str[17] = ':';
        Time_Str[20] = ':';
    }
    else
    {
        Time_Str[17] = ' ';
        Time_Str[20] = ' ';
    }

    PC_LINK_IO_Write_String_To_Buffer(Time_Str);
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

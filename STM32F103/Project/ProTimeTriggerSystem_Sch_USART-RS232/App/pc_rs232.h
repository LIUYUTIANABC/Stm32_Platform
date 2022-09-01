#ifndef _PC_RS232_H
#define _PC_RS232_H

#include "system.h"

// ------ Public function prototypes -------------------------------

void MENU_Command_Processor(void);

void MENU_Show_Menu(void);
void MENU_Perform_Task(u8 c);

void Function_A(void);
void Function_B(void);
void Function_C(void);


// ------ Public constants -----------------------------------------

const u8 CHAR_MAP_G[10] = {'0','1','2','3','4','5','6','7','8','9'};

void Elapsed_Time_RS232_Update(void);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

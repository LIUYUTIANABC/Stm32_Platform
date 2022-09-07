#ifndef _USART_H
#define _USART_H

#include "system.h"

// ------ Public constants -----------------------------------------

// Value returned by PC_LINK_Get_Char_From_Buffer if no character is
// available in buffer
#define PC_LINK_IO_NO_CHAR 127

// ------ Public function prototypes -------------------------------

void PC_LINK_IO_Write_String_To_Buffer(u8 *STR_PTR);
void PC_LINK_IO_Write_Char_To_Buffer(u8 CHARACTER);

u8 PC_LINK_IO_Get_Char_From_Buffer(void);

void PC_LINK_IO_Send_Char(u8 CHARACTER);

// Must schedule or otherwise regularly call this function...
void PC_LINK_IO_Update(void);

void Usart1NoInterruptInit(u16 BAUD_RATE);
void UsartDebug(void);
void UsartDebugRx(void);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "system.h"

#define KEY_C_0 PBout(0)
#define KEY_C_1 PBout(1)
#define KEY_C_2 PBout(4)

#define KEY_R_3 PBin(5)
#define KEY_R_4 PBin(6)
#define KEY_R_5 PBin(7)
#define KEY_R_6 PBin(8)

// ------ Public function prototypes -------------------------------

void KEYPAD_Init(void);
void KEYPAD_Update(void);
u8 KEYPAD_Get_Data_From_Buffer(u8 *pKey, u8 *pFuncKey);
void KEYPAD_Clear_Buffer(void);
u8 KEYPAD_Scan(u8 *pKey, u8 *pFuncKey);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

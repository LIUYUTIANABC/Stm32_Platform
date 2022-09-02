#ifndef _KEY_H
#define _KEY_H

#include "system.h"

#define KEY_PIN PCin(15)

// ------ Public function prototypes -------------------------------
void KeyInit(void);
void SWITCH_Update(void);

#endif

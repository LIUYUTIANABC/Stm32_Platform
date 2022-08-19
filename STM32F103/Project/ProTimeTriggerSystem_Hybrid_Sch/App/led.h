#ifndef _LED_H
#define _LED_H

#include "system.h"

#define LED_BIT_GPIOC_PIN14 PCout(14)
#define LED_BIT_GPIOC_PIN13 PCout(13)

void LedInit(void);
void LedFlash(void);
void Led_Short_Update(void);
void Led_Long_Update(void);

#endif

#ifndef _LED_H
#define _LED_H

#include "system.h"

#define LED_BIT_GPIOC_PIN14 PCout(14)

void LedInit(void);
void LedFlash(void);

#endif

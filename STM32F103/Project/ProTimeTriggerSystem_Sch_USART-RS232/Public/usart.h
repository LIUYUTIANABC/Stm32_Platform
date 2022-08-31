#ifndef _USART_H
#define _USART_H

#include "system.h"

void Usart1NoInterruptInit(u16 BAUD_RATE);
void UsartDebug(void);
void UsartDebugRx(void);

#endif

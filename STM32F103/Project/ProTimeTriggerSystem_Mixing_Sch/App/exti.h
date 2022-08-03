#ifndef _EXTI_H
#define _EXTI_H

#include "system.h"

#define KEY_EXTI_EXTI0 PAin(0)
#define KEY_EXTI_EXTI1 PAin(1)
#define KEY_EXTI_EXTI2 PAin(2)
#define LED_EXTI_TEST PAout(3)

void My_EXTI_Init(void);

#endif

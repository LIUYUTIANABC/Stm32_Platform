#include "stm32f10x.h"
#include "RegisterTemplate.h"
#include "led.h"

#undef REGISTER_LED_FEATURE
#define LED_FEATURE

/*************************************************************************
 * Name: main
 * Function:
 * Input:
 * Output:
 *************************************************************************/
int main(void)
{
#ifdef REGISTER_LED_FEATURE
    RegLedFlash();
#endif
#ifdef LED_FEATURE
    LedInit();
#endif

    while (1)
    {
        #ifdef LED_FEATURE
            LedFlash();
        #endif
    }
}

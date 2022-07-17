#include "stm32f10x.h"
#include "RegisterTemplate.h"

#undef REGISTER_LED_FEATURE

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

    while (1)
    {
        ;
    }
}

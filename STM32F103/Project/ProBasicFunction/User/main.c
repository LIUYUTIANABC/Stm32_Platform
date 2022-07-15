#include "stm32f10x.h"
#include "RegisterTemplate.h"

#define REGISTER_LED_FEATURE

/*************************************************************************
 * Name: SystemInit
 * Function: Set System clock and AHB、APB clock
 * Input:
 * Output:
 *************************************************************************/
void SystemInit()
{
}

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

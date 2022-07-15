#include "RegisterTemplate.h"

//------------------------------------------------------------------------
// define GPIOC register
//------------------------------------------------------------------------
#define GPIOC_CRL *(unsigned int *)(GPIOC_BASE + 0x00)
#define GPIOC_CRH *(unsigned int *)(GPIOC_BASE + 0x04)
#define GPIOC_IDR *(unsigned int *)(GPIOC_BASE + 0x08)
#define GPIOC_ODR *(unsigned int *)(GPIOC_BASE + 0x0C)
#define GPIOC_BSRR *(unsigned int *)(GPIOC_BASE + 0x10)
#define GPIOC_BRR *(unsigned int *)(GPIOC_BASE + 0x14)
#define GPIOC_LCKR *(unsigned int *)(GPIOC_BASE + 0x18)

#define RCC_APB2ENR *(unsigned int *)(RCC_BASE + 0x18)

/*************************************************************************
 * Name: Reg_Delay
 * Function:
 * Input:
 * Output:
 *************************************************************************/
void Reg_Delay(u32 i)
{
    while (i--)
        ;
}

/*************************************************************************
 * Name: RegLedFlash
 * Function:
 * Input:
 * Output:
 *************************************************************************/
void RegLedFlash(void)
{
    // Enable RCC GPIOC Periph
    RCC_APB2ENR |= 1 << 4;
    // Set GPIOC_PIN13 PP output
    GPIOC_CRH &= ~(0x0F << (4 * 5));
    GPIOC_CRH |= (3 << 4 * 5);
    // Set output low level
    GPIOC_BSRR = (1 << (16 + 13));

    while (1)
    {
        GPIOC_BSRR = (1 << (16 + 13));
        Reg_Delay(0xFFFFF);
        GPIOC_BSRR = (1 << (13));
        Reg_Delay(0xFFFFF);
    }
}

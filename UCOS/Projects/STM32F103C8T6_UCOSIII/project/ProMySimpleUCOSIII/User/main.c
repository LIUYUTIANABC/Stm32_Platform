#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "cpu.h"

#define TASK1_STK_SIZE 128
#define TASK2_STK_SIZE 128

static CPU_STK Task1Stk[TASK1_STK_SIZE];
static CPU_STK Task2Stk[TASK2_STK_SIZE];

uint32_t flag1;
uint32_t flag2;

void delay(uint32_t count)
{
    for (; count != 0; count--)
        ;
}

int main(void)
{
    for (;;)
    {
        ;
    }
}

/* 任务1 */
void Task1(void *p_arg)
{
    for (;;)
    {
        flag1 = 1;
        delay(100);
        flag1 = 0;
        delay(100);
    }
}

/* 任务2 */
void Task2(void *p_arg)
{
    for (;;)
    {
        flag2 = 1;
        delay(100);
        flag2 = 0;
        delay(100);
    }
}

#include "SysTick.h"
#include "misc.h"

static u8 fac_us = 0;  // us 延时倍乘数
static u16 fac_ms = 0; // ms 延时倍乘数

/*******************************************************************************
 * 函 数 名 : SysTick_Init * 函数功能：
 *            SysTick 初始化，SYSTICK 的时钟固定为 AHB 时钟的 1/8
 * 输 入 : SYSCLK:系统时钟频率
 * 输 出 : 无
 *******************************************************************************/
void SysTick_Init(u8 SYSCLK)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SYSCLK / 8;         // SYSCLK的 8分频 保存 1us所需的计数次数
    fac_ms = (u16)fac_us * 1000; //每个 ms 需要的 systick 时钟数
}

/*******************************************************************************
* 函 数 名 : delay_us * 函数功能 : us 延时，
* 输 入 : nus：要延时的 us 数
* 注 意:
    nus 的值, 不要大于 1,864,135us (最大值即 2^24/fac_us @fac_us=9)
* 输 出 : 无
*******************************************************************************/
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; //时间加载
    SysTick->VAL = 0x00;          //清空计数器
    SysTick->CTRL |= 0x01;        //开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL &= ~0x01;                         //关闭计数器
    SysTick->VAL = 0X00;                            //清空计数器
}

/*******************************************************************************
* 函 数 名 : delay_ms
* 函数功能 : ms 延时，
* 输 入 : nms：要延时的 ms 数
* 注 意:
    nms 的值,SysTick->LOAD 为 24 位寄存器，不要大于 0xffffff*8*1000/SYSCLK
    对 72M 条件下,nms <= 1864ms
* 输 出 : 无
*******************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms; // 时 间 加 载(SysTick->LOAD 为 24bit)
    SysTick->VAL = 0x00;               //清空计数器
    SysTick->CTRL |= 0x01;             //开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL &= ~0x01;                         //关闭计数器
    SysTick->VAL = 0X00;                            //清空计数器
}

#include "system.h"
#include "RegisterTemplate.h"
#include "led.h"
#include "SysTick.h"
#include "exti.h"
#include "time.h"
#include "iwdg.h"

#undef REGISTER_LED_FEATURE
#undef FEATURE_LED
#define FEATURE_SYSTICK
#define FEATURE_EXTI
#define FEATURE_TIME
#define FEATURE_IWDG

void RCC_HSE_Config(u32 div, u32 pllm);

/*************************************************************************
 * Name: main
 * Function:
 * Input:
 * Output:
 *************************************************************************/
int main(void)
{
    RCC_HSE_Config(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#ifdef REGISTER_LED_FEATURE
    RegLedFlash();
#endif
#ifdef FEATURE_LED
    LedInit();
#endif
#ifdef FEATURE_SYSTICK
    SysTick_Init(72);  // 72 是现在的系统时钟是 72MHZ
    LedInit();
#endif
#ifdef FEATURE_EXTI
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组 分 2 组
    My_EXTI_Init();
#endif
#ifdef FEATURE_TIME
    TIM4_Init(1000, 36000-1); // 500ms 定时器 TIM4 中断
#endif
#ifdef FEATURE_IWDG
    IWDG_Init(4, 800);  // 看门狗时钟是 1280ms
#endif

    delay_ms(800);

    while (1)
    {
        #ifdef FEATURE_IWDG
        IWDG_FeedDog();
        #endif
        #ifdef FEATURE_LED
        LedFlash();
        #endif
        #ifdef FEATURE_SYSTICK
        LED_BIT_GPIOC_PIN14 = !LED_BIT_GPIOC_PIN14;
        delay_ms(100);
        __NOP;
        #endif
    }
}

/*******************************************************************************
* 函 数 名 : RCC_HSE_Config
* 函数功能 : 自定义系统时钟，可以通过修改 PLL 时钟源和倍频系数实现时钟调整
* 输 入 : div：RCC_PLLSource_HSE_Div1/RCC_PLLSource_HSE_Div2
          pllm：RCC_PLLMul_2-RCC_PLLMul_16
* 输 出 : 无
系统默认时钟：RCC_HSE_Config(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
系统时钟 36MHZ：RCC_HSE_Config(RCC_PLLSource_HSE_Div2,RCC_PLLMul_9);
*******************************************************************************/
void RCC_HSE_Config(u32 div, u32 pllm) //自定义系统时间（可以修改时钟）
{
    RCC_ClocksTypeDef get_rcc_clock;    // 用来查看系统时钟

    RCC_DeInit();                           //将外设 RCC 寄存器重设为缺省值
    RCC_HSEConfig(RCC_HSE_ON);              //设置外部高速晶振（HSE）
    if (RCC_WaitForHSEStartUp() == SUCCESS) //等待 HSE 起振
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1); //设置 AHB 时钟（HCLK）
        RCC_PCLK1Config(RCC_HCLK_Div2);  //设置低速 AHB 时钟（PCLK1）
        RCC_PCLK2Config(RCC_HCLK_Div1);  //设置高速 AHB 时钟（PCLK2）
        RCC_PLLConfig(div, pllm);        //设置 PLL 时钟源及倍频系数
        RCC_PLLCmd(ENABLE);              //使能或者失能 PLL
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
            ;                                      //检查指定的 RCC 标志位设置与否,PLL 就绪
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // 设 置 系 统 时 钟（SYSCLK）
        while (RCC_GetSYSCLKSource() != 0x08)
            ; //返回用作系统时钟的时钟源,0x08：PLL 作为系统时钟
    }
    RCC_GetClocksFreq(&get_rcc_clock);    // 获取系统时钟在结构体中
    __NOP();
}

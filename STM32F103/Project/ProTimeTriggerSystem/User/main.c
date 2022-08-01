#include "system.h"
#include "RegisterTemplate.h"
#include "led.h"
#include "SysTick.h"
#include "exti.h"
#include "time.h"
#include "iwdg.h"
#include "Sch.h"
#include "T_Lights.h"

//------------------------------------------------------------------------
//- 特性
//------------------------------------------------------------------------
#undef REGISTER_LED_FEATURE
#define FEATURE_LED
#define FEATURE_SYSTICK
#undef FEATURE_EXTI
#undef FEATURE_TIME
#undef FEATURE_IWDG
#define FEATURE_SCH

//------------------------------------------------------------------------
//- 私有函数原型
//------------------------------------------------------------------------
void RCC_HSE_Config(u32 div, u32 pllm);
void Square_Number(int, int*);
void Add_Number(int a, int* b);

/*************************************************************************
 * Name: main
 * Function:
 * Input:
 * Output:
 *************************************************************************/
int main(void)
{
    // 系统时钟是 72MHZ
    RCC_HSE_Config(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#ifdef REGISTER_LED_FEATURE
    RegLedFlash();
#endif
#ifdef FEATURE_LED
    LedInit();
#endif
#ifdef FEATURE_SYSTICK
    SysTick_Init(72);  // 72 是现在的系统时钟是 72MHZ
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
#ifdef FEATURE_SCH
    SCH_Init_TIM4();  // 内部初始化 TIM4 定时 1ms
    // SCH_Add_Task(LedFlashUpdate, 0, 1000);
    SCH_Add_Task(TRAFFIC_LIGHTS_Update, 0, 1000);
#endif

    // 上电复位时间 800ms
    delay_ms(800);

    int a = 2, b =3;
    void (* pFn)(int, int*);
    void (* pFn_led)(void);
    int Result_a,Result_b;

    pFn = Square_Number;
    pFn = Add_Number;
    pFn_led = LedFlash;

    Square_Number(a, &Result_a);
    (*pFn)(b, &Result_b);

    while (1)
    {
        #ifdef FEATURE_IWDG
        IWDG_FeedDog();
        #endif
        SCH_Dispatch_Tasks();
    }
}

void Square_Number(int a, int* b)
{
    *b = a * a;
}

void Add_Number(int a, int* b)
{
    *b = a + a;
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

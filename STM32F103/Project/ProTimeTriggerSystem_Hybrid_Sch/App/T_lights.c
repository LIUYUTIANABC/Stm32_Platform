#include "T_lights.h"
#include "led.h"

//------------------------------------------------------------------------
//- 私有常量
//------------------------------------------------------------------------
#define ON 0
#define OFF 1

// 定义几个状态
#define Red (0)
#define Red_And_Amber (1)
#define Green (2)

// 每个点亮状态的时间，（每秒调用一次刷新任务，所以单位是秒）
#define RED_DURATION (10)
#define RED_AND_AMBER_DURATION (10)
#define GREEN_DURATION RED_DURATION
#define AMBER_DURATION RED_AND_AMBER_DURATION

//------------------------------------------------------------------------
//- 私有变量
//------------------------------------------------------------------------
static unsigned char Light_State_G = Red;

/*************************************************************************
* Name: TRAFFIC_LIGHTS_Init
* Function:
* Input:
* Output:
*************************************************************************/
void TRAFFIC_LIGHTS_Init(const unsigned char START_STATE)
{
    Light_State_G = START_STATE;
}

/*************************************************************************
* Name: TRAFFIC_LIGHTS_Update
* Function: 必须每秒调用一次
* Input:
* Output:
*************************************************************************/
void TRAFFIC_LIGHTS_Update(void)
{
    static unsigned int Time_in_state;
    switch (Light_State_G)
    {
    case Red:
        // IO_LED_RED = ON;
        // IO_LED_AMBER = OFF;
        // IO_LED_GREENN = OFF;
        LED_BIT_GPIOC_PIN13 = !LED_BIT_GPIOC_PIN13;
        if (++Time_in_state == RED_DURATION)
        {
            Light_State_G = Red_And_Amber;
            Time_in_state = 0;
        }
        break;
    case Red_And_Amber:
        // IO_LED_RED = ON;
        // IO_LED_AMBER = ON;
        // IO_LED_GREENN = OFF;
        LED_BIT_GPIOC_PIN13 = 1;
        LED_BIT_GPIOC_PIN14 = 1;
        if (++Time_in_state == RED_AND_AMBER_DURATION)
        {
            Light_State_G = Green;
            Time_in_state = 0;
        }
        break;
    case Green:
        // IO_LED_RED = OFF;
        // IO_LED_AMBER = OFF;
        // IO_LED_GREENN = ON;
        LED_BIT_GPIOC_PIN14 = !LED_BIT_GPIOC_PIN14;
        if (++Time_in_state == GREEN_DURATION)
        {
            Light_State_G = Red;
            Time_in_state = 0;
        }
        break;
    default:
        break;
    }
}

#include "usart.h"

// #include "stm32f4xx_rcc.h"
// #include "stm32f4xx_gpio.h"
#include "stm32f10x_usart.h"
// #include "stm32f4xx_syscfg.h"
// #include "misc.h"

/*************************************************************************
* Name: fputc
* Function: printf redirect
* Input:
* Output:
*************************************************************************/
int fputc(int ch, FILE *p)
{
    USART_SendData(USART1, (u8)ch);
    /************************************
    * Note:
    * USART_FLAG_TXE: send to shift register
    * USART_FLAG_TC: send data is over
    * USART_FLAG_TC can not replace USART_FLAG_TXE
    * Because: Maybe before send the data, some bit is in shift register
    * if use the USART_FLAG_TC, The first send is error;
    * if use the USART_FLAG_TXE, send is right;
    ***************************************/
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
    return ch;
}

/*************************************************************************
* Name: DebugPrintf
* Function:
* Input:
* Output:
        Unsigned Char c
        Unsigned Short int 1234
        Float 12.3
        Int -123
        String Hello World
*************************************************************************/
void DebugPrintf(void)
{
    u8 u8_int = 12;
    u16 u16_int = 1234;
    float f_float = 12.34;
    int i = -123;
    char str[] = "Hello World";

    printf("Unsigned Char %x \r\n", u8_int);
    printf("Unsigned Short int %d \r\n", u16_int);
    printf("Float %3.1f \r\n", f_float);
    printf("Int %d \r\n", i);
    printf("String %s \r\n", str);
}

/*************************************************************************
* Name: Usart1NoInterruptInit
* Function: The Init is not need Interrupt
* Input:
* Output:
*************************************************************************/
void Usart1NoInterruptInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruture;
    USART_InitTypeDef USART_InitStruture;

    // Enable GPIO Rcc clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Alternate functions: USART1
    // Can not used GPIO_PinSource9 | GPIO_PinSource10 to enable GPIO_PinAFConfig
    // Error: GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 | GPIO_PinSource10, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    // GPIO Init
    GPIO_InitStruture.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruture.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruture.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruture);

    // USART RCC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // USART Init
    USART_InitStruture.USART_BaudRate = 9600;
    USART_InitStruture.USART_WordLength = USART_WordLength_8b;
    USART_InitStruture.USART_StopBits = USART_StopBits_1;
    USART_InitStruture.USART_Parity = USART_Parity_No;
    USART_InitStruture.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruture);

    // USART Enable
    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
}

/*************************************************************************
* Name: Usart1InterruptInit
* Function: Use USART1 Interrupt method
* Input:
* Output:
*************************************************************************/
void Usart1InterruptInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);

    // Enable USART
    USART_Cmd(USART1, ENABLE);

    // Enable TX/RX interrupt
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC Init
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*************************************************************************
* Name: USART1_IRQHandler
* Function: The address about usart1 Interrupt
* Input:
* Output:
*************************************************************************/
void USART1_IRQHandler(void)
{
    u8 u_temp;
    // Receive usart data
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        u_temp = USART_ReceiveData(USART1);
        USART_SendData(USART1, u_temp);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
            ;
    }
    USART_ClearFlag(USART1, USART_FLAG_TC);
}

//------------------------------------------------------------------------
//- Debug No_Interrput function
//------------------------------------------------------------------------
// if (Pwm_Value % 64 == 0)
// {
//     USART_SendData(USART1, Pwm_Value >> 8);
//     while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
//     {
//         ;
//     }
//     USART_ClearFlag(USART1, USART_FLAG_TC);

//     USART_SendData(USART1, Pwm_Value);
//     while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
//     {
//         ;
//     }
//     USART_ClearFlag(USART1, USART_FLAG_TC);
// }
// if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == 1)
// {
//     g_Count = USART_ReceiveData(USART1);
//     USART_ClearFlag(USART1, USART_FLAG_RXNE);
//     USART_SendData(USART1, g_Count);
//     while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
//     {
//         ;
//     }
//     USART_ClearFlag(USART1, USART_FLAG_TC);
// }

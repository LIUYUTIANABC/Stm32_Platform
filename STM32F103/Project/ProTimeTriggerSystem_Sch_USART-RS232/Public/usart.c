#include "usart.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

/*************************************************************************
* Name: Usart1NoInterruptInit
* Function: The Init is not need Interrupt
* Input:
* Output:
*************************************************************************/
void Usart1NoInterruptInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // Enable GPIO Rcc clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // USART RCC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // GPIO Init
    // Configure USART1 Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART Init
    /* USART2 configuration ------------------------------------------------------*/
    /* USART2 configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control enabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);
}

/*************************************************************************
* Name: UsartDebug
* Function:
* Input:
* Output: print 'Hello World'
*************************************************************************/
void UsartDebug(void)
{
    u8 l_test_buff[15] = "Hello World!";
    u8 i = 0;
   while (i < 15)
   {
       USART_SendData(USART1, l_test_buff[i]);
       while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
       i++;
   }
}

/*************************************************************************
* Name: UsartDebug
* Function:
* Input:
* Output: print 'Hello World'
*************************************************************************/
void UsartDebugRx(void)
{
    u8 i = 0;

   if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
   {
      i = USART_ReceiveData(USART1);
      USART_SendData(USART1, i);
   }
}


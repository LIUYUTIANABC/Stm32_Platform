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
void Usart1NoInterruptInit(u16 BAUD_RATE)
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
    USART_InitStructure.USART_BaudRate = BAUD_RATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
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
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
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

    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
        i = USART_ReceiveData(USART1);
        USART_SendData(USART1, i);
    }
}

// ------ Public variable definitions ------------------------------

u8 In_read_index_G = 0;    // Data in buffer that has been read
u8 In_waiting_index_G = 0; // Data in buffer not yet read

u8 Out_written_index_G = 0; // Data in buffer that has been written
u8 Out_waiting_index_G = 0; // Data in buffer not yet written

// ------ Public variable declarations -----------------------------

// The error code variable
//
// See Main.H for port on which error codes are displayed
// and for details of error codes
extern u8 Error_code_G;

#define ERROR_USART_TI (21)
#define ERROR_USART_WRITE_CHAR (22)

// ------ Private constants ----------------------------------------

// The receive buffer length
#define RECV_BUFFER_LENGTH 8

// The transmit buffer length
#define TRAN_BUFFER_LENGTH 50

// ------ Private variables ----------------------------------------

static u8 Recv_buffer[RECV_BUFFER_LENGTH];
static u8 Tran_buffer[TRAN_BUFFER_LENGTH];

/*------------------------------------------------------------------*-

  PC_LINK_IO_Update()

  Checks for character in the UART (hardware) receive buffer
  Sends next character from the software transmit buffer

-*------------------------------------------------------------------*/
void PC_LINK_IO_Update(void)
{
    // Deal with transmit bytes here

    // Is there any data ready to send?
    if (Out_written_index_G < Out_waiting_index_G)
    {
        PC_LINK_IO_Send_Char(Tran_buffer[Out_written_index_G]);

        Out_written_index_G++;
    }
    else
    {
        // No data to send - just reset the buffer index
        Out_waiting_index_G = 0;
        Out_written_index_G = 0;
    }

    // Only dealing with received bytes here
    // -> Just check the RI flag
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
        // Flag only set when a valid stop bit is received,
        // -> data ready to be read into the received buffer

        // Want to read into index 0, if old data has been read
        // (simple ~circular buffer)
        if (In_waiting_index_G == In_read_index_G)
        {
            In_waiting_index_G = 0;
            In_read_index_G = 0;
        }

        // Read the data from USART buffer
        Recv_buffer[In_waiting_index_G] = USART_ReceiveData(USART1);

        if (In_waiting_index_G < RECV_BUFFER_LENGTH)
        {
            // Increment without overflowing buffer
            In_waiting_index_G++;
        }

        USART_ClearFlag(USART1, USART_FLAG_RXNE); // Clear RT flag
    }
}

/*------------------------------------------------------------------*-

  PC_LINK_IO_Write_Char_To_Buffer()

  Stores a character in the 'write' buffer, ready for
  later transmission

-*------------------------------------------------------------------*/
void PC_LINK_IO_Write_Char_To_Buffer(u8 CHARACTER)
{
    // Write to the buffer *only* if there is space
    if (Out_waiting_index_G < TRAN_BUFFER_LENGTH)
    {
        Tran_buffer[Out_waiting_index_G] = CHARACTER;
        Out_waiting_index_G++;
    }
    else
    {
        // Write buffer is full
        // Increase the size of TRAN_BUFFER_LENGTH
        // or increase the rate at which UART 'update' function is called
        // or reduce the amount of data sent to PC
        Error_code_G = ERROR_USART_WRITE_CHAR;
    }
}

/*------------------------------------------------------------------*-

  PC_LINK_IO_Write_String_To_Buffer()

  Copies a (null terminated) string to the character buffer.
  (The contents of the buffer are then passed over the serial link)

  STR_PTR - Pointer to the NULL-TERMINATED string.

-*------------------------------------------------------------------*/
void PC_LINK_IO_Write_String_To_Buffer(u8 *STR_PTR)
{
    u8 i = 0;

    while (STR_PTR[i] != '\0')
    {
        PC_LINK_IO_Write_Char_To_Buffer(STR_PTR[i]);
        i++;
    }
}

/*------------------------------------------------------------------*-

  PC_LINK_IO_Get_Char_From_Buffer()

  Retrieves a character from the (software) buffer, if available

  The character from the buffer is returned, or - if no
  data are available - PC_LINK_IO_NO_CHAR is returned.

-*------------------------------------------------------------------*/
u8 PC_LINK_IO_Get_Char_From_Buffer(void)
{
    u8 Ch = PC_LINK_IO_NO_CHAR;

    // If there is new data in the buffer
    if (In_read_index_G < In_waiting_index_G)
    {
        Ch = Recv_buffer[In_read_index_G];

        if (In_read_index_G < RECV_BUFFER_LENGTH)
        {
            In_read_index_G++;
        }
    }

    return Ch;
}

/*------------------------------------------------------------------*-

  PC_LINK_IO_Send_Char()

  Based on Keil sample code, with added (loop) timeouts.
  Implements Xon / Off control.

  Uses on-chip UART hardware.

-*------------------------------------------------------------------*/
void PC_LINK_IO_Send_Char(u8 CHARACTER)
{
    u32 Timeout1 = 0;
    u32 Timeout2 = 0;

    if (CHARACTER == '\n')
    {
        Timeout1 = 0;
        while ((++Timeout1) && (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET))
            ;

        if (Timeout1 == 0)
        {
            // uart did not respond - error
            Error_code_G = ERROR_USART_TI;
            return;
        }

        USART_SendData(USART1, 0x0d); // output CR
    }

    Timeout1 = 0;
    while ((++Timeout1) && (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET))
        ;

    if (Timeout1 == 0)
    {
        // USART did not respond - error
        Error_code_G = ERROR_USART_TI;
        return;
    }

    USART_SendData(USART1, CHARACTER);
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

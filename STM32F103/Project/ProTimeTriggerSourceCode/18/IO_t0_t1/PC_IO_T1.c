/*------------------------------------------------------------------*-

   PC_IO_T1.C (v1.01)

  ------------------------------------------------------------------

   PC link library. Bidirectional. T1 used for baud rate generation.

   Uses the USART, and Pins 3.1 (Tx) and 3.0 (Rx) 

   See text for details (Chapter 18).


   COPYRIGHT
   ---------

   This code is from the book:

   PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont 
   [Pearson Education, 2001; ISBN: 0-201-33138-1].

   This code is copyright (c) 2001 by Michael J. Pont.
 
   See book for copyright details and other information.

-*------------------------------------------------------------------*/

#include "Main.h"
#include "PC_IO_T1.h"

// ------ Public variable declarations -----------------------------

extern tByte In_read_index_G;     
extern tByte In_waiting_index_G;  

extern tByte Out_written_index_G;  
extern tByte Out_waiting_index_G;  

/*------------------------------------------------------------------*-

  PC_LINK_IO_Init_T1()

  This (generic) version uses T1 for baud rate generation.

-*------------------------------------------------------------------*/
void PC_LINK_IO_Init_T1(const tWord BAUD_RATE)
   {
   PCON &= 0x7F;   // Set SMOD bit to 0 (don't double baud rates)

   //  Receiver enabled.
   //  8-bit data, 1 start bit, 1 stop bit, 
   //  Variable baud rate (asynchronous)
   //  Receive flag will only be set if a valid stop bit is received
   //  Set TI (transmit buffer is empty)
   SCON = 0x72;

   TMOD |= 0x20;   // T1 in mode 2, 8-bit auto reload

   TH1 = (256 - (tByte)((((tLong)OSC_FREQ / 100) * 3125) 
            / ((tLong) BAUD_RATE * OSC_PER_INST * 1000)));

   TL1 = TH1;  
   TR1 = 1;  // Run the timer
   TI = 1;   // Send first character (dummy)

   // Set up the buffers for reading and writing
   In_read_index_G = 0;
   In_waiting_index_G = 0;
   Out_written_index_G = 0;
   Out_waiting_index_G = 0;

   PC_LINK_IO_Write_String_To_Buffer("Serial OK\n");

   // Interrupt *NOT* enabled
   ES = 0;
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/




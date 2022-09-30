/*------------------------------------------------------------------*-

   Main.c (v1.00)

  ------------------------------------------------------------------

   Test program for shared-clock (UART) scheduler for 89C52, etc.

   *** (LOCAL / RS-232) SLAVE CODE ***

   --- HARDWARE ASSUMED ---
   --- 89C52 (or any 8051/52 device with T2)
   --- DS1232 (or similar) external watchdog [see text for conns]

   *** Both Master and Slave share the same tick rate (5 ms) ***
   *** - See Master code for details ***

   Required linker options (see Chapter 14 for details):

   OVERLAY (main ~ (LED_Flash_Update,TRAFFIC_LIGHTS_Update), 
   SCH_Dispatch_Tasks ! (LED_Flash_Update,TRAFFIC_LIGHTS_Update))
 

   COPYRIGHT
   ---------

   This code is from the book:

   PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont 
   [Pearson Education, 2001; ISBN: 0-201-33138-1].

   This code is copyright (c) 2001 by Michael J. Pont.
 
   See book for copyright details and other information.

-*------------------------------------------------------------------*/

#include "Main.h"

#include "LED_Flas.h"
#include "SCU_As.H"
#include "TLight_B.h"

/* ............................................................... */
/* ............................................................... */

void main(void)
   {
   // Set up the scheduler 
   SCU_A_SLAVE_Init_T1(9600);

   // Set up the flash LED task (demo purposes)
   LED_Flash_Init();

   // Prepare for the traffic light task
   TRAFFIC_LIGHTS_Init();

   // Add a 'flash LED' task (on for 1000 ms, off for 1000 ms)
   SCH_Add_Task(LED_Flash_Update, 0, 200);

   // Add a 'traffic lights' task
   SCH_Add_Task(TRAFFIC_LIGHTS_Update, 10, 200);

   // Start the scheduler
   SCU_A_SLAVE_Start();

   while(1)
      {
      SCH_Dispatch_Tasks();
      }
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/


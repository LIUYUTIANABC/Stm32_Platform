/*------------------------------------------------------------------*-

   Main.c (v1.00)

  ------------------------------------------------------------------

   Test program for shared-clock (interrupt) scheduler for 89C52, etc.

   *** DATA - MASTER CODE ***

   *** Both Master and Slave share the same tick rate (1 ms) ***
 
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

#include "LED_flas.h"
#include "SCI_Dm.H"
#include "TLight_B.h"

/* ................................................................. */
/* ................................................................. */

void main(void)
   {
   // Set up the scheduler 
   SCI_D_MASTER_Init_T2();

   // Prepare for the traffic light task
   TRAFFIC_LIGHTS_Init();

   // Prepare for the flash LED task (demo only)
   LED_Flash_Init();

   // Add a 'flash LED' task (on for 1000 ms, off for 1000 ms)
   SCH_Add_Task(LED_Flash_Update, 0, 1000);

   // Add a 'traffic light' task
   SCH_Add_Task(TRAFFIC_LIGHTS_Update, 30, 1000);
   
   // Start the scheduler
   SCI_D_MASTER_Start();

   while(1)
      {
      SCH_Dispatch_Tasks();
      }
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

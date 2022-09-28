/*------------------------------------------------------------------*-

   SCI_Ti1m.c (v1.00)

  ------------------------------------------------------------------

   This is an implementation of SCI SCHEDULER (TICK) for 8051/52.

   --- See Chapter 26 ---

   *** MASTER NODE : TICK-ONLY (SIMPLEX) ***

   *** DOES NOT CHECK FOR SLAVE ACKNOWLEDEMENTS

   *** Uses T2 for timing, 16-bit auto reload ***
   *** 12 MHz oscillator -> 1 ms (precise) tick interval ***

   --- Assumes '1232' watchdog on Slave ---


   COPYRIGHT
   ---------

   This code is from the book:

   PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont 
   [Pearson Education, 2001; ISBN: 0-201-33138-1].

   This code is copyright (c) 2001 by Michael J. Pont.
 
   See book for copyright details and other information.

-*------------------------------------------------------------------*/

#include "Main.h"
#include "Port.h"

#include "SCI_Ti1m.H"
#include "Delay_T0.h"
#include "TLight_A.h"

// ------ Public variable declarations -----------------------------

// The array of tasks (see Sch51.c)
extern sTask SCH_tasks_G[SCH_MAX_TASKS];

// The error code variable (see Sch51.c)
extern tByte Error_code_G;

/*------------------------------------------------------------------*-

  SCI_TICK1_MASTER_Init_T2()

  Scheduler initialisation function.  Prepares scheduler data 
  structures and sets up timer interrupts at required rate.

  You must call this function before using the scheduler.  

-*------------------------------------------------------------------*/
void SCI_TICK1_MASTER_Init_T2(void) 
   {
   tByte i;

   // No interrupts (yet)
   EA = 0;

   // ------ Set up the scheduler ----------------------------------
   // Sort out the tasks
   for (i = 0; i < SCH_MAX_TASKS; i++) 
      {
      SCH_Delete_Task(i);
      }

   // Reset the global error variable
   // - SCH_Delete_Task() will generate an error code, 
   //   (because the task array is empty)
   Error_code_G = 0;

   // ------ Set up Timer 2 (begin) --------------------------------
   // Now set up Timer 2
   // 16-bit timer function with automatic reload

   // Crystal is assumed to be 12 MHz
   // The Timer 2 resolution is 0.000001 seconds (1 �s)
   // The required Timer 2 overflow is 0.001 seconds (1 ms)
   // - this takes 1000 timer ticks
   // Reload value is 65536 - 1000 = 64536 (dec) = 0xFC18

   T2CON = 0x04;   // load Timer 2 control register
   T2MOD = 0x00;   // load Timer 2 mode register

   TH2    = 0xFC;  // load timer 2 high byte
   RCAP2H = 0xFC;  // load timer 2 reload capture reg, high byte
   TL2    = 0x18;  // load timer 2 low byte
   RCAP2L = 0x18;  // load timer 2 reload capture reg, low byte
   
   ET2   = 1;  // Timer 2 interrupt is enabled

   TR2   = 1;  // Start Timer 2
   // ------ Set up Timer 2 (end) ----------------------------------
   }

/*------------------------------------------------------------------*-

  SCI_TICK1_MASTER_Start()

  Starts the scheduler, by enabling interrupts.

  NOTE: Usually called after all regular tasks are added,
  to keep the tasks synchronised.

  NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!! 
 
-*------------------------------------------------------------------*/
void SCI_TICK1_MASTER_Start(void) 
   {
   // Try to place system in 'safe' state at start or after errors
   SCI_TICK1_MASTER_Enter_Safe_State();

   // Delay here to cause the slave to time out and reset
   // Adjust this delay to match the timeout periods on the slaves
   Hardware_Delay_T0(500);

   // Now send first tick to start the slave
   // (starts on falling edge)
   Interrupt_output_pin = 1;
   Hardware_Delay_T0(5);
   Interrupt_output_pin = 0;
   Hardware_Delay_T0(5);

   Interrupt_output_pin = 1; // Ready for first tick

   // Start the scheduler
   EA = 1;
   }

/*------------------------------------------------------------------*-

  SCI_TICK1_MASTER_Update_T2

  This is the scheduler ISR.  It is called at a rate determined by 
  the timer settings in SCI_TICK1_MASTER_Init_T2().  This version is 
  triggered by Timer 2 interrupts: timer is automatically reloaded.

-*------------------------------------------------------------------*/
void SCI_TICK1_MASTER_Update_T2(void) interrupt INTERRUPT_Timer_2_Overflow  
   {
   tByte Index;

   TF2 = 0; // Must manually clear this. 
 
   // Send 'tick' message to the slave
   Interrupt_output_pin = 0;

   // NOTE: calculations are in *TICKS* (not milliseconds)
   for (Index = 0; Index < SCH_MAX_TASKS; Index++)
      {
      // Check if there is a task at this location
      if (SCH_tasks_G[Index].pTask)
         {
         if (SCH_tasks_G[Index].Delay == 0)
            {
            // The task is due to run
            SCH_tasks_G[Index].RunMe += 1;  // Increment the run flag

            if (SCH_tasks_G[Index].Period)
               {
               // Schedule this regular task to run again
               SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
               }
            }
         else
            {
            // Not yet ready to run: just decrement the delay 
            SCH_tasks_G[Index].Delay -= 1;
            }
         }         
      }

   // Prepare for next tick
   Interrupt_output_pin = 1;
   }   

/*------------------------------------------------------------------*-

  SCI_TICK1_MASTER_Enter_Safe_State()

  This is the state entered by the system when:
  (1) The node is powered up or reset
  (2) The slave node fails
  (3) The network has an error
  (4) Ack messages are delayed for any other reason

  Try to ensure that the system is in a 'safe' state in these 
  circumstances.

-*------------------------------------------------------------------*/
void SCI_TICK1_MASTER_Enter_Safe_State(void) reentrant
   {
   // USER DEFINED - Edit as required

   // Here we display a safe output
   TRAFFIC_LIGHTS_Display_Safe_Output();
   }  

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

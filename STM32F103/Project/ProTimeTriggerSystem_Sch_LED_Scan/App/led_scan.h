#ifndef _LED_SCAN_H
#define _LED_SCAN_H

#include "system.h"

// ------ LED_MX4.C -----------------------------------------------

// LED connection requires 12 port pins
#define LED_DATA_PORT (P2)

#define LED_NUM_DIGITS (4)

/* Connections to LED_DATA_PORT - See Figure 21.6 for details

     DP   G    F    E    D    C    B    A    =   LED display pins
     |    |    |    |    |    |    |    |
    x.7  x.6  x.5  x.4  x.3  x.2  x.1  x.0   =   Port pins

    x.7 == LED_DATA_PORT^7, etc

   LED codes (NB - positive logic assumed here)

   0  = abcdef  => 00111111 = 0x3F
   1  = bc      => 00000110 = 0x06
   2  = abdeg   => 01011011 = 0x5B
   3  = abcdg   => 01001111 = 0x4F
   4  = bcfg    => 01100110 = 0x66
   5  = acdfg   => 01101101 = 0x6D
   6  = acdefg  => 01111101 = 0x7D
   7  = abc     => 00000111 = 0x07
   8  = abcdefg => 01111111 = 0x7F
   9  = abcdfg  => 01101111 = 0x6F

   To display decimal point, add 10 (decimal) to the above values  */

// Any combination of (4) pins on any ports may be used here
#define LED_DIGIT_0 PBout(8)
#define LED_DIGIT_1 PBout(9)
#define LED_DIGIT_2 PBout(10)
#define LED_DIGIT_3 PBout(11)

void LedScanInit(void);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

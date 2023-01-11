/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       CPU CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename      : cpu_cfg.h
* Version       : V1.25
* Programmer(s) : SR
*                 ITJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  CPU_CFG_MODULE_PRESENT
#define  CPU_CFG_MODULE_PRESENT


/*$PAGE*/
/*
*********************************************************************************************************
*                                     CPU TIMESTAMP CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_TS_xx_EN to enable/disable CPU timestamp features :
*
*               (a) CPU_CFG_TS_32_EN   enable/disable 32-bit CPU timestamp feature
*               (b) CPU_CFG_TS_64_EN   enable/disable 64-bit CPU timestamp feature
*
*           (2) (a) Configure CPU_CFG_TS_TMR_SIZE with the CPU timestamp timer's word size :
*
*                       CPU_WORD_SIZE_08         8-bit word size
*                       CPU_WORD_SIZE_16        16-bit word size
*                       CPU_WORD_SIZE_32        32-bit word size
*                       CPU_WORD_SIZE_64        64-bit word size
*
*               (b) If the size of the CPU timestamp timer is not a binary multiple of 8-bit octets
*                   (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple octet word
*                   size SHOULD be configured (e.g. to 16-bits).  However, the minimum supported word
*                   size for CPU timestamp timers is 8-bits.
*
*                   See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2a'.
*********************************************************************************************************
*/

                                                                /* Configure CPU timestamp features (see Note #1) :     */
#define  CPU_CFG_TS_32_EN                       DEF_ENABLED
#define  CPU_CFG_TS_64_EN                       DEF_DISABLED
                                                                /*   DEF_DISABLED  CPU timestamps DISABLED              */
                                                                /*   DEF_ENABLED   CPU timestamps ENABLED               */

                                                                /* Configure CPU timestamp timer word size ...          */
                                                                /* ... (see Note #2) :                                  */
#define  CPU_CFG_TS_TMR_SIZE                    CPU_WORD_SIZE_32


#endif                                                          /* End of CPU cfg module include.                       */


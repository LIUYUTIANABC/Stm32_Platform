/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2011; Micrium, Inc.; Weston, FL
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

#include  <cpu.h>

/*
*********************************************************************************************************
*
*                                           CORE CPU MODULE
*
* Filename      : cpu_core.h
* Version       : V1.29.01
* Programmer(s) : SR
*                 ITJ
*********************************************************************************************************
* Note(s)       : (1) Assumes the following versions (or more recent) of software modules are included in
*                     the project build :
*
*                     (a) uC/LIB V1.35.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This core CPU header file is protected from multiple pre-processor inclusion through use of
*               the  core CPU module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  CPU_CORE_MODULE_PRESENT                                /* See Note #1.                                         */
#define  CPU_CORE_MODULE_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   CPU_CORE_MODULE
#define  CPU_CORE_EXT
#else
#define  CPU_CORE_EXT  extern
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU CONFIGURATION
*
* Note(s) : (1) The following pre-processor directives correctly configure CPU parameters.  DO NOT MODIFY.
*
*           (2) CPU timestamp timer feature is required for :
*
*               (a) CPU timestamps
*               (b) CPU interrupts disabled time measurement
*
*               See also 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                      & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1'.
*********************************************************************************************************
*/

#ifdef   CPU_CFG_TS_EN
#undef   CPU_CFG_TS_EN
#endif


#if    ((CPU_CFG_TS_32_EN == DEF_ENABLED) || \
        (CPU_CFG_TS_64_EN == DEF_ENABLED))
#define  CPU_CFG_TS_EN                          DEF_ENABLED
#else
#define  CPU_CFG_TS_EN                          DEF_DISABLED
#endif

#if    ((CPU_CFG_TS_EN == DEF_ENABLED) || \
(defined(CPU_CFG_INT_DIS_MEAS_EN)))
#define  CPU_CFG_TS_TMR_EN                      DEF_ENABLED
#else
#define  CPU_CFG_TS_TMR_EN                      DEF_DISABLED
#endif

/*
*********************************************************************************************************
*                                      CPU TIMESTAMP DATA TYPES
*
* Note(s) : (1) CPU timestamp timer data type defined to the binary-multiple of 8-bit octets as configured
*               by 'CPU_CFG_TS_TMR_SIZE' (see 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #2').
*********************************************************************************************************
*/
typedef  CPU_INT32U  CPU_TS32;
typedef  CPU_INT64U  CPU_TS64;

typedef  CPU_TS32    CPU_TS;                                    /* Req'd for backwards-compatibility.                   */

#if     (CPU_CFG_TS_TMR_EN   == DEF_ENABLED)                    /* CPU ts tmr defined to cfg'd word size (see Note #1). */
typedef  CPU_INT32U  CPU_TS_TMR;
#endif

/*
*********************************************************************************************************
*                               CPU TIMESTAMP TIMER FREQUENCY DATA TYPE
*********************************************************************************************************
*/

typedef  CPU_INT32U  CPU_TS_TMR_FREQ;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/
#if  (CPU_CFG_TS_TMR_EN   == DEF_ENABLED)
CPU_CORE_EXT  CPU_TS_TMR_FREQ  CPU_TS_TmrFreq_Hz;               /* CPU ts tmr freq (in Hz).                             */
#endif

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
CPU_CORE_EXT  CPU_INT16U       CPU_IntDisMeasCtr;               /* Nbr tot    ints dis'd ctr.                           */
CPU_CORE_EXT  CPU_INT16U       CPU_IntDisNestCtr;               /* Nbr nested ints dis'd ctr.                           */
                                                                /* Ints dis'd time (in ts tmr cnts) : ...               */
CPU_CORE_EXT  CPU_TS_TMR       CPU_IntDisMeasStart_cnts;        /* ...  start time.                                     */
CPU_CORE_EXT  CPU_TS_TMR       CPU_IntDisMeasStop_cnts;         /* ...  stop  time.                                     */
CPU_CORE_EXT  CPU_TS_TMR       CPU_IntDisMeasOvrhd_cnts;        /* ...        time meas ovrhd.                          */
CPU_CORE_EXT  CPU_TS_TMR       CPU_IntDisMeasMaxCur_cnts;       /* ...     resetable max time dis'd.                    */
CPU_CORE_EXT  CPU_TS_TMR       CPU_IntDisMeasMax_cnts;          /* ... non-resetable max time dis'd.                    */
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                      DEFINED IN PRODUCT'S BSP
*********************************************************************************************************
*/
void             CPU_Init                 (void);

/* 测量中断禁止时间函数 */
#ifdef  CPU_CFG_INT_DIS_MEAS_EN                                         /* -------- CPU INT DIS TIME MEAS FNCTS ------- */
                                                                        /* See Note #1.                                 */
CPU_TS_TMR       CPU_IntDisMeasMaxCurReset(void);

CPU_TS_TMR       CPU_IntDisMeasMaxCurGet  (void);

CPU_TS_TMR       CPU_IntDisMeasMaxGet     (void);


void             CPU_IntDisMeasStart      (void);

void             CPU_IntDisMeasStop       (void);
#endif

/* 任务优先级 前导0 和 后导0 函数 */
                                                                        /* ----------- CPU CNT ZEROS FNCTS ------------ */
CPU_DATA         CPU_CntLeadZeros         (CPU_DATA    val);
CPU_DATA         CPU_CntTrailZeros        (CPU_DATA    val);

/* 时间戳 函数*/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit(void);
void  CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ  freq_hz);
CPU_TS_TMR  CPU_TS_TmrRd (void);
#endif

#endif

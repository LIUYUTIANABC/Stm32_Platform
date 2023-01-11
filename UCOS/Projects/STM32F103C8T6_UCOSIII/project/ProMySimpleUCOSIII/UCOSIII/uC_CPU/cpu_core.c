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

/*
*********************************************************************************************************
*
*                                           CORE CPU MODULE
*
* Filename      : cpu_core.c
* Version       : V1.29.01
* Programmer(s) : SR
*                 ITJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    CPU_CORE_MODULE
#include  <cpu_core.h>
// #include "stm32f10x.h"


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
/*
*******************************************************************
*                           寄存器定义
*******************************************************************
*/
#define  BSP_REG_DEM_CR                  (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                  (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT              (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR               (*(CPU_REG32 *)0xE0042004)

/*
*******************************************************************
*                           寄存器位定义
*******************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                          (1<<24)

#define  BSP_BIT_DWT_CR_CYCCNTENA                       (1<<0)

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*$PAGE*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                    /* ----------------- CPU TS FNCTS ----------------- */
#if   ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
       (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
static  void        CPU_TS_Init          (void);
#endif

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*                                             CPU_Init()
*
* Description : (1) Initialize CPU module :
*
*                   (a) Initialize CPU timestamps
*                   (b) Initialize CPU interrupts disabled time measurements
*                   (c) Initialize CPU host name
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Your Product's Application.
*
*               This function is a CPU initialization function & MAY be called by application/
*               initialization function(s).
*
* Note(s)     : (2) CPU_Init() MUST be called ... :
*
*                   (a) ONLY ONCE from a product's application; ...
*                   (b) BEFORE product's application calls any core CPU module function(s)
*
*               (3) The following initialization functions MUST be sequenced as follows :
*
*                   (a) CPU_TS_Init()           SHOULD precede ALL calls to other CPU timestamp functions
*
*                   (b) CPU_IntDisMeasInit()    SHOULD precede ALL calls to CPU_CRITICAL_ENTER()/CPU_CRITICAL_EXIT()
*                                                   & other CPU interrupts disabled time measurement functions
*********************************************************************************************************
*/
#if 0
void  CPU_Init (void)
{
                                                                /* --------------------- INIT TS ---------------------- */
#if ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
     (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
    CPU_TS_Init();                                              /* See Note #3a.                                        */
#endif
                                                                /* -------------- INIT INT DIS TIME MEAS -------------- */
#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasInit();                                       /* See Note #3b.                                        */
#endif

                                                                /* ------------------ INIT CPU NAME ------------------- */
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
     CPU_NameInit();
#endif
}
#endif
/* CPU初始化函数 */
void  CPU_Init (void)
{
/* CPU初始化函数中总共做了三件事
    1、初始化时间戳
    2、初始化中断禁用时间测量
    3、初始化CPU名字
这里只讲时间戳功能，剩下两个的初始化代码则删除不讲 */

#if ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
    (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
    CPU_TS_Init();
#endif

}


/*$PAGE*/
/*
*********************************************************************************************************
*                                            CPU_TS_Init()
*
* Description : (1) Initialize CPU timestamp :
*
*                   (a) Initialize/start CPU timestamp timer                            See Note #1
*                   (b) Initialize       CPU timestamp controls
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_Init().
*
* Note(s)     : (1) The following initialization MUST be sequenced as follows :
*
*                   (a) CPU_TS_TmrFreq_Hz     MUST be initialized prior to CPU_TS_TmrInit()
*                   (b) CPU_TS_TmrInit()      SHOULD precede calls to all other CPU timestamp functions;
*                                                 otherwise, invalid time measurements may be calculated/
*                                                 returned.
*
*                   See also 'CPU_Init()  Note #3a'.
*********************************************************************************************************
*/

#if ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
     (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
static  void  CPU_TS_Init (void)
{
                                                                /* ----------------- INIT CPU TS TMR ------------------ */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
    CPU_TS_TmrFreq_Hz   = 0u;                                   /* Init/clr     ts tmr freq (see Note #1a).             */
    CPU_TS_TmrInit();                                           /* Init & start ts tmr      (see Note #1b).             */
#endif
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/
/* 时间戳定时器初始化 */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    // CPU_INT32U  cpu_clk_freq_hz;

    // DEM_CR         |= (CPU_INT32U)DEM_CR_TRCENA;                /* Enable Cortex-M3's DWT CYCCNT reg.                   */
    // DWT_CYCCNT      = (CPU_INT32U)0u;
    // DWT_CR         |= (CPU_INT32U)DWT_CR_CYCCNTENA;

    // cpu_clk_freq_hz = BSP_CPU_ClkFreq();
    // CPU_TS_TmrFreqSet(cpu_clk_freq_hz);

    CPU_INT32U  fclk_freq;
    fclk_freq = BSP_CPU_ClkFreq();

    /* 启用DWT外设 */
    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;
    /* DWT CYCCNT寄存器计数清零 */
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    /* 注意：当使用软件仿真全速运行的时候，会先停在这里，
    就好像在这里设置了一个断点一样，需要手动运行才能跳过，
    当使用硬件仿真的时候却不会 */
    /* 启用Cortex-M3 DWT CYCCNT寄存器 */
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
/* 获取CPU的HCLK时钟
这个是跟硬件相关的，目前我们是软件仿真，我们暂时把跟硬件相关的代码屏蔽掉，
直接手动设置CPU的HCLK时钟*/
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
#if 1
    RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
#else
    CPU_INT32U    CPU_HCLK;

    /* 目前软件仿真我们使用25M的系统时钟 */
    CPU_HCLK = 25000000;

    return CPU_HCLK;
#endif
}

/* 初始化CPU_TS_TmrFreq_Hz，这个就是系统的时钟，单位为HZ */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ  freq_hz)
{
    CPU_TS_TmrFreq_Hz = freq_hz;
}
#endif

/* 获取CYCNNT计数器的值 */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;
    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;
    return (ts_tmr_cnts);
}
#endif
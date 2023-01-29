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

#ifdef  CPU_CFG_INT_DIS_MEAS_EN                                     /* ---------- CPU INT DIS TIME MEAS FNCTS --------- */
static  void        CPU_IntDisMeasInit   (void);

static  CPU_TS_TMR  CPU_IntDisMeasMaxCalc(CPU_TS_TMR  time_tot_cnts);
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

    // 1、初始化时间戳
#if ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
    (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
    CPU_TS_Init();
#endif

    // 2、初始化中断禁用时间测量
#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasInit();                                       /* See Note #3b.                                        */
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
*                                         CPU_TS_TmrFreqSet()
*
* Description : Set CPU timestamp's timer frequency.
*
* Argument(s) : freq_hz     Frequency (in Hertz) to set for CPU timestamp's timer.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_TmrInit(),
*               Application/BSP initialization function(s).
*
*               This function is a CPU module BSP function & SHOULD be called only by appropriate
*               application/BSP function(s) [see Note #1].
*
* Note(s)     : (1) (a) (1) CPU timestamp timer frequency is NOT required for internal CPU timestamp
*                           operations but may OPTIONALLY be configured by CPU_TS_TmrInit() or other
*                           application/BSP initialization functions.
*
*                       (2) CPU timestamp timer frequency MAY be used with optional CPU_TSxx_to_uSec()
*                           to convert CPU timestamps from timer counts into microseconds.
*
*                           See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2a'.
*
*                   (b) CPU timestamp timer period SHOULD be less than the typical measured time but MUST
*                       be less than the maximum measured time; otherwise, timer resolution inadequate to
*                       measure desired times.
*
*                       See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2b'.
*********************************************************************************************************
*/
/* 初始化CPU_TS_TmrFreq_Hz，这个就是系统的时钟，单位为HZ */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ  freq_hz)
{
    CPU_TS_TmrFreq_Hz = freq_hz;
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                     CPU_IntDisMeasMaxCurReset()
*
* Description : Reset current maximum interrupts disabled time.
*
* Argument(s) : none.
*
* Return(s)   : Maximum interrupts disabled time (in CPU timestamp timer counts) before resetting.
*
*               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                      & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
* Caller(s)   : Application.
*
*               This function is a CPU module application programming interface (API) function
*               & MAY be called by application function(s).
*
* Note(s)     : (1) After initialization, 'CPU_IntDisMeasMaxCur_cnts' MUST ALWAYS be accessed
*                   exclusively with interrupts disabled -- but NOT with critical sections.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
//初始化（复位）测量程序段的最大关中断时间
CPU_TS_TMR  CPU_IntDisMeasMaxCurReset (void)
{
    CPU_TS_TMR  time_max_cnts;
    //使用到临界段（在关/开中断时）时必须用到该宏，该宏声明和
    //定义一个局部变量，用于保存关中断前的 CPU 状态寄存器
    // SR（临界段关中断只需保存SR），开中断时将该值还原。
    CPU_SR_ALLOC();

    //获取复位前的程序段最大关中断时间
    time_max_cnts             = CPU_IntDisMeasMaxCurGet();
    //关中断
    CPU_INT_DIS();
    //清零程序段的最大关中断时间
    CPU_IntDisMeasMaxCur_cnts = 0u;
    //开中断
    CPU_INT_EN();

    //返回复位前的程序段最大关中断时间
    return (time_max_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                      CPU_IntDisMeasMaxCurGet()
*
* Description : Get current maximum interrupts disabled time.
*
* Argument(s) : none.
*
* Return(s)   : Current maximum interrupts disabled time (in CPU timestamp timer counts).
*
*               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                      & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
* Caller(s)   : CPU_IntDisMeasMaxCurReset(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function
*               & MAY be called by application function(s).
*
* Note(s)     : (1) After initialization, 'CPU_IntDisMeasMaxCur_cnts' MUST ALWAYS be accessed
*                   exclusively with interrupts disabled -- but NOT with critical sections.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
//获取测量的程序段的最大关中断时间
CPU_TS_TMR  CPU_IntDisMeasMaxCurGet (void)
{
    CPU_TS_TMR  time_tot_cnts;
    CPU_TS_TMR  time_max_cnts;
    CPU_SR_ALLOC();


    CPU_INT_DIS();
    //获取未处理的程序段最大关中断时间
    time_tot_cnts = CPU_IntDisMeasMaxCur_cnts;
    CPU_INT_EN();
    //获取减去测量时间后的最大关中断时间
    time_max_cnts = CPU_IntDisMeasMaxCalc(time_tot_cnts);
    //返回程序段的最大关中断时间
    return (time_max_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                       CPU_IntDisMeasMaxGet()
*
* Description : Get (non-resetable) maximum interrupts disabled time.
*
* Argument(s) : none.
*
* Return(s)   : (Non-resetable) maximum interrupts disabled time (in CPU timestamp timer counts).
*
*               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                      & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
* Caller(s)   : CPU_IntDisMeasInit(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function
*               & MAY be called by application function(s).
*
* Note(s)     : (1) After initialization, 'CPU_IntDisMeasMax_cnts' MUST ALWAYS be accessed
*                   exclusively with interrupts disabled -- but NOT with critical sections.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
//获取整个程序目前最大的关中断时间
CPU_TS_TMR  CPU_IntDisMeasMaxGet (void)
{
    CPU_TS_TMR  time_tot_cnts;
    CPU_TS_TMR  time_max_cnts;
    CPU_SR_ALLOC();


    CPU_INT_DIS();
    //获取尚未处理的最大关中断时间
    time_tot_cnts = CPU_IntDisMeasMax_cnts;
    CPU_INT_EN();
    //获取减去测量时间后的最大关中断时间
    time_max_cnts = CPU_IntDisMeasMaxCalc(time_tot_cnts);
    //返回目前最大关中断时间
    return (time_max_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                        CPU_IntDisMeasStart()
*
* Description : Start interrupts disabled time measurement.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_CRITICAL_ENTER().
*
*               This function is an INTERNAL CPU module function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
void  CPU_IntDisMeasStart (void)
{
    CPU_IntDisMeasCtr++;
    /* 嵌套层数为0   */
    if (CPU_IntDisNestCtr == 0u) {                                  /* If ints NOT yet dis'd, ...                       */
        /* 保存时间戳  */
        CPU_IntDisMeasStart_cnts = CPU_TS_TmrRd();                  /* ... get ints dis'd start time.                   */
    }
    CPU_IntDisNestCtr++;
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                        CPU_IntDisMeasStop()
*
* Description : Stop interrupts disabled time measurement.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_CRITICAL_EXIT().
*
*               This function is an INTERNAL CPU module function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : (1) (a) The total amount of time interrupts are disabled by system &/or application code
*                       during critical sections is calculated by the following equations :
*
*                       (1) When interrupts disabled time measurements are disabled :
*
*
*                               |   CRITICAL  |                           |   CRITICAL  |
*                               |<- SECTION ->|                           |<- SECTION ->|
*                               |    ENTER    |                           |    EXIT     |
*
*                            Disable                                    Enable
*                           Interrupts                                Interrupts
*
*                               ||           ||                           ||           ||
*                               ||           ||                           ||           ||
*                               ||       |   ||<------------------------->||       |   ||
*                               ||       |<->||             |             ||<----->|   ||
*                               ||       | | ||             |             ||   |   |   ||
*                                        | |                |                  |   |
*                                   interrupts            time                 interrupts
*                                    disabled                 interrupts       |enabled
*                                          |                   disabled        |
*                                          |              (via application)    |
*                                       time                                 time
*                                           interrupts                           interrupts
*                                         disabled ovrhd                        enabled ovrhd
*
*
*                           (A) time            =  [ time            -  time           ]  -  time
*                                   interrupts     [     interrupts         interrupts ]         total
*                                    disabled      [      enabled            disabled  ]         ovrhd
*                               (via application)
*
*
*                           (B) time       =  time              +  time
*                                   total         interrupts           interrupts
*                                   ovrhd        enabled ovrhd       disabled ovrhd
*
*
*                                   where
*
*                                           time                    time interrupts are disabled between
*                                               interrupts              first critical section enter &
*                                                disabled               last  critical section exit (i.e.
*                                           (via application)           minus total overhead time)
*
*                                           time                    time when interrupts are disabled
*                                               interrupts
*                                                disabled
*
*                                           time                    time when interrupts are  enabled
*                                               interrupts
*                                                enabled
*
*
*                                           time                    total overhead time to disable/enable
*                                               total                   interrupts during critical section
*                                               ovrhd                   enter & exit
*
*                                           time                    total overhead time to disable interrupts
*                                               interrupts              during critical section enter
*                                             disabled ovrhd
*
*                                           time                    total overhead time to enable  interrupts
*                                               interrupts              during critical section exit
*                                              enabled ovrhd
*
*$PAGE*
*
*                       (2) When interrupts disabled time measurements are enabled :
*
*
*        |                                    |                           |                                       |
*        |<----- CRITICAL SECTION ENTER ----->|                           |<------- CRITICAL SECTION EXIT ------->|
*        |                                    |                           |                                       |
*
*                   Time                                                 Time
*     Disable    Measurement                                          Measurement                  Enable
*    Interrupts     Start                                                Stop                    Interrupts
*
*        ||           |                      ||                           ||                         |           ||
*        ||           |                      ||                           ||                         |           ||
*        ||           |        |             ||<------------------------->||               |         |           ||
*        ||       |   |        |<----------->||             |             ||<------------->|         |       |   ||
*        ||       |   |        |      |      ||             |             ||       |       |         |       |   ||
*                 |            |      |                     |                      |       |                 |
*            interrupts       get     |                   time                     |      get            interrupts
*             disabled    start time  |                       interrupts           |   stop time          enabled
*                            meas     |                        disabled            |     meas
*                                   time                  (via application)      time
*                                       start meas                                   stop meas
*                                         ovrhd                                        ovrhd
*
*
*                           (A) time            =  [ time       -  time      ]  -  time
*                                   interrupts     [      stop         start ]         total meas
*                                    disabled      [      meas         meas  ]           ovrhd
*                               (via application)
*
*
*                           (B) time            =  time            +  time
*                                   total meas         start meas         stop meas
*                                     ovrhd              ovrhd              ovrhd
*
*
*                                   where
*
*                                           time                    time interrupts are disabled between first
*                                               interrupts              critical section enter & last critical
*                                                disabled               section exit (i.e. minus measurement
*                                           (via application)           overhead time; however, this does NOT
*                                                                       include any overhead time to disable
*                                                                       or enable interrupts during critical
*                                                                       section enter & exit)
*
*                                           time                    time of disable interrupts start time
*                                               start                   measurement (in timer counts)
*                                               meas
*
*                                           time                    time of disable interrupts stop  time
*                                               stop                    measurement (in timer counts)
*                                               meas
*
*
*                                           time                    total overhead time to start/stop disabled
*                                               total meas              interrupts time measurements (in timer
*                                                 ovrhd                 counts)
*
*                                           time                    total overhead time after getting start
*                                               start meas              time until end of start measurement
*                                                 ovrhd                 function  (in timer counts)
*
*                                           time                    total overhead time from beginning of stop
*                                               stop meas               measurement function until after getting
*                                                 ovrhd                 stop time (in timer counts)
*
*
*$PAGE*
*                   (b) (1) (A) In order to correctly handle unsigned subtraction overflows of start times
*                               from stop times, CPU timestamp timer count values MUST be returned via
*                               word-size-configurable 'CPU_TS_TMR' data type.
*
*                               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2a'.
*
*                           (B) Since unsigned subtraction of start times from stop times assumes increasing
*                               values, timestamp timer count values MUST increase with each time count.
*
*                               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2b'.
*
*                       (2) (A) To expedite & reduce interrupts disabled time measurement overhead; only the
*                               subtraction of start times from stop times is performed.
*
*                           (B) The final calculations to subtract the interrupts disabled time measurement
*                               overhead is performed asynchronously in appropriate API functions.
*
*                               See also 'CPU_IntDisMeasMaxCalc()  Note #1b'.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
void  CPU_IntDisMeasStop (void)
{
    CPU_TS_TMR  time_ints_disd_cnts;


    CPU_IntDisNestCtr--;
    /* 嵌套层数为0*/
    if (CPU_IntDisNestCtr == 0u) {                                  /* If ints NO longer dis'd,        ...              */
        /* 保存时间戳  */
        CPU_IntDisMeasStop_cnts = CPU_TS_TmrRd();                   /* ... get  ints dis'd stop time & ...              */
                                                                    /* ... calc ints dis'd tot  time (see Note #1b2A).  */
        /* 得到关中断时间  */
        time_ints_disd_cnts     = CPU_IntDisMeasStop_cnts -
                                  CPU_IntDisMeasStart_cnts;
                                                                    /* Calc max ints dis'd times.                       */
        /* 得到关中断时间  */
        if (CPU_IntDisMeasMaxCur_cnts < time_ints_disd_cnts) {
            CPU_IntDisMeasMaxCur_cnts = time_ints_disd_cnts;
        }
        if (CPU_IntDisMeasMax_cnts    < time_ints_disd_cnts) {
            CPU_IntDisMeasMax_cnts    = time_ints_disd_cnts;
        }
    }
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                        CPU_IntDisMeasInit()
*
* Description : (1) Initialize interrupts disabled time measurements feature :
*
*                   (a) Initialize interrupts disabled time measurement controls
*                   (b) Calculate  interrupts disabled time measurement overhead
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_Init().
*
* Note(s)     : (2) CPU_IntDisMeasInit() SHOULD precede ALL calls to CPU_CRITICAL_ENTER()/CPU_CRITICAL_EXIT()
*                   & other CPU interrupts disabled time measurement functions; otherwise, invalid interrupts
*                   disabled time measurements may be calculated/returned.
*
*                   See also 'CPU_Init()  Note #3b'.
*
*               (3) (a) (1) Interrupts disabled time measurement overhead performed multiple times to calculate
*                           a rounded average with better accuracy, hopefully of +/- one timer count.
*
*                       (2) However, a single overhead time measurement is recommended, even for instruction-
*                           cache-enabled CPUs, since critical sections are NOT typically called within
*                           instruction-cached loops.  Thus a single non-cached/non-averaged time measurement
*                           is a more realistic overhead for the majority of non-cached interrupts disabled
*                           time measurements.
*
*                   (b) Interrupts MUST be disabled while measuring the interrupts disabled time measurement
*                       overhead; otherwise, overhead measurements could be interrupted which would incorrectly
*                       calculate an inflated overhead time which would then incorrectly calculate deflated
*                       interrupts disabled times.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
static  void  CPU_IntDisMeasInit (void)
{
    CPU_TS_TMR  time_meas_tot_cnts;
    CPU_INT16U  i;
    CPU_SR_ALLOC();

                                                                /* ----------- INIT INT DIS TIME MEAS CTRLS ----------- */
    CPU_IntDisMeasCtr         = 0u;
    CPU_IntDisNestCtr         = 0u;
    CPU_IntDisMeasStart_cnts  = 0u;
    CPU_IntDisMeasStop_cnts   = 0u;
    CPU_IntDisMeasMaxCur_cnts = 0u;
    CPU_IntDisMeasMax_cnts    = 0u;
    CPU_IntDisMeasOvrhd_cnts  = 0u;

                                                                /* ----------- CALC INT DIS TIME MEAS OVRHD ----------- */
    time_meas_tot_cnts = 0u;
    /* 关中断 */
    CPU_INT_DIS();                                              /* Ints MUST be dis'd for ovrhd calc (see Note #3b).    */
    for (i = 0u; i < CPU_CFG_INT_DIS_MEAS_OVRHD_NBR; i++) {
        CPU_IntDisMeasMaxCur_cnts = 0u;
        /* 执行多个连续的开始/停止时间测量  */
        CPU_IntDisMeasStart();                                  /* Perform multiple consecutive start/stop time meas's  */
        CPU_IntDisMeasStop();
        /* 计算总的时间 */
        time_meas_tot_cnts += CPU_IntDisMeasMaxCur_cnts;        /* ...       & sum time meas max's                  ... */
    }
    /*得到平均值，就是每一次测量额外消耗的时间  */
                                                                /* ... to calc avg time meas ovrhd (see Note #3a).      */
    CPU_IntDisMeasOvrhd_cnts  = (time_meas_tot_cnts + (CPU_CFG_INT_DIS_MEAS_OVRHD_NBR / 2u))
                                                    /  CPU_CFG_INT_DIS_MEAS_OVRHD_NBR;
    CPU_IntDisMeasMaxCur_cnts =  0u;                            /* Reset max ints dis'd times.                          */
    CPU_IntDisMeasMax_cnts    =  0u;
    CPU_INT_EN();
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                       CPU_IntDisMeasMaxCalc()
*
* Description : Calculate maximum interrupts disabled time.
*
* Argument(s) : time_tot_cnts   Total interrupt disabled time, in timer counts.
*
* Return(s)   : Maximum interrupts disabled time (in CPU timestamp timer counts).
*
* Caller(s)   : CPU_IntDisMeasMaxCurGet(),
*               CPU_IntDisMeasMaxGet().
*
* Note(s)     : (1) (a) The total amount of time interrupts are disabled by system &/or application code
*                       during critical sections is calculated by the following equations :
*
*                       (1) time            =   [ time      -  time      ]  -  time
*                               interrupts      [     stop         start ]         total meas
*                                disabled       [     meas         meas  ]           ovrhd
*                           (via application)
*
*
*                       (2) time            =  time            +  time
*                               total meas         start meas         stop meas
*                                 ovrhd              ovrhd              ovrhd
*
*
*                               where
*
*                                       time                    time interrupts are disabled between
*                                           interrupts              first critical section enter &
*                                            disabled               last  critical section exit minus
*                                       (via application)           time measurement overhead
*
*                                       time                    time of disable interrupts start time
*                                           start                   measurement (in timer counts)
*                                           meas
*
*                                       time                    time of disable interrupts stop  time
*                                           stop                    measurement (in timer counts)
*                                           meas
*
*                                       time                    total overhead time to start/stop disabled
*                                           total meas              interrupts time measurements (in timer
*                                             ovrhd                 counts)
*
*                                       time                    total overhead time after getting start
*                                           start meas              time until end of start measurement
*                                             ovrhd                 function  (in timer counts)
*
*                                       time                    total overhead time from beginning of stop
*                                           stop meas               measurement function until after getting
*                                             ovrhd                 stop time (in timer counts)
*
*
*                   (b) To expedite & reduce interrupts disabled time measurement overhead, the final
*                       calculations to subtract the interrupts disabled time measurement overhead is
*                       performed asynchronously in API functions.
*
*                       See also 'CPU_IntDisMeasStop()  Note #1b2'.
*$PAGE*
*                   (c) The amount of time interrupts are disabled is calculated by either of the
*                       following equations :
*
*                       (1) Interrupts disabled time  =  Number timer counts  *  Timer period
*
*                               where
*
*                                   Number timer counts             Number of timer counts measured
*                                   Timer period                    Timer's period in some units of
*                                                                       (fractional) seconds
*                                   Interrupts disabled time        Amount of time interrupts are
*                                                                       disabled, in same units of
*                                                                       (fractional) seconds as the
*                                                                       Timer period
*
*                                                         Number timer counts
*                       (2) Interrupts disabled time  =  ---------------------
*                                                           Timer frequency
*
*                               where
*
*                                   Number timer counts             Number of timer counts measured
*                                   Timer frequency                 Timer's frequency in some units
*                                                                       of counts per second
*                                   Interrupts disabled time        Amount of time interrupts are
*                                                                       disabled, in seconds
*
*                       See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                              & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
*               (2) Although it is not typical, it is possible for an interrupts disabled time
*                   measurement to be less than the interrupts disabled time measurement overhead;
*                   especially if the overhead was calculated with a single, non-cached measurement
*                   & critical sections are called within instruction-cached loops.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
static  CPU_TS_TMR  CPU_IntDisMeasMaxCalc (CPU_TS_TMR  time_tot_cnts)
{
    CPU_TS_TMR  time_max_cnts;


    time_max_cnts = time_tot_cnts;
    if (time_max_cnts >  CPU_IntDisMeasOvrhd_cnts) {            /* If       max ints dis'd time >  ovrhd time, ...      */
        time_max_cnts -= CPU_IntDisMeasOvrhd_cnts;              /* ... adj  max ints dis'd time by ovrhd time; ...      */
    } else {                                                    /* ... else max ints dis'd time <  ovrhd time, ...      */
        time_max_cnts  = 0u;                                    /* ... clr  max ints dis'd time (see Note #2).          */
    }

    return (time_max_cnts);
}
#endif


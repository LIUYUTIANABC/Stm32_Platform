/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                       OS CONFIGURATION (APPLICATION SPECIFICS)
*
* File    : OS_CFG_APP.C
* By      : JJL
* Version : V3.03.01
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
************************************************************************************************************************
* Note(s) : DO NOT CHANGE THIS FILE!
************************************************************************************************************************
*/

#define  MICRIUM_SOURCE
#include <os_cfg_app.h>
#include <os.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *os_cfg_app__c = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                    DATA STORAGE
************************************************************************************************************************
*/

CPU_STK        OSCfg_IdleTaskStk   [OS_CFG_IDLE_TASK_STK_SIZE];

/* 时基列表 */
OS_TICK_SPOKE  OSCfg_TickWheel     [OS_CFG_TICK_WHEEL_SIZE];

/*$PAGE*/
/*
************************************************************************************************************************
*                                                      CONSTANTS
************************************************************************************************************************
*/

/* 空闲任务栈起始地址 */
CPU_STK      * const  OSCfg_IdleTaskStkBasePtr   = (CPU_STK    *)&OSCfg_IdleTaskStk[0];
/* 空闲任务栈大小 */
CPU_STK_SIZE   const  OSCfg_IdleTaskStkSize      = (CPU_STK_SIZE)OS_CFG_IDLE_TASK_STK_SIZE;
/* 时基列表大小 */
OS_OBJ_QTY     const  OSCfg_TickWheelSize        = (OS_OBJ_QTY  )OS_CFG_TICK_WHEEL_SIZE;

;
;********************************************************************************************************
;                                                uC/OS-III
;                                          The Real-Time Kernel
;
;
;                           (c) Copyright 2009-2010; Micrium, Inc.; Weston, FL
;                    All rights reserved.  Protected by international copyright laws.
;
;                                           ARM Cortex-M3 Port
;
; File      : OS_CPU_A.ASM
; Version   : V3.01.2
; By        : JJL
;             BAN
;
; For       : ARMv7M Cortex-M3
; Mode      : Thumb2
; Toolchain : RealView Development Suite
;             RealView Microcontroller Development Kit (MDK)
;             ARM Developer Suite (ADS)
;             Keil uVision
;********************************************************************************************************
;

;********************************************************************************************************
;                                          PUBLIC FUNCTIONS
;********************************************************************************************************

    ; IMPORT  OSRunning                                           ; External references
    ; IMPORT  OSPrioCur
    ; IMPORT  OSPrioHighRdy
    IMPORT  OSTCBCurPtr             ; 外部文件引人的参考(1)
    IMPORT  OSTCBHighRdyPtr
    ; IMPORT  OSIntExit
    ; IMPORT  OSTaskSwHook
    ; IMPORT  OS_CPU_ExceptStkBase


    EXPORT  OSStartHighRdy                                      ; Functions declared in this file
    EXPORT  OS_CPU_PendSVHandler    ; 该文件定义的函数(2)

;PAGE
;********************************************************************************************************
;                                               EQUATES
;********************************************************************************************************

; NVIC_INT_CTRL   EQU     0xE000ED04                              ; Interrupt control state register.
; NVIC_SYSPRI14   EQU     0xE000ED22                              ; System priority register (priority 14).
; NVIC_PENDSV_PRI EQU           0xFF                              ; PendSV priority value (lowest).
; NVIC_PENDSVSET  EQU     0x10000000                              ; Value to trigger PendSV exception.

;********************************************************************
;                               常量
;********************************************************************
;--------------------------------------------------------------------
;有关内核外设寄存器定义可参考官方文档：STM32F10xxx Cortex-M3 programming manual
;系统控制块外设SCB地址范围：0xE000ED00-0xE000ED3F
;--------------------------------------------------------------------
NVIC_INT_CTRL   EQU     0xE000ED04    ; 中断控制及状态寄存器 SCB_ICSR。
NVIC_SYSPRI14   EQU     0xE000ED22    ; 系统优先级寄存器 SCB_SHPR3：
; bit16~23
NVIC_PENDSV_PRI EQU           0xFF    ; PendSV 优先级的值(最低)。
NVIC_PENDSVSET  EQU     0x10000000    ; 触发PendSV异常的值 Bit28：PENDSVSET


;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************

    PRESERVE8
    THUMB

    AREA CODE, CODE, READONLY


;PAGE
;********************************************************************************************************
;                                         START MULTITASKING
;                                      void OSStartHighRdy(void)
;
; Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
;              the first task to start.
;
;           2) OSStartHighRdy() MUST:
;              a) Setup PendSV exception priority to lowest;
;              b) Set initial PSP to 0, to tell context switcher this is first run;
;              c) Set the main stack to OS_CPU_ExceptStkBase
;              d) Trigger PendSV exception;
;              e) Enable interrupts (tasks will run with interrupts enabled).
;********************************************************************************************************

; OSStartHighRdy
;     LDR     R0, =NVIC_SYSPRI14                                  ; Set the PendSV exception priority
;     LDR     R1, =NVIC_PENDSV_PRI
;     STRB    R1, [R0]

;     MOVS    R0, #0                                              ; Set the PSP to 0 for initial context switch call
;     MSR     PSP, R0

;     LDR     R0, =OS_CPU_ExceptStkBase                           ; Initialize the MSP to the OS_CPU_ExceptStkBase
;     LDR     R1, [R0]
;     MSR     MSP, R1

;     LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
;     LDR     R1, =NVIC_PENDSVSET
;     STR     R1, [R0]

;     CPSIE   I                                                   ; Enable interrupts at processor level

; OSStartHang
;     B       OSStartHang                                         ; Should never get here

;*******************************************************************
;                        开始第一次上下文切换
; 1、配置PendSV异常的优先级为最低
; 2、在开始第一次上下文切换之前，设置psp=0
; 3、触发PendSV异常，开始上下文切换
;*******************************************************************
OSStartHighRdy
    LDR   R0, = NVIC_SYSPRI14    ; 设置  PendSV 异常优先级为最低
    LDR     R1, = NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                 ;设置psp的值为0，开始第一次上下文切换
    MSR     PSP, R0

    LDR     R0, =NVIC_INT_CTRL     ; 触发PendSV异常
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                      ; 启用总中断，NMI和HardFault除外

OSStartHang
    B       OSStartHang            ; 程序应永远不会运行到这里

;PAGE
;********************************************************************************************************
;                                       HANDLE PendSV EXCEPTION
;                                   void OS_CPU_PendSVHandler(void)
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
;              b) Save remaining regs r4-r11 on process stack;
;              c) Save the process SP in its TCB, OSTCBCurPtr->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook();
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              f) Get current ready thread TCB, OSTCBCurPtr = OSTCBHighRdyPtr;
;              g) Get new process SP from TCB, SP = OSTCBHighRdyPtr->OSTCBStkPtr;
;              h) Restore R4-R11 from new process stack;
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into PendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) OSTCBCurPtr      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdyPtr  points to the OS_TCB of the task to resume
;
;           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;********************************************************************************************************

; OS_CPU_PendSVHandler
;     CPSID   I                                                   ; Prevent interruption during context switch
;     MRS     R0, PSP                                             ; PSP is process stack pointer
;     CBZ     R0, OS_CPU_PendSVHandler_nosave                     ; Skip register save the first time

;     SUBS    R0, R0, #0x20                                       ; Save remaining regs r4-11 on process stack
;     STM     R0, {R4-R11}

;     LDR     R1, =OSTCBCurPtr                                    ; OSTCBCurPtr->OSTCBStkPtr = SP;
;     LDR     R1, [R1]
;     STR     R0, [R1]                                            ; R0 is SP of process being switched out

;                                                                 ; At this point, entire context of process has been saved
; OS_CPU_PendSVHandler_nosave
;     PUSH    {R14}                                               ; Save LR exc_return value
;     LDR     R0, =OSTaskSwHook                                   ; OSTaskSwHook();
;     BLX     R0
;     POP     {R14}

;     LDR     R0, =OSPrioCur                                      ; OSPrioCur   = OSPrioHighRdy;
;     LDR     R1, =OSPrioHighRdy
;     LDRB    R2, [R1]
;     STRB    R2, [R0]

;     LDR     R0, =OSTCBCurPtr                                    ; OSTCBCurPtr = OSTCBHighRdyPtr;
;     LDR     R1, =OSTCBHighRdyPtr
;     LDR     R2, [R1]
;     STR     R2, [R0]

;     LDR     R0, [R2]                                            ; R0 is new process SP; SP = OSTCBHighRdyPtr->StkPtr;
;     LDM     R0, {R4-R11}                                        ; Restore r4-11 from new process stack
;     ADDS    R0, R0, #0x20
;     MSR     PSP, R0                                             ; Load PSP with new process SP
;     ORR     LR, LR, #0x04                                       ; Ensure exception return uses process stack
;     CPSIE   I
;     BX      LR                                                  ; Exception return will restore remaining context

;***********************************************************************
;                          PendSVHandler异常
;***********************************************************************
OS_CPU_PendSVHandler
    ; 关中断，NMI和HardFault除外，防止上下文切换被中断
    CPSID   I
    ; 将psp的值加载到R0
    MRS     R0, PSP
    ; 判断R0，如果值为0则跳转到OS_CPU_PendSVHandler_nosave
    ; 进行第一次任务切换的时候，R0肯定为0
    CBZ     R0, OS_CPU_PendSVHandler_nosave

    ;-----------------------一、保存上文-----------------------------
    ; 任务的切换，即把下一个要运行的任务的栈内容加载到CPU寄存器中
    ;--------------------------------------------------------------
    ; 在进入PendSV异常的时候，当前CPU的xPSR，PC（任务入口地址），
    ; R14，R12，R3，R2，R1，R0会自动存储到当前任务栈，
    ;同时递减PSP的值，随便通过代码：MRS R0, PSP 把PSP的值传给R0

    ; 手动存储CPU寄存器R4-R11的值到当前任务的栈
    STMDB   R0!, {R4-R11}

    ; 加载 OSTCBCurPtr 指针的地址到R1，这里LDR属于伪指令
    LDR     R1, = OSTCBCurPtr
    ; 加载 OSTCBCurPtr 指针到R1，这里LDR属于ARM指令
    LDR     R1, [R1]
    ; 存储R0的值到OSTCBCurPtr->OSTCBStkPtr，这个时候R0存的是任务空闲栈的栈顶
    STR     R0, [R1]

    ;-----------------------二、切换下文-----------------------------
    ; 实现 OSTCBCurPtr = OSTCBHighRdyPtr
    ; 把下一个要运行的任务的栈内容加载到CPU寄存器中
    ;--------------------------------------------------------------
OS_CPU_PendSVHandler_nosave
    ; 加载 OSTCBCurPtr 指针的地址到R0，这里LDR属于伪指令
    LDR     R0, = OSTCBCurPtr
    ; 加载 OSTCBHighRdyPtr 指针的地址到R1，这里LDR属于伪指令
    LDR     R1, = OSTCBHighRdyPtr
    ; 加载 OSTCBHighRdyPtr 指针到R2，这里LDR属于ARM指令
    LDR     R2, [R1]
    ; 存储 OSTCBHighRdyPtr 到 OSTCBCurPtr
    STR     R2, [R0]

    ; 加载 OSTCBHighRdyPtr 到 R0
    LDR     R0, [R2]
    ; 加载需要手动保存的信息到CPU寄存器R4-R11
    LDMIA   R0!, {R4-R11}
    ; 更新PSP的值，这个时候PSP指向下一个要执行的任务的栈的栈底
    ;（这个栈底已经加上刚刚手动加载到CPU寄存器R4-R11的偏移）
    MSR     PSP, R0
    ; 确保异常返回使用的栈指针是PSP，即LR寄存器的位2要为1
    ORR     LR, LR, #0x04
    ; 开中断
    CPSIE   I
    ; 异常返回，这个时候任务栈中的剩下内容将会自动加载到xPSR，
    ; PC（任务入口地址），R14，R12，R3，R2，R1，R0（任务的形参）
    ; 同时PSP的值也将更新，即指向任务栈的栈顶。
    ; 在STM32中，栈是由高地址向低地址生长的。
    BX      LR

    END

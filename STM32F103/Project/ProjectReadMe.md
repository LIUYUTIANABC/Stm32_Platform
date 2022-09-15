# 工程介绍

## ProBasicFunction

包含 STM32F103C8 的基础功能，熟悉芯片，可以烧录，调试

- 寄存器模板
- 库函数模板
- 使用固件库控制 GPIO
- 系统时钟
- 位带操作
- 滴答定时器 SysTick
- STM32 中断系统
- 外部中断
- 定时器 TIM4 定时中断
- 独立看门狗 IWDG

## ProTimeTriggerSystem_Sch

结合 "时间触发嵌入式系统设计模式" 书籍，学习基于时间的触发嵌入式系统

- 学习书籍笔记参考文件夹：
  - "..\\..\\STM32_Developm_List\时间触发嵌入式系统设计模式\时间触发嵌入式系统笔记.md"
- 调度器模式
  - 合作式调度器
  - 抢占式调度器

## ProTimeTriggerSystem_Hybrid_Sch

- 混合式调度器：综合抢占式调度器和合作式调度器的特性，且是受控的
  - 即有一个抢占式任务的合作式调度器

## ProTimeTriggerSourceCode

"时间触发嵌入式系统设计模式" 书籍的光盘 CD 内的源码；

## ProTimeTriggerSystem_Sch_USART-RS232

- 对应 时间触发嵌入式系统 的 Chapter 18

使用合作式调度器，实现 USART1 和 PC 通过串口助手通信；

- 新的思路：
  - 1、文件的分层；
    - 只给人看的函数，函数名就是功能，应用层； pc_rs232 只写功能
    - 内部和底层硬件沟通的，中间层；usart.c 写初始化和底层代码
  - 2、多级任务；
    - 所有要发送的字节写到 buff 里面
    - 只有一个 update 函数，每 10ms 调度发送

- 新的方法
  - 1、使用指针，指向数组下标
    - 如何调度 update；需要两个指针，指向已发送和未发送的数组下标
  - 2、“时分秒” 更好的计算方式，用于定时的代码

## ProTimeTriggerSystem_Sch_Key

- 对应 时间触发嵌入式系统 的 Chapter 19

- 方法
  - 按键按下 --- Switch_A
  - 通断开关 --- On_Off
  - 多状态开关 --- 短按，中长按，长按

## ProTimeTriggerSystem_Sch_Key_RS232

- 对应 时间触发嵌入式系统 的 Chapter 20

- 新的思路：
  - 1、函数调用：值通过指针参数，取指的方式传递回原函数
  - 2、将按键的码值存储在数组中，数组使用两个下标指针，先进先出的原则，充当队列
    - 将数据缓存到队列当中，外部只操作队列
  - 3、二维指针的使用

- 方法
  - 1、按鍵扫描方法 - 多级任务
  - 2、队列的数据的，存，取方法

- 注意事项：
  - 按键的 IO 口使用了 PB2 和 PB3，不能正常使用
  - PB2 和 BOOT1 共用 IO 口，复位之后是 PB2/BOOT1
    - BOOT0 和 BOOT1 不能做普通 IO 使用
  - PB3 复位之后默认是 JTDO 是 JTAG 的调试脚
    - PB3 要做为普通 IO 使用，需要禁止 JTAG 的调试功能，使用 SWD 调试

## ProTimeTriggerSystem_Sch_LED_Scan

- 对应 时间触发嵌入式系统 的 Chapter 21
- LED 组成七段数码管，外加'.'，即 8 段数码管

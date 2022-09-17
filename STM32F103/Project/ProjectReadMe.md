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

```
// 函数参数是指针，可以改变值
u8 KEYPAD_Scan(char* const pKey, char* const pFuncKey)
KEYPAD_Scan(&Key, &FnKey);

// Load keypad data into buffer
KEYPAD_recv_buffer[KEYPAD_in_waiting_index][0] = Key;
KEYPAD_recv_buffer[KEYPAD_in_waiting_index][1] = FnKey;
```

- 方法
  - 1、按鍵扫描方法 - 多级任务
  - 2、队列的数据的，存，取方法

- 注意事项：
  - STM32 做按键的 IO 口使用了 PB2 和 PB3，不能正常使用
  - PB2 和 BOOT1 共用 IO 口，复位之后是 PB2/BOOT1
    - BOOT0 和 BOOT1 不能做普通 IO 使用
  - PB3 复位之后默认是 JTDO 是 JTAG 的调试脚
    - PB3 要做为普通 IO 使用，需要禁止 JTAG 的调试功能，使用 SWD 调试

## ProTimeTriggerSystem_Sch_LED_Scan

- 对应 时间触发嵌入式系统 的 Chapter 21
- LED 组成七段数码管，外加'.'，即 8 段数码管

方法：

- 时分秒 的计算方法，使用存储在 flash 里面的数组存储不变变量

```
// 存储不变的变量，使用 const 修饰，数据存放在 flash（ROM）中
tByte code LED_Table_G[20] =
// 0     1     2     3     4     5     6     7     8     9
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
// 0.    1.    2.    3.    4.    5.    6.    7.    8.    9.
 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

// 存储 可变的变量，普通变量，数据存放在 RAM（全局变量）或 堆栈（局部变量）中
// Global data formatted for display (initially 0,0,0,0)
tByte LED_Mx4_Data_G[4] = {0x3F,0x3F,0x3F,0x3F};

LED_Mx4_Data_G[1] = LED_Table_G[Min_G / 10];
LED_Mx4_Data_G[0] = LED_Table_G[Min_G % 10];
```

- 多路复用的方式扫描
- 端口取反的方法

```
// 数据取反：
LED_DATA_PORT = 255 - LED_Mx4_Data_G[Digit_G];
```


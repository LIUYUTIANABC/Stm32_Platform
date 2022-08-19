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

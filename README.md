# Stm32_Platform

整个工程都是基于普中 STM32 开发板的硬件结构

注意：

- 普中开发板的 STM32F103C8T6 出现串口不能下载的情况
- 所以从网上购买最小系统板, 后面开发应该是如下的最小系统板展开
  - 购买链接: https://detail.tmall.com/item.htm?_u=i1qhgis7fb7e&id=626358704662

## STM32F103

- 主控芯片：STM32F103C8T6

- Document： 芯片手册, 原理图, 中文参考资料等
  - STM32F103C8T6_Feature.md; 记录的是 STM32F103C8T6 的特性，时钟，GPIO，中断，定时器等基本功能讲解
- Programming： 使用串口下载程序, ST-Link, 普中仿真器, SWD 的使用方法
- Project： 使用 STM32F103 开发的工程项目
  - ProBasicFunction： 学会使用 STM32F103C8T6，参考 “STM32F103C8T6_Feature.md” 实现基本功能

## STM32F407

- 主控芯片：STM32F407ZGT6

## STM32_Development_List

- stm32_开发项目记录.md
  - 网上搜索的可能在开发中有用的嵌入式系统
  - 记录待开发的工程列表
- 时间嵌入式触发系统

## UCOSII

- Documents：UCOSII 学习笔记；任哲版本
- Projects
  - GCC_Demo；熟悉 GCC 编译 C 语言代码；因为任哲书中使用 Borland，已经被淘汰，使用 GCC；
    - Demo_HelloWorld; 基本使用
    - Demo_Two_C_files; 编译多个文件
    - Demo_Make；使用 make 工具和 makefile 调用 GCC 编译 main.c
    - Demo_Bat；批处理 .bat 文件的编写
  - BatAndMake；写 PowerShell 脚本批处理文件，通过 GCC 编译 main.c，自动生成程序框架
  - UcosIIBook_2-8；对应任哲书中的 2-8 程序代码；了解 TCB 结构体
  - UcosIIBook_3_6；对应任哲书中的 3-6 程序代码；程序在 WIN10 不能使用，因为书中是 XP 系统的编译环境
  - STM32F103C8T6_UCOSIII；移植 UCOSIII 到 STM32F103C8T6 当中
    - project：工程文件
      - ProBasicFunction: 按照普中学习文档，成功移植 UCOSIII 到 STM32F103C8T6

## STM32 寻找资源

ST官网：www.st.com

ST社区：www.stmcu.org.cn

ST中文网：www.stmcu.com.cn

### ST 官网寻找资料

- 参考网站：https://blog.csdn.net/ybhuangfugui/article/details/52705377
- 参考网站：https://blog.csdn.net/ybhuangfugui/article/details/52677648

主要资料

- STM32Fxx 芯片相关文档资料
- 标准外设库
- 开发工具： STM32CubeMX， Keil(MDK-ARM), IAR for ARM
- 烧录软件：STM32 ST-LINK Utility

什么是 Keil(MDK-ARM)？

- 使用 keil 开发stm32的项目，
- keil4 是内部集成的芯片包，keil5 需要下载芯片包
- keil4 开发51单片机，keil5 + pack 开发Arm内核的 stm32 程序
- 安装 pack 直接双击打开，默认会选择 keil5 的路径

怎么下载中文资料？

- 参考网址：https://blog.csdn.net/qq_20222919/article/details/109099872
- 进入ST官方论坛：https://www.stmcu.org.cn/
- 资料 - 中文译文
- 一些教学网站下载资料

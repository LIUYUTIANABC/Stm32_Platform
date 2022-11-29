# 嵌入式实时操作系统 μCOS-II 原理及应用 -- 任哲

- [嵌入式实时操作系统 μCOS-II 原理及应用 -- 任哲](#嵌入式实时操作系统-μcos-ii-原理及应用----任哲)
  - [前言](#前言)
  - [第一章：嵌入式实时操作系统的概念](#第一章嵌入式实时操作系统的概念)
  - [第二章：预备知识](#第二章预备知识)
    - [开发工具](#开发工具)

## 前言

- Linux 相当庞大，没有一定基础，直接学习比较难；
- uCOS 奠定基础，再去学习 Linux 可能更容易理解；
- 操作系统都是触类旁通的，学会一款，其他上手很快；

## 第一章：嵌入式实时操作系统的概念

- 它是多任务的，抢占式的，且要满足实时性，即任务切换时间应与系统任务数量无关，且中断延迟的时间是可预知尽量短；

![img](./img/2022-11-20_1_ucos_form.jpg)

## 第二章：预备知识

### 开发工具

好的开发工具：GCC；make 工具及 makefile；

- 任哲这本书里面用的是 “Borland C 3.1” 这是一款在 PC 编译 C 文件的软件
- 这本书的出版时间是 2006 年，BC3.1 是一款比较老的软件，基本现在都淘汰不用
- 现在比较流行的是 GCC 所以，要先学习 GCC 编译 C 文件

学习 GCC

参考网址：https://blog.csdn.net/qq_42475711/article/details/85224010

- GCC（GNU C Compiler）即GNU c编译器；它是编译器，用来编译 C 语言；
- 现在 GCC 拓展称为（GNU Compiler Collection）即 GNU 编译器套件；可以编译 C,C++,Java 等；所以称为编译器套件
- GUN 是一个技术组织，建立自由软件基金会，发行了 GPL（GUN General Public License）GUN 通用公共授权书
  - 后来 Linux 遵循 GPL，自由软件从此迅速发展，Linux 越来越强大

下载安装 GCC：https://osdn.net/projects/mingw/releases/

- MinGW（Minimalist GUN for Windows）：Windows 上的极简 GUN
- 安装好后，把 MinGM 的 bin 目录加入到环境变量的 path 中，这样 dos 在任意目录都可以调用 GCC
  - 添加环境变量的作用就是让 dos 或者其他软件可以访问到环境变量里面的内容
  - Windows 运行程序会从环境变量中浏览可执行命令 cmdlet

GCC 常用指令讲解

- 用 GCC 在 Windows 上编译 .c 文件并非直接生成 .exe 文件（Linux上为*.out）中间还经历了预处理、编译和汇编几个过程
- .c 预处理 .i 编译 .s 汇编 .o 连接 .exe(out)

![img](./img/2022-11-29_2_GCC_process.jpg)

- gcc Demo.c; Demo.c 文件 -> a.exe； a.exe 是默认生成的文件名；
- -o 指令：
  - 指定文件名：gcc Demo.c -o Demo.exe;
  - 指定生成文件目录和文件名：gcc Demo.c -o ..\Demo.exe;
- -E（预处理）指令：
  - 将执行预处理操作也即生成 *.i 文件,gcc 编译器将对 # 开头的指令进行解析


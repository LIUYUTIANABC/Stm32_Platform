## project\ProBasicFunction 移植成功的案例

- 参考文档：普中STM32F1xx-UCOSIII开发攻略.pdf
- 按照参考文档中的步骤，移植会出现一些问题
  - 1、startup_stm32f10x_hd.s 启动文件中有三处 PendSV_Handler 和 SysTick_Handler 分别改为OS_CPU_PendSVHandler 和 OS_CPU_SysTickHandle
    - 粗心只改了两处，有个头没有注意到
  - 2、lib_cfg.h 文件中 LIB_MEM_CFG_HEAP_SIZE 要改为 5u * 1024u（c8t6的RAM只有20K，不可超过）
    - 如果不修改，编译会提示空间不够：
      - .\Obj\ProBasicFunction.axf: Error: L6406E: No space in execution regions with .ANY selector matching lib_mem.o(.bss).
    - 上面文档中用的 F103ZE 空间比 F03C8 大很多；所以移植到中容量的芯片中，空间不够，需要修改 memory 空间大小
    - 参考网址：https://blog.csdn.net/d89538239/article/details/109811582

## project\ProMySimpleUCOSIII

按照野火文档，从 0 到 1 写一个简单版的 UCOS；目的是学习内核

- 参考文档：https://doc.embedfire.com/rtos/ucos/i.mx_rt1052/zh/latest/zero_to_one/foreword.html
- 参考视频：https://www.bilibili.com/video/av62027241/

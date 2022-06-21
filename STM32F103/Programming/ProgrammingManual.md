## 如何下载程序到 STM32F103C8

- 使用串口
- 使用 SWD 下载程序
- 使用 ST-Link
- 使用普中 STM32 仿真

## ST-Link, J-Link, U-Link; SWD, JTAG, SWIM 以及串口之间的联系与区别

参考网址：

- https://blog.csdn.net/m0_46170433/article/details/105883593

### 串口

- 串口有多种：
  - 异步串口 UART; UART包含TTL电平的串口和RS232电平的串口
  - 一般UART通常是指TTL电平的串口

- TTL电平和RS-232电平
  - Uart一般接到SOC，使用的是TTL电平，定义-3.3V-0为低电平-L(0)，而0~+3.3V为高电平-H(1)
  - 一般PC端用的串口是RS232串口，RS232是负逻辑电平，它定义+5 -+12V为低电平-L(0)，而-12~-5V为高电平-H(1)
  - 由于 Uart 和 RS232 的电平逻辑不同，所以，需要中间转换的芯片，CH340G，MAX232
  - CH340G，USB 转 TTL 串口，需要 CH340 驱动，虚拟出 COM 口
  - SP3232，RS232 转 TTL 型，专为 RS232 标准串口设计

- RS232 和 RS485 区别
  - RS232 是点对点的通信，只有一对收发，适合本地设备之间短距离通信
  - RS-485 是多点双向通信，只能有一个主机（master），其他为从机，最多可连接32个设备，最大传输距离是1219米，传输速率为 10Mb/s

### 芯片烧录方式


## 使用串口下载 SMT32F103 程序

参考网址:

- https://detail.tmall.com/item.htm?_u=i1qhgis7fb7e&id=626358704662
- https://www.cnblogs.com/leo0621/p/8478832.html
- https://blog.csdn.net/weixin_44226857/article/details/106303337

- 硬件: 芯片 CH340G 是 USB 转 TTL 模块, 也是 RS232 转串口;
  - CH340 配合上位机可以下载程序
  - 也可以当成 USART1 和串口助手通信
- 软件: FlyMcu 或 PZ-ISP(普中烧录软件)

|                           |                                       |
| ------------------------- | ------------------------------------- |
| CH340G 原理图(普中)       | ![img](./img/CH340G_SCH_PZ.png)       |
| CH340G 原理图(网上)       | ![img](./img/CH340G_SCH_Internet.png) |
| CH340G 和 STM32F03 连接图 | ![img](./img/CH340G_Connect.png)      |
| Boot1 和 Boot0            | ![img](./img/Boot0_Boot1.png)         |
| PZ-ISP 下载成功           | ![img](./img/PZ-ISP_download.png)     |
| FlyMCU 下载成功           | ![img](./img/FlyMcu_download.png)     |

- 注意:
  - DTR 和 RST 的高低选择需要根据原理图选择

## 使用 SWD 下载程序
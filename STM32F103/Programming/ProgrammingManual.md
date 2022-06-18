## 如何下载程序到 STM32F103C8

- 使用串口
- 使用 SWD 下载程序
- 使用 ST-Link
- 使用普中 STM32 仿真

## 使用串口

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
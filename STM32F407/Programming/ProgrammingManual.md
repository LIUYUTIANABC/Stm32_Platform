## 如何下载程序到 STM32F407

- 使用串口
- 使用 SWD 下载程序
- 使用 ST-Link
- 使用普中 STM32 仿真

## 使用串口

- 普中的 ISP 不支持烧录 STM32F4 的芯片
- 使用 FlyMcu 串口烧录失败
- STM32F4 还是尝试其他几种烧录方式

## 使用 SWD 下载程序

### 使用 CMSISI-DAP 下载

- 参考普中开发攻略
- 参考 STM32F103 文件夹中的 Programming - ProgrammingManual.md
  - 里面记录的是 STM32F103 的烧录详细内容；
  - 有 CMSIS-DAP 的使用方式

### 使用 ST-Link 下载

- 参考 STM32F103 文件夹中的 Programming - ProgrammingManual.md
  - 里面记录的是 STM32F103 的烧录详细内容；
  - 有 ST-Link 的使用方式

# C 语言深入学习

- [C 语言深入学习](#c-语言深入学习)
  - [结构体](#结构体)
  - [static 和 const 和 extern 和 volatile](#static-和-const-和-extern-和-volatile)
  - [编译器特性 __attribute__((packed))](#编译器特性-attributepacked)

## 结构体

C语言结构体初始化的四种方法

参考网址：https://blog.csdn.net/ericbar/article/details/79567108

![img](./img/2023-1-13_C_Struct_Init.jpg)

## static 和 const 和 extern 和 volatile

- static 静态变量，存储在 RAM 中，值不会改变，别的地方访问
- const 只读，常量，不能再次赋值
- extern 声明全局变量，在 c 文件中定义，h 文件中使用 extern 声明，别的地方可以访问
- volatile 编译器不会对变量优化，从内存中读取
  - 使用情况：
  - 1、中断中的变量
  - 2、各任务间的共享标志，系统状态
  - 3、硬件寄存器

联合使用

- static const int arr = 0； 定义一个 只读的全局静态常量，可以在头文件中定义
- static int arr[3] = {0,1,2};  定义一个全局的数组
  - 可以放到 c 文件中：表示在本文件有效
  - 可以放到 h 文件中：表示全局变量

## 编译器特性 __attribute__((packed))

参考网址：https://www.jianshu.com/p/9c307c3eb8a9

- 背景：在定义结构体中，会涉及到数据对齐，AT32 默认是 4 字节对齐；
  - 即使结构体中的元素是 u8 也会按照 4 字节分配空间

```
typedef struct
{
    int a;
    char b;
} test1;
// leath = sizeof(test1); 是 8 字节；
```

- 如果接收数据，前面即使是 1 字节，默认分配 4 字节的空间
  - 注意：那么后面的 data 会接收到乱码；因为，cmd 后面还有 3 个空字节

```
typedef struct
{
    u8 cmd;
    u32 data;
} test2;
// leath = sizeof(test1); 是 8 字节；
```

- 如何避免上面的问题？
  - 设置编译器特性，改变对齐方式
- 利用 __attribute__ 关键字，设置结构体或共用体的属性
- __attribute__ ((attribute-list))；
  - attribute-list：可选 6 个参数；aligned, packed.....
  - __attribute__((packed))
    - packed 属性的主要目的是让编译器更紧凑地使用内存；取消结构体优化对齐
    - 使用 1 字节为单位对齐
  - __attribute__((packed, aligned(1)))
    - aligned(x) 就是告诉编译器，以 x 个字节为单位进行对齐，x 只能是 1，或 2 的幂
    - 等价于 __attribute__((packed))

```
typedef struct __attribute__((packed))
{
    int a;
    char b;
} test1;
// leath = sizeof(test1); 是 5 字节；
```

######################################################################
# 创建 makefile 可执行文件
######################################################################
Get_main.exe: Get_main.i Get_main.s Get_main.o
	gcc -g -Wall main.c -o main.exe
	.\main.exe
Get_main.i: main.c main.h
	gcc -E main.c -o main.i
Get_main.s: main.c main.h
	gcc -S main.c -o main.s
Get_main.o: main.c main.h
	gcc -c main.c -o main.o
# 使用 .PHONY: 定义伪目标
.PHONY: Clean Clean1
Clean:
	del main.i
Clean1:
	del main.s
	del main.o
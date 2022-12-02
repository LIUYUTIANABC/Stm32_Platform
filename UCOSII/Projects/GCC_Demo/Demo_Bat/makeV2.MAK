######################################################################
# 创建 makefile 可执行文件
######################################################################
# 用变量表示所使用的开发工具，路径
GCC_PATH = C:\MinGW
MAKE = C:\MinGW\bin\make.exe
######################################################################
# 用变量表示各个目录
OBJ = ..\OBJ
SOURCE = ..\SOURCE
TARGET = ..\TEST
WORK = ..\WORK
PC = ..\..\PC
PORT = ..\..\PORT\PT
######################################################################
# 编译参数变量
C_FLAGS = gcc -g -Wall
######################################################################
# 链接参数变量
LINK_FLAGS =
######################################################################
# 包含文件变量
INCLUDES = $(SOURCE)\INCLUDES.H  \
           $(PC)\PC.H            \
           $(PORT)\PT.H
######################################################################
# 创建可执行文件(.exe)
######################################################################
$(TARGET)\TEST.EXE:                    \
                    $(WORK)\INCLUDES.H  \
                    $(OBJ)\INCLUDES.H  \
                    $(OBJ)\INCLUDES.H  \
                    $(OBJ)\INCLUDES.H

	COPY $(SOURCE)\LINK.LNK
	$(LINK) $(LINK_FLAGS) @LINK.LNK
	COPY $(OBJ)\TEST.EXE $(TARGET)\TEST.EXE
	DEL TEST.MAK
######################################################################
# 创建各个目标文件(OBJ)
$(OBJ)\PT.OBJ:                \
                $(PORT)\PT.C  \
                $(PORT)\PT.H
	COPY $(PORT)\PT.C PT.C
	$(CC) $(C_FLAGS) PT.C
$(OBJ)\PC.OBJ:                \
                $(PC)\PC.C  \
                $(PC)\PC.H
	COPY $(PC)\PC.C PC.C
	$(CC) $(C_FLAGS) PC.C
$(WORK)\INCLUDES.H:                \
                $(INCLUDES)
	COPY $(SOURCE)\INCLUDES.H INCLUDES.H
	COPY $(PC)\PC.H PC.H
	COPY $(PORT)\PT.H PT.H

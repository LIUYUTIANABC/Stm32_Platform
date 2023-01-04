######################################################################
# 创建 makefile 可执行文件
######################################################################
name = rick
curname = AddEng
name += $(curname)
src_path = C:\U_files\Git_Repo\Stm32_Platform\UCOSII\Projects\BatAndMake\src
obj_path = C:\U_files\Git_Repo\Stm32_Platform\UCOSII\Projects\BatAndMake\obj
Get_main.exe: Get_main.i Get_main.s Get_main.o
	gcc -g -Wall $(src_path)\main.c -o .\main.exe
Get_main.i: $(src_path)\main.c
	gcc -E $(src_path)\main.c -o $(obj_path)\main.i
Get_main.s: $(src_path)\main.c
	gcc -S $(src_path)\main.c -o $(obj_path)\main.s
Get_main.o: $(src_path)\main.c
	gcc -c $(src_path)\main.c -o $(obj_path)\main.o
# 使用 .PHONY: 定义伪目标
.PHONY: Clean Clean1
Clean:
	del $(obj_path)\main.i
	del $(obj_path)\main.s
	del $(obj_path)\main.o
Print:
	echo $(curname)
	echo $(name)

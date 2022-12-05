#----------------------------------------------------------------------------------
# File Name: script.ps1
# Create Date: 2022-12-05 13:42:34
# Developer: Rick Liu
# Version: 1.0.0
# Copyright:
# Comment:
#----------------------------------------------------------------------------------

param (
    [Parameter()]
    [String]$clear = "No"
)

$SRC_PATH = (Get-Location).ToString() + '\src'
$OBJ_PATH = (Get-Location).ToString() + '\obj'
$MAKE_PATH = (Get-Location).ToString() + '\make'
$MAIN = "#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf(`"Hello World!!  \n`");
    return 1;
}
"
$MAKE = "######################################################################
# 创建 makefile 可执行文件
######################################################################
name = rick
curname = AddEng
name += `$(curname)
src_path = $SRC_PATH
obj_path = $OBJ_PATH
Get_main.exe: Get_main.i Get_main.s Get_main.o
	gcc -g -Wall `$(src_path)\main.c -o .\main.exe
Get_main.i: `$(src_path)\main.c
	gcc -E `$(src_path)\main.c -o `$(obj_path)\main.i
Get_main.s: `$(src_path)\main.c
	gcc -S `$(src_path)\main.c -o `$(obj_path)\main.s
Get_main.o: `$(src_path)\main.c
	gcc -c `$(src_path)\main.c -o `$(obj_path)\main.o
# 使用 .PHONY: 定义伪目标
.PHONY: Clean Clean1
Clean:
	del `$(obj_path)\main.i
	del `$(obj_path)\main.s
	del `$(obj_path)\main.o
Print:
	echo `$(curname)
	echo `$(name)
"

#----------------------------------------------------------------------------------
# Function name: CheckMcuInfo()
# Input:
# Output:
# Comment: Check and confirmed the MCU models
#----------------------------------------------------------------------------------
function ClearFile {
    if ($clear -eq "Yes") {
        Remove-Item -path $SRC_PATH -Recurse
        Remove-Item -path $OBJ_PATH -Recurse
        Remove-Item -path $MAKE_PATH -Recurse
        Remove-Item -path main.exe -Recurse
        Write-Host "---- Clear files done!" -ForegroundColor "Green"
    }
}
#----------------------------------------------------------------------------------
# Function name: FormatWriteOutput()
# Input:
# Output:
# Comment: format output string
#----------------------------------------------------------------------------------
function FormatWriteOutput($fstring) {
    Write-Host "-------------------------------------------------".Substring($fstring.length / 2) -NoNewline -ForegroundColor "Cyan"
    Write-Host " $fstring " -NoNewline -ForegroundColor "DarkCyan"
    Write-Host "-------------------------------------------------".Substring($fstring.length / 2 + $fstring.length % 2) -ForegroundColor "Cyan"
}
#----------------------------------------------------------------------------------
# Function name: CheckFolder
# Input:
# Output: Chenck path and files
# Comment:
#----------------------------------------------------------------------------------
function CheckFolder {
    Write-Output "---- Check SRC_PATH"
    if (-not (Test-Path $SRC_PATH)) {
        Write-Host "Create obj folder:$SRC_PATH" -ForegroundColor "Green"
        New-Item -Path $SRC_PATH -ItemType "Directory"
        New-Item -Path $SRC_PATH -Name "main.c" -ItemType "file" -Value $MAIN
    }
    else {
        Write-Host "Pass" -ForegroundColor "Green"
    }
    Write-Output "---- Check OBJ_PATH"
    if (-not (Test-Path $OBJ_PATH)) {
        Write-Host "Create obj folder:$OBJ_PATH" -ForegroundColor "Green"
        New-Item -Path $OBJ_PATH -ItemType "Directory"
    }
    else {
        Write-Host "Pass" -ForegroundColor "Green"
    }
    Write-Output "---- Check MAKE_PATH"
    if (-not (Test-Path $MAKE_PATH)) {
        Write-Host "Create obj folder:$MAKE_PATH" -ForegroundColor "Green"
        New-Item -Path $MAKE_PATH -ItemType "Directory"
        New-Item -Path $MAKE_PATH -Name "MAKEFILE.MAK" -ItemType "file" -Value $MAKE
    }
    else {
        Write-Host "Pass" -ForegroundColor "Green"
    }
}
#----------------------------------------------------------------------------------
# Function name: GCC_Compile
# Input:
# Output: compile all .c files
# Comment:
#----------------------------------------------------------------------------------
function GCC_Compile{
    Write-Output "---- GCC -E"
    gcc -E $SRC_PATH\main.c -o $OBJ_PATH\main.i
    Write-Output "---- GCC -S"
    gcc -S $SRC_PATH\main.c -o $OBJ_PATH\main.s
    Write-Output "---- GCC -c"
    gcc -c $SRC_PATH\main.c -o $OBJ_PATH\main.o
    Write-Output "---- GCC Compile"
    gcc -g -Wall $SRC_PATH\main.c -o .\main.exe
    # you can call 'makefile' to run the 'main.exe' too;
    # make -f $MAKE_PATH\MAKEFILE.MAK
}

#----------------------------------------------------------------------------------
# Function name:
# Input:
# Output:
# Comment:
#----------------------------------------------------------------------------------
function main{
    Write-Output "-------- Check Folder and file --------"
    CheckFolder
    Write-Output "-------- Use GCC to compile --------"
    GCC_Compile

    if ((Test-Path "main.exe")) {
        Write-Host "================= Your main function output =================" -ForegroundColor "Red"
        .\main.exe
        Write-Host "================= Your main function END =================" -ForegroundColor "Red"
    }
    else {
        Write-Host "No 'main.exe' file" -ForegroundColor "Red"
    }
    Write-Output "-------- Clear files? --------"
    Write-Host "---- $clear" -ForegroundColor "Green"
    ClearFile
}

#----------------------------------------------------------------------------------
# Catch the compile
#----------------------------------------------------------------------------------
$StopWatch = New-Object System.Diagnostics.Stopwatch
$StopWatch.Start()

FormatWriteOutput('Process Start')
Main

Write-Host "---- Total Build time: " -NoNewline
Write-Host ($StopWatch.Elapsed).ToString() -ForegroundColor "Cyan"
Write-Host "---- Compiled date time: " -NoNewline
Write-Host (get-date -format "yyyy-MM-dd HH:mm:ss.fff") -ForegroundColor "Cyan"

#----------------------------------------------------------------------------------
# Restore the path to the system, because there are some changes during the script.
#----------------------------------------------------------------------------------
$StopWatch.Stop()
return;

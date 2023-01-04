@REM ***************************************************************
@REM * File: Make a batch file for build project
@REM * Name: Rick.Liu
@REM * Time: 2022-12-2
@REM ***************************************************************

@ECHO OFF
COLOR 0F
ECHO ***************************************************************
ECHO *  Use GCC and MAKE tool to compile C project !!!
ECHO ***************************************************************

@REM ---------------------------------------------------------------
@REM - Set Variables
@REM ---------------------------------------------------------------
SET SRC_PATH=.\src
SET OBJ_PATH=.\obj
SET MAKE_PATH=.\make
SET MAIN_C=.\src\main.c

@REM ---------------------------------------------------------------
@REM - Check folders and files
@REM ---------------------------------------------------------------
@REM ECHO -------- Check SRC_PATH --------
@REM IF not EXIST %SRC_PATH% (
@REM     MD src
@REM     ECHO     Creat a new 'src' folder;
@REM     ECHO     Pass
@REM ) ELSE (
@REM     ECHO     Pass
@REM )
@REM ECHO -------- Check OBJ_PATH --------
@REM IF not EXIST %OBJ_PATH% (
@REM     MD obj
@REM     ECHO     Creat a new 'obj' folder;
@REM     ECHO     Pass
@REM ) ELSE (
@REM     ECHO     Pass
@REM )
@REM ECHO -------- Check MAKE_PATH --------
@REM IF not EXIST %MAKE_PATH% (
@REM     MD make
@REM     ECHO     Creat a new 'make' folder;
@REM     ECHO     Pass
@REM ) ELSE (
@REM     ECHO     Pass
@REM )

@REM Call powershell script;
@REM POWERSHELL -ExecutionPolicy Bypass -File script.ps1
powershell.exe -executionpolicy remotesigned -File script.ps1

PAUSE

@REM @ECHO OFF
@REM ECHO ********************************************
@REM ECHO *  Batch file
@REM ECHO ********************************************
@REM MD WORK
@REM MD OBJ
@REM CD WORK
@REM COPY ..\..\Demo_Make\makefile makeV2.MAK
@REM REM make -f makeV2.MAK
@REM CD ..\
@REM DEL WORK
@REM DEL OBJ
@REM RD .\WORK
@REM RD .\OBJ
@REM PAUSE

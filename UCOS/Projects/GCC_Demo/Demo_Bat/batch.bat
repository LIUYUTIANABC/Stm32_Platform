@ECHO OFF
ECHO ********************************************
ECHO *  Batch file
ECHO ********************************************
MD WORK
MD OBJ
CD WORK
COPY ..\..\Demo_Make\makefile makeV2.MAK
REM make -f makeV2.MAK
CD ..\
DEL WORK
DEL OBJ
RD .\WORK
RD .\OBJ
PAUSE

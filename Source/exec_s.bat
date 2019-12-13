@echo off
If not exist %1.asm echo File Does Not Exist
If not exist %1.asm goto end

If exist %1.obj erase %1.obj
If exist %1.exe erase %1.exe

masm %1 /z /Zi /Zd /v ,%1 ;
If not exist %1.obj goto end

link %1,%1,nul;
If not exist %1.exe goto end

%1.exe

:end
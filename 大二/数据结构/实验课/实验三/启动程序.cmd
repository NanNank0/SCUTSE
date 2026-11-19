@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\Qt\6.11.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%"
start "" "%~dp0程序\SortCompare.exe" --data "%~dp0测试数据" --results "%~dp0运行结果\timings.csv"

@echo off
chcp 65001 >nul
setlocal
cd /d "%~dp0"
set "PATH=C:\Qt\6.11.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%"
echo 1. BFS / DFS
echo 2. 拓扑排序与判环
echo 3. 最小费用换古筝
choice /c 123 /n /m "请选择题号 [1/2/3]："
set "problem=%errorlevel%"
start "" "%~dp0程序\exp4_%problem%.exe" --data "%~dp0测试数据"

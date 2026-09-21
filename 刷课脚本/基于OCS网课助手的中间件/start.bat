@echo off
chcp 65001 >nul
echo ========================================
echo   OCS DeepSeek 中间件
echo ========================================
echo.

REM 检查 Python 是否安装
python --version >nul 2>&1
if errorlevel 1 (
    echo ❌ 未检测到 Python，请先安装 Python
    echo 下载地址: https://www.python.org/downloads/
    pause
    exit /b
)

REM 检查依赖是否安装
python -c "import fastapi" >nul 2>&1
if errorlevel 1 (
    echo 📦 正在安装依赖...
    pip install -r requirements.txt
    echo.
)

echo 🚀 启动中间件...
echo.
python server.py

pause
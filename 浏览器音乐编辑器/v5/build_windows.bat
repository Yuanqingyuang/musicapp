@echo off
REM ========================================
REM FL Studio Editor - Windows 构建脚本
REM ========================================

setlocal enabledelayedexpansion

echo.
echo ========================================
echo   FL Studio Editor - Windows 构建
echo ========================================
echo.

REM 检查 Qt 环境
where qmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] 未找到 Qt6，请先设置 Qt6 的 PATH 环境变量
    echo 例如: set PATH=C:\Qt\6.x.x\msvc2019_64\bin;%%PATH%%
    echo 或者在 Qt Creator 中打开项目
    echo.
    pause
    exit /b 1
)

echo [INFO] 检测到 Qt:
qmake --version | findstr /i "qt"
echo.

REM 设置构建目录
set BUILD_DIR=%~dp0build-windows
if exist "%BUILD_DIR%" (
    echo [INFO] 清理旧的构建目录...
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

REM 配置 CMake
echo [INFO] 配置 CMake 项目...
cmake .. -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo [ERROR] CMake 配置失败
    cd /d %~dp0
    pause
    exit /b 1
)

REM 构建
echo.
echo [INFO] 编译项目...
cmake --build . --config Release --parallel
if %errorlevel% neq 0 (
    echo [ERROR] 编译失败
    cd /d %~dp0
    pause
    exit /b 1
)

echo.
echo ========================================
echo [SUCCESS] 构建完成!
echo   可执行文件: %BUILD_DIR%\Release\FLStudioEditor.exe
echo ========================================
echo.

REM 询问是否生成安装包
set /p choice=是否生成安装包? (y/n):
if /i "%choice%"=="y" (
    echo.
    echo [INFO] 生成安装包...
    cpack -C Release
    echo.
    echo ========================================
    echo [SUCCESS] 安装包已生成在 build-windows 目录
    echo ========================================
)

cd /d %~dp0
echo.
echo 按任意键退出...
pause >nul

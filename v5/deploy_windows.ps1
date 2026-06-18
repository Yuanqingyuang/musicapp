# ========================================
# FL Studio Editor - Windows 部署脚本
# ========================================
# 使用方法:
#   1. 先运行 build_windows.bat 构建项目
#   2. 运行 deploy_windows.ps1 部署 Qt 库和创建发布包
# ========================================

param(
    [string]$BuildDir = "$PSScriptRoot\build-windows",
    [string]$DeployDir = "$PSScriptRoot\dist-windows",
    [string]$Config = "Release"
)

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  FL Studio Editor - Windows 部署" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 检查构建产物
$ExePath = "$BuildDir\$Config\FLStudioEditor.exe"
if (-not (Test-Path $ExePath)) {
    Write-Host "[ERROR] 未找到可执行文件: $ExePath" -ForegroundColor Red
    Write-Host "  请先运行 build_windows.bat 构建项目" -ForegroundColor Yellow
    exit 1
}

# 检测 Qt 路径并找到 windeployqt
$QtBinDir = $null
$WinDeployQt = $null

# 方法 1: 从 qmake 检测
try {
    $qmake = Get-Command qmake -ErrorAction Stop
    $QtBinDir = Split-Path $qmake.Source -Parent
    $WinDeployQt = Join-Path $QtBinDir "windeployqt.exe"
} catch {
    # 方法 2: 尝试常见安装路径
    $QtCandidates = @(
        "C:\Qt\6.7.0\msvc2019_64\bin",
        "C:\Qt\6.6.0\msvc2019_64\bin",
        "C:\Qt\6.5.0\msvc2019_64\bin",
        "C:\Qt\6.4.0\msvc2019_64\bin",
        "C:\Qt\6.3.0\msvc2019_64\bin",
        "C:\Qt\6.2.0\msvc2019_64\bin"
    )
    foreach ($p in $QtCandidates) {
        if (Test-Path "$p\windeployqt.exe") {
            $QtBinDir = $p
            $WinDeployQt = "$p\windeployqt.exe"
            break
        }
    }
}

if (-not (Test-Path $WinDeployQt)) {
    Write-Host "[ERROR] 未找到 windeployqt.exe" -ForegroundColor Red
    Write-Host "  请确保已安装 Qt6 并配置了环境变量" -ForegroundColor Yellow
    exit 1
}

Write-Host "[INFO] Qt 目录: $QtBinDir" -ForegroundColor Green
Write-Host "[INFO] windeployqt: $WinDeployQt" -ForegroundColor Green
Write-Host ""

# 准备部署目录
if (Test-Path $DeployDir) {
    Write-Host "[INFO] 清理旧的部署目录..." -ForegroundColor Yellow
    Remove-Item $DeployDir -Recurse -Force
}
New-Item -ItemType Directory -Path $DeployDir | Out-Null

# 复制可执行文件
Copy-Item $ExePath $DeployDir
Write-Host "[INFO] 已复制可执行文件" -ForegroundColor Green

# 运行 windeployqt 复制 Qt 运行时库
Write-Host "[INFO] 运行 windeployqt 部署 Qt 运行时库..." -ForegroundColor Yellow
$Env:Path = "$QtBinDir;" + $Env:Path
$DeployExe = Join-Path $DeployDir "FLStudioEditor.exe"
& $WinDeployQt `
    --no-compiler-runtime `
    --no-translations `
    --no-system-d3d-compiler `
    $DeployExe

if ($LASTEXITCODE -ne 0) {
    Write-Host "[WARNING] windeployqt 报告了一些问题，但程序可能仍然可以运行" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "[SUCCESS] 部署完成!" -ForegroundColor Green
Write-Host "  部署目录: $DeployDir" -ForegroundColor White
Write-Host "  可执行文件: $DeployExe" -ForegroundColor White
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 创建 ZIP 发布包
$ZipPath = "$PSScriptRoot\FLStudioEditor-windows-x64.zip"
Write-Host "[INFO] 创建 ZIP 发布包..." -ForegroundColor Yellow

# 压缩部署目录
Compress-Archive -Path "$DeployDir\*" -DestinationPath $ZipPath -Force

$ZipSizeMB = [math]::Round((Get-Item $ZipPath).Length / 1MB, 2)
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "[SUCCESS] ZIP 发布包已创建!" -ForegroundColor Green
Write-Host "  文件名: $ZipPath" -ForegroundColor White
Write-Host "  大小: ${ZipSizeMB} MB" -ForegroundColor White
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 创建一个快速启动说明文件
$ReadmeContent = @"
FL Studio Editor for Windows - 运行说明
=========================================

1. 运行 FLStudioEditor.exe 启动程序
2. 首次运行 Windows 可能会弹出防火墙提示，请允许访问
3. 如果缺少 VC++ 运行时，请安装:
   https://learn.microsoft.com/cpp/windows/latest-supported-vc-redist

项目文件说明:
- *.flproj 项目文件 (JSON 格式)
- 导出的 WAV 文件保存在您选择的目录中

问题反馈: 请访问项目仓库
"@
Set-Content -Path "$DeployDir\README.txt" -Value $ReadmeContent -Encoding UTF8

Write-Host "部署完成。按 Enter 键打开部署目录..." -ForegroundColor Gray
$null = Read-Host
try {
    Invoke-Item $DeployDir
} catch {
    # 静默失败
}

# FL Studio Editor v5 - 纯本地音乐编辑器

v5 是一个完全基于 C++17 和 Qt6 的本地桌面音乐编辑软件，不依赖 HTML/浏览器技术。风格类似 FL Studio，提供完整的音乐制作闭环体验。

## 核心功能

### 钢琴卷帘编辑器 (Piano Roll)
- 覆盖 88 键完整钢琴键盘范围（C0-B7）
- 鼠标左键点击创建音符，右键删除
- 拖拽移动音符位置，拖拽边缘调整时长
- 播放时高亮跟随当前播放位置
- 网格吸附功能（1/4、1/8、1/16）
- 滚轮缩放，支持水平和垂直方向缩放

### 步进音序器 (Step Sequencer)
- 16 步进鼓机风格网格
- 每行对应一个鼓组音轨
- 实时高亮当前播放步进位置
- 点击切换激活状态

### 混音器面板 (Mixer)
- 每条音轨独立的音量滑块（0-100%）
- 声像旋钮（-100 到 +100）
- 静音/独奏按钮
- 实时数值显示

### 传输控制栏 (Transport)
- 播放/暂停按钮（图标动态切换）
- 停止按钮
- BPM 微调框（60-200）
- 循环开关
- 时间位置显示（小节:拍:细分）

### 多乐器合成引擎

| 乐器 | 合成类型 | 特点 |
|-----|---------|------|
| Piano | FM 合成 | 调制频率 1.5x，增益 200，明亮钢琴音色 |
| Organ | 加法合成 | 6 谐波叠加，丰满风琴音色 |
| Synth | 减法合成 | 锯齿波 + 低通滤波，合成器 lead |
| Pad | 铺底音色 | 5 音合唱偏移，宽广延音 |
| Bass | 减法合成 | 低频锯齿波，紧凑贝斯音色 |

**ADSR 包络控制**：Attack、Decay、Sustain、Release 四个参数独立可调

**滤波器设置**：
- 类型：低通（Lowpass）、高通（Highpass）、带通（Bandpass）
- 频率：100Hz - 10kHz 可调

### 项目管理
- JSON 格式保存和加载项目
- 自动创建默认项目（包含钢琴和鼓组轨道）
- 未保存更改提醒

### 音频导出
- 离线渲染整个项目
- 导出为 WAV 格式（44.1kHz/16bit 立体声）

---

## 系统要求

### 硬件要求
- **处理器**：x86_64 或 ARM64 架构
- **内存**：建议 4GB 以上
- **磁盘空间**：约 100MB
- **音频设备**：支持标准音频输出

### 软件要求

| 组件 | 最低版本 | 推荐版本 |
|-----|---------|---------|
| 操作系统 | Windows 10 / macOS 10.14 / Ubuntu 20.04 | 最新稳定版 |
| C++ 编译器 | GCC 11 / Clang 14 / MSVC 2022 | 最新稳定版 |
| CMake | 3.16 | 3.24+ |
| Qt6 | 6.0 | 6.5+ |

---

## 安装依赖

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev qt6-multimedia-dev libgl1-mesa-dev
```

### Fedora / RHEL / CentOS

```bash
sudo dnf install -y cmake gcc-c++ qt6-qtbase qt6-qtmultimedia mesa-libGL
```

### Arch Linux

```bash
sudo pacman -S --needed cmake base-devel qt6-base qt6-multimedia
```

### macOS (Homebrew)

```bash
brew install cmake qt@6
```

如果使用 Qt6 默认路径，需要手动设置：
```bash
export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"
export PKG_CONFIG_PATH="/opt/homebrew/opt/qt@6/lib/pkgconfig:$PKG_CONFIG_PATH"
```

### Windows (vcpkg)

```bash
# 安装 vcpkg（如果尚未安装）
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat

# 安装 Qt6
vcpkg install qtbase:x64-windows qtmultimedia:x64-windows
```

或者使用 Qt 官方安装包：
1. 下载 Qt Online Installer：https://www.qt.io/download-qt-installer
2. 安装时选择 Qt 6.5+ 和 MSVC 2022 组件
3. 设置 QTDIR 环境变量

---

## 构建步骤

### 标准构建流程

```bash
# 1. 进入 v5 目录
cd v5

# 2. 创建并进入构建目录
mkdir -p build
cd build

# 3. 运行 CMake 配置
cmake ..

# 4. 编译项目（使用多核加速）
cmake --build . -j$(nproc)

# 5. 运行程序
./FLStudioEditor      # Linux/macOS
.\FLStudioEditor.exe  # Windows
```

### Qt 路径配置

如果 CMake 找不到 Qt6，需要指定 Qt 安装路径：

```bash
# 使用 qt6-cmake 工具查找
cmake .. -DCMAKE_PREFIX_PATH=$(qt6-cmake --prefix)

# 或者手动指定
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6
```

### Qt Creator IDE

1. 打开 Qt Creator
2. File → Open File or Project
3. 选择 `v5/CMakeLists.txt`
4. 配置 Kit（选择 Qt6 版本）
5. 点击 Configure Project
6. 点击 Run（Ctrl+R）编译并运行

---

## 目录结构

```
v5/
├── CMakeLists.txt              # CMake 构建配置
├── README.md                   # 本文档
├── include/                    # 头文件目录
│   ├── core/                   # 核心数据模型
│   │   ├── Note.h              # 音符数据结构
│   │   ├── Track.h             # 音轨数据结构
│   │   └── Project.h          # 项目数据管理
│   ├── audio/                  # 音频引擎
│   │   ├── AudioEngine.h      # 实时音频播放引擎
│   │   ├── Scheduler.h        # 播放调度器
│   │   ├── Synthesizer.h      # 多类型合成器
│   │   ├── DrumSample.h       # 鼓采样生成器
│   │   └── WavExporter.h      # WAV 文件导出
│   ├── ui/                     # 用户界面
│   │   ├── MainWindow.h       # 主窗口
│   │   ├── PianoRollWidget.h  # 钢琴卷帘编辑器
│   │   ├── PianoKeyboard.h    # 钢琴键盘组件
│   │   ├── StepSequencerWidget.h  # 步进音序器
│   │   ├── MixerWidget.h      # 混音器面板
│   │   ├── TransportWidget.h  # 传输控制栏
│   │   └── InstrumentEditor.h  # 乐器编辑器
│   └── utils/                  # 工具类
│       └── AudioBuffer.h       # 音频缓冲区
└── src/                        # 源代码目录
    ├── main.cpp                # 应用入口
    ├── core/
    │   └── Project.cpp         # 项目管理实现
    ├── audio/
    │   ├── AudioEngine.cpp     # 音频引擎实现
    │   ├── Scheduler.cpp       # 调度器实现
    │   ├── Synthesizer.cpp     # 合成器实现
    │   ├── DrumSample.cpp      # 鼓采样实现
    │   └── WavExporter.cpp     # WAV 导出实现
    ├── ui/
    │   ├── MainWindow.cpp       # 主窗口实现
    │   ├── PianoRollWidget.cpp  # 钢琴卷帘实现
    │   ├── PianoKeyboard.cpp   # 钢琴键盘实现
    │   ├── StepSequencerWidget.cpp  # 步进音序器实现
    │   ├── MixerWidget.cpp     # 混音器实现
    │   ├── TransportWidget.cpp # 传输控制实现
    │   └── InstrumentEditor.cpp # 乐器编辑器实现
    └── utils/
        └── AudioBuffer.cpp     # 音频缓冲区实现
```

---

## 使用指南

### 快速开始

1. **启动程序**：运行 `./FLStudioEditor`
2. **创建音符**：切换到钢琴卷帘视图，点击空白处创建音符
3. **编辑节奏**：切换到步进音序器，点击网格编辑鼓点
4. **调整音色**：在右侧乐器编辑器中调整 ADSR 和滤波器
5. **混音**：使用混音器调整各轨道音量和声像
6. **播放**：按空格键或点击播放按钮试听
7. **保存**：Ctrl+S 保存项目
8. **导出**：Ctrl+E 导出 WAV 音频

### 快捷键

| 快捷键 | 功能 |
|--------|------|
| Space | 播放/暂停 |
| Escape | 停止 |
| Ctrl+N | 新建项目 |
| Ctrl+O | 打开项目 |
| Ctrl+S | 保存项目 |
| Ctrl+Shift+S | 另存为 |
| Ctrl+E | 导出音频 |
| Ctrl+1 | 切换到钢琴卷帘 |
| Ctrl+2 | 切换到步进音序器 |
| F11 | 全屏模式 |

### 钢琴卷帘操作

| 操作 | 方法 |
|------|------|
| 创建音符 | 左键点击空白位置 |
| 删除音符 | 右键点击音符 |
| 移动音符 | 拖拽音符中心 |
| 调整时长 | 拖拽音符左/右边缘 |
| 缩放 | 鼠标滚轮 |

---

## 与 v4 版本对比

| 特性 | v4 (Electron) | v5 (Qt/C++) |
|------|--------------|-------------|
| 技术栈 | HTML/JS + Electron | C++17 + Qt6 |
| 运行方式 | Chromium 渲染引擎 | 原生系统窗口 |
| 音频后端 | Web Audio API | Qt Multimedia |
| UI 渲染 | HTML Canvas | QPainter |
| 包体积 | ~100MB+ | ~20MB |
| 启动速度 | 3-5 秒 | < 1 秒 |
| 内存占用 | 200-500MB | 50-100MB |
| 音频质量 | 受限于浏览器 | 原生音质 |
| 系统集成 | 中等 | 高（原生窗口、系统主题） |
| 跨平台 | 优秀 | 优秀 |

---

## 技术架构

### 模块说明

**core 层（数据模型）**
- `Note`：音符数据（音高、起始步、时长、力度）
- `Track`：音轨数据（类型、音量、声像、静音、独奏、步进序列）
- `Project`：项目管理（轨道集合、音符集合、BPM、项目保存加载）

**audio 层（音频引擎）**
- `AudioEngine`：基于 Qt Multimedia 的实时音频输出
- `Scheduler`：播放调度器，处理 BPM 计算和音符触发
- `Synthesizer`：多类型合成器（FM、加法、减法、铺底）
- `DrumSample`：程序化生成鼓采样（Kick、Snare、Hi-Hat 等）
- `WavExporter`：离线渲染和 WAV 文件导出

**ui 层（用户界面）**
- `MainWindow`：主窗口，整合所有编辑器组件
- `PianoRollWidget`：钢琴卷帘，可视化音符编辑
- `StepSequencerWidget`：步进音序器，鼓机风格编辑
- `MixerWidget`：混音器面板，轨道控制
- `TransportWidget`：传输控制栏
- `InstrumentEditor`：乐器编辑器，ADSR 和滤波器

### 鼓采样音色

| 名称 | 合成方式 | 时长 | 特点 |
|-----|---------|------|------|
| Kick | 正弦频率扫描 + 指数衰减 | 0.5s | 低频 60Hz 起始，快速衰减 |
| Snare | 噪声混合 + 正弦 | 0.3s | 噪声 70% + 180Hz 正弦 30% |
| Hi-Hat | 滤波噪声 | 0.1s | 高频滤波，快速衰减 |
| Clap | 多段噪声包络 | 0.3s | 3 段瞬态噪声叠加 |
| Tom | 正弦频率扫描 | 0.4s | 100Hz 起始，中速衰减 |
| Crash | 噪声包络 | 1.0s | 金属感噪声，长衰减 |
| Ride | 噪声 + 正弦混合 | 0.5s | 400Hz 正弦 + 噪声 |
| Perc | 正弦频率扫描 | 0.2s | 短促打击感 |

---

## 故障排除

### CMake 找不到 Qt6

```bash
# 方法 1：设置 Qt6 路径
cmake .. -DCMAKE_PREFIX_PATH=$(qt6-config --prefix)

# 方法 2：使用 qt-cmake 工具
cmake .. -DCMAKE_PREFIX_PATH=$(which qt6-cmake | xargs dirname | xargs dirname)
```

### 编译报错缺少头文件

确保安装了完整的 Qt6 开发包：
```bash
# Ubuntu
sudo apt install qt6-base-dev qt6-multimedia-dev

# macOS
brew install qt@6
```

### 运行时无声音

1. 检查系统音量设置
2. 确认音频设备正常
3. 尝试使用不同的音频输出设备
4. 检查 Qt Multimedia 音频插件是否正确安装

### 界面显示异常

尝试设置 Qt 样式为 Fusion：
```bash
./FLStudioEditor --style fusion
```

---

## 版本历史

- **v5.0** (2024)：Qt/C++ 完全重写，高性能原生实现

---

## 许可证

MIT License

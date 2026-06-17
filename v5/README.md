# FL Studio Editor v5 - 纯本地音乐编辑器

v5 是一个完全基于 C++17 和 Qt6 的本地桌面音乐编辑软件，不依赖 HTML/浏览器技术。风格类似 FL Studio，提供完整的音乐制作闭环体验。

## 特性

- **钢琴卷帘 (Piano Roll)**：可视化音符编辑，支持拖拽、调整长度、删除
- **步进音序器 (Step Sequencer)**：16 步鼓机风格节奏编排
- **多乐器合成引擎**：
  - FM 合成（钢琴等）
  - 加法合成（管风琴等）
  - 减法合成（贝斯等）
  - 铺底音色（Pad）
- **ADSR 包络 + 滤波器**：每乐器独立设置
- **混音器**：音量、声像、静音、独奏控制
- **项目管理**：JSON 格式保存/加载
- **音频导出**：离线渲染导出 WAV 文件
- **暗色主题**：专业 DAW 风格界面

## 项目结构

```
v5/
├── CMakeLists.txt          # CMake 构建配置
├── README.md               # 本文件
├── include/                # 头文件
│   ├── core/               # 核心数据模型
│   │   ├── Note.h
│   │   ├── Track.h
│   │   └── Project.h
│   ├── audio/              # 音频引擎
│   │   ├── AudioEngine.h   # QAudioSink 实时输出
│   │   ├── Scheduler.h     # 播放调度器
│   │   ├── Synthesizer.h   # 合成器（FM/加法/减法/Pad）
│   │   ├── DrumSample.h    # 鼓样本生成
│   │   └── WavExporter.h   # WAV 导出
│   ├── ui/                 # 用户界面
│   │   ├── MainWindow.h    # 主窗口
│   │   ├── PianoRollWidget.h
│   │   ├── PianoKeyboard.h
│   │   ├── StepSequencerWidget.h
│   │   ├── MixerWidget.h
│   │   ├── TransportWidget.h
│   │   └── InstrumentEditor.h
│   └── utils/              # 工具类
│       └── AudioBuffer.h
└── src/                    # 源文件
    ├── main.cpp
    ├── core/
    │   ├── Project.cpp
    │   ├── Track.cpp
    │   └── Note.cpp
    ├── audio/
    │   ├── AudioEngine.cpp
    │   ├── Scheduler.cpp
    │   ├── Synthesizer.cpp
    │   ├── DrumSample.cpp
    │   └── WavExporter.cpp
    ├── ui/
    │   ├── MainWindow.cpp
    │   ├── PianoRollWidget.cpp
    │   ├── PianoKeyboard.cpp
    │   ├── StepSequencerWidget.cpp
    │   ├── MixerWidget.cpp
    │   ├── TransportWidget.cpp
    │   └── InstrumentEditor.cpp
    └── utils/
        └── AudioBuffer.cpp
```

## 构建要求

- **C++17** 兼容编译器（GCC 11+, Clang 14+, MSVC 2022+）
- **CMake** 3.16+
- **Qt6**（Core, Widgets, Multimedia 模块）

### 安装依赖

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-multimedia-dev libgl1-mesa-dev
```

#### macOS (Homebrew)
```bash
brew install cmake qt@6
```

#### Windows (vcpkg)
```bash
vcpkg install qtbase qtmultimedia
```

## 构建步骤

```bash
cd v5
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
```

### macOS 特殊说明
如果 Qt6 通过 Homebrew 安装，可能需要指定路径：
```bash
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
```

## 运行

```bash
./FLStudioEditor
```

## 使用说明

### 快捷键

| 快捷键 | 功能 |
|--------|------|
| Space | 播放/停止 |
| Ctrl+N | 新建项目 |
| Ctrl+O | 打开项目 |
| Ctrl+S | 保存项目 |
| Ctrl+Shift+S | 另存为 |
| Ctrl+E | 导出音频 |

### 操作指南

1. **钢琴卷帘**：点击空白处添加音符，拖拽移动，拖拽边缘调整长度，右键删除
2. **步进音序器**：点击网格开关鼓点
3. **乐器编辑**：右侧面板添加/编辑乐器，调整 ADSR 和滤波器
4. **混音器**：调整各轨道音量和声像
5. **传输控制**：设置 BPM，开启/关闭循环

## 与 v4 的区别

| 特性 | v4 (Electron) | v5 (Qt/C++) |
|------|--------------|-------------|
| 技术栈 | HTML/JS + Electron | C++17 + Qt6 |
| 运行方式 | 基于 Chromium | 纯本地原生 |
| 音频后端 | Web Audio API | QAudioSink |
| 渲染 | HTML Canvas | QPainter |
| 包大小 | ~100MB+ | ~20MB |
| 启动速度 | 较慢 | 快 |
| 内存占用 | 较高 | 低 |

## 许可证

MIT License

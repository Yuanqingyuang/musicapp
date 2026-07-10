# FL Studio Style Music Editor

一款类似 FL Studio 的音乐编辑软件，提供两种版本：基于 Electron 的浏览器友好版本（v4）和基于 Qt/C++ 的高性能原生版本（v5）。

## 功能特性

### 钢琴卷帘编辑器 (Piano Roll)
- **音符编辑**：覆盖 3 个八度（C3-B5），支持可视化编辑
- **鼠标操作**：左键点击创建音符，右键删除，拖拽移动和调整长度
- **播放同步**：播放时音符高亮跟随当前位置
- **网格吸附**：可调节的网格精度（1/4、1/8、1/16）
- **缩放功能**：支持水平和垂直方向缩放

### 步进音序器 (Step Sequencer)
- **16 步进网格**：鼓机风格的节奏编排界面
- **多轨道支持**：可同时编辑多条鼓组轨道（Kick、Snare、Hi-Hat、Clap、Tom 等）
- **实时反馈**：播放时高亮当前步进位置
- **一键切换**：点击即可开启/关闭当前步进

### 混音器 (Mixer)
- **独立轨道控制**：每条音轨独立的音量推子
- **声像调节**：支持左/右声道声像定位
- **静音/独奏**：快速隔离或静音特定音轨
- **可视化反馈**：实时显示当前音量值

### 传输控制 (Transport)
- **播放控制**：播放、暂停、停止功能
- **BPM 调节**：支持 60-200 BPM 范围调节
- **循环播放**：可开关的循环模式
- **时间显示**：实时显示当前小节和拍数

### 多乐器合成引擎

| 合成类型 | 音色特点 | 适用场景 |
|---------|---------|---------|
| FM 合成 | 明亮金属感 | 钢琴、电钢琴 |
| 加法合成 | 丰满谐波 | 风琴、弦乐 |
| 减法合成 | 滤波塑形 | 贝斯、合成器 lead |
| 铺底音色 | 宽广延音 | 氛围音垫 |

**音色参数控制**：
- **ADSR 包络**：Attack（起音）、Decay（衰减）、Sustain（延持）、Release（释音）
- **滤波器**：低通/高通/带通滤波，频率可调
- **动态调制**：可根据力度和包络实时调制滤波参数

### 项目管理与导出
- **项目文件**：支持新建、保存和加载项目（JSON 格式）
- **音频导出**：离线渲染并导出为 WAV 格式音频文件
- **撤销/重做**：支持操作历史的撤销（重做功能开发中）

---

## 安装与运行

### 版本选择指南

| 版本 | 适用场景 | 系统要求 |
|-----|---------|---------|
| **v4 (Electron)** | 跨平台浏览器兼容、快速原型演示 | Windows/macOS/Linux，需 Node.js |
| **v5 (Qt/C++)** | 追求性能和原生体验 | 需要 C++ 编译环境 |

---

### v4 Electron 版本

#### 系统要求
- **操作系统**：Windows 10+、macOS 10.14+、Linux（Ubuntu 18.04+）
- **运行环境**：Node.js 16.x 或更高版本
- **内存**：建议 4GB 以上

#### 安装步骤

**1. 克隆项目**
```bash
git clone <repository-url>
cd <project-directory>
```

**2. 安装依赖**
```bash
npm install
```
这将自动安装 Electron 及相关依赖包。

**3. 启动开发模式**
```bash
npm start
```

**4. 构建可执行文件**

Windows：
```bash
npm run build:win
```

macOS：
```bash
npm run build:mac
```

Linux：
```bash
npm run build:linux
```

构建完成后，可执行文件位于 `dist/` 目录下。

#### 浏览器直接运行
Electron 版本也可以直接在浏览器中打开：
```bash
# 直接双击 index.html 文件
# 或使用本地服务器
npx serve .
```

---

### v5 Qt/C++ 原生版本

#### 系统要求
- **操作系统**：Windows 10+、macOS 11+、Linux（Ubuntu 20.04+）
- **编译器**：GCC 11+、Clang 14+、MSVC 2022+
- **构建工具**：CMake 3.16+
- **运行时**：Qt6（Core、Widgets、Multimedia 模块）

#### 安装依赖

**Ubuntu / Debian**
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-multimedia-dev libgl1-mesa-dev
```

**Fedora / RHEL**
```bash
sudo dnf install cmake gcc-c++ qt6-qtbase qt6-qtmultimedia mesa-libGL
```

**macOS（使用 Homebrew）**
```bash
brew install cmake qt@6
```

**Windows（使用 vcpkg）**
```bash
vcpkg install qtbase:x64-windows qtmultimedia:x64-windows
```

#### 编译构建

```bash
# 进入 v5 目录
cd v5

# 创建构建目录
mkdir build && cd build

# 运行 CMake 配置
cmake ..

# 编译项目（使用多核加速）
cmake --build . -j$(nproc)
```

**macOS 特殊配置**
如果 Qt6 通过 Homebrew 安装，需要指定 Qt 路径：
```bash
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
```

#### 运行程序

```bash
# Linux/macOS
./FLStudioEditor

# Windows
./Debug/FLStudioEditor.exe
# 或
./Release/FLStudioEditor.exe
```

---

## 快捷键参考

### 全局快捷键（v4/v5 通用）

| 功能 | 快捷键 |
|------|--------|
| 播放/暂停 | Space |
| 停止 | Escape |
| 新建项目 | Ctrl+N |
| 打开项目 | Ctrl+O |
| 保存项目 | Ctrl+S |
| 导出音频 | Ctrl+E |
| 钢琴卷帘视图 | Ctrl+1 |
| 步进音序器视图 | Ctrl+2 |
| 全屏模式 | F11 |

### 钢琴卷帘操作（鼠标）

| 操作 | 动作 |
|------|------|
| 创建音符 | 左键点击空白位置 |
| 删除音符 | 右键点击音符 |
| 移动音符 | 拖拽音符中心区域 |
| 调整时长 | 拖拽音符左右边缘 |
| 缩放视图 | Ctrl+鼠标滚轮 |

---

## 技术架构

### v4 Electron 版本
- **前端框架**：原生 HTML5 + CSS3 + JavaScript
- **音频引擎**：Web Audio API
- **合成算法**：FM 合成、加法合成、减法合成
- **调度系统**：Lookahead 精确调度算法
- **桌面框架**：Electron

### v5 Qt/C++ 版本
- **编程语言**：C++17
- **图形框架**：Qt6 + QPainter
- **音频框架**：Qt Multimedia（QAudioSink）
- **构建系统**：CMake
- **代码结构**：模块化分层设计

```
v5/src/
├── core/          # 数据模型层（Note、Track、Project）
├── audio/         # 音频引擎层（合成器、调度器、采样器）
├── ui/            # 用户界面层（各编辑器组件）
└── utils/         # 工具类层（音频缓冲区）
```

---

## 版本对比

| 特性 | v4 (Electron) | v5 (Qt/C++) |
|------|--------------|-------------|
| 技术栈 | HTML/JS + Electron | C++17 + Qt6 |
| 运行方式 | Chromium 渲染引擎 | 原生系统窗口 |
| 音频引擎 | Web Audio API | Qt Multimedia |
| UI 渲染 | HTML Canvas | QPainter |
| 安装包大小 | ~100MB+ | ~20MB |
| 启动速度 | 3-5 秒 | < 1 秒 |
| 内存占用 | 200-500MB | 50-100MB |
| 跨平台性 | 优秀 | 优秀 |
| 系统集成度 | 中等 | 高 |

---

## 项目文件结构

```
/
├── index.html          # Electron 渲染进程主页面
├── main.js             # Electron 主进程入口
├── preload.js          # 预加载脚本（IPC 桥接）
├── package.json        # Node.js 项目配置
├── README.md           # 本文档
├── assets/             # 静态资源目录
│   └── icon.png        # 应用图标
└── v5/                 # Qt/C++ 原生版本
    ├── CMakeLists.txt  # CMake 构建配置
    ├── include/        # 头文件目录
    ├── src/            # 源代码目录
    └── build/          # 构建输出目录
```

---

## 常见问题

### Q: v4 版本无法启动，提示缺少模块
**A**: 运行 `npm install` 确保所有依赖正确安装。

### Q: v5 版本编译失败，提示找不到 Qt6
**A**: 确认 Qt6 已正确安装，并检查 CMake 的 Qt 路径配置。

### Q: 音频播放没有声音
**A**: 
- 检查系统音量设置
- v4 版本需允许浏览器播放音频
- v5 版本需正确配置音频输出设备

### Q: 如何导出高质量音频？
**A**: 在 v5 版本中使用"导出音频"功能，WAV 格式为无损 44.1kHz/16bit 立体声。

---

## 版本历史

- **v1.0**：基础功能实现，钢琴卷帘和步进音序器
- **v2.0**：升级音源系统，支持多种合成方式
- **v3.0**：多乐器管理，ADSR 包络控制
- **v4.0**：Electron 桌面应用打包，跨平台支持
- **v5.0**：Qt/C++ 原生重构，性能优化

---

## 许可证

MIT License

---

## 贡献指南

欢迎提交 Issue 和 Pull Request！

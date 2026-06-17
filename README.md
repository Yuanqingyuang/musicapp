# FL Studio Style Music Editor v4

一款类似 FL Studio 的音乐编辑软件，支持浏览器运行和 Electron 桌面应用。

## 功能特性

### 钢琴卷帘 (Piano Roll)
- 覆盖 3 个八度（C3–B5）
- 鼠标点击绘制音符，拖拽移动和调整长度
- 右键删除音符
- 播放时音符高亮跟随
- 网格吸附（1/4、1/8、1/16）

### 步进音序器 (Step Sequencer)
- 4 条乐器轨道（底鼓、军鼓、踩镲、旋律）
- 16 步进编辑
- 每条轨道独立音量/声像控制

### 混音器 (Mixer)
- 每条轨道音量推子、声像旋钮
- 静音/独奏按钮
- 主输出电平表

### 传输控制 (Transport)
- 播放、停止、循环控制
- BPM 调节（60–200）
- 时间显示

### 音源系统
- 多乐器支持，可添加/删除乐器
- FM 合成（钢琴音色）
- 加法合成（风琴音色）
- 减法合成（合成器音色）
- 铺底音色
- ADSR 包络控制
- 滤波器控制

### 项目管理
- 新建/保存/加载项目
- 导出 WAV 音频

## 使用方法

### 浏览器版本
直接双击打开 `index.html` 文件即可运行。

### Electron 桌面版本

1. 安装依赖：
```bash
npm install
```

2. 启动应用：
```bash
npm start
```

3. 构建可执行文件：
```bash
# Windows
npm run build:win

# macOS
npm run build:mac

# Linux
npm run build:linux
```

## 快捷键

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
| 放大 | Ctrl++ |
| 缩小 | Ctrl+- |
| BPM +5 | Ctrl+Up |
| BPM -5 | Ctrl+Down |

## 钢琴卷帘操作

- **左键点击空白处**：创建新音符
- **右键点击音符**：删除音符
- **拖拽音符中心**：移动音符位置
- **拖拽音符边缘**：调整音符长度

## 项目结构

```
/
├── index.html      # 主应用文件（浏览器版本）
├── main.js         # Electron 主进程
├── preload.js      # Electron 预加载脚本
├── package.json    # 项目配置
├── README.md       # 说明文档
└── assets/         # 资源文件
    └── icon.png    # 应用图标
```

## 技术实现

- **音频引擎**：Web Audio API
- **合成器**：FM、加法、减法合成
- **调度器**：基于 lookahead 的精确调度
- **界面**：原生 HTML/CSS/JavaScript
- **桌面应用**：Electron

## 版本历史

- **v1**: 基础功能实现
- **v2**: 升级音源系统，多种合成方式
- **v3**: 多乐器管理，自定义音色
- **v4**: Electron 桌面应用打包

## 许可证

MIT License
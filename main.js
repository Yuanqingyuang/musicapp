const { app, BrowserWindow, Menu, dialog, ipcMain } = require('electron');
const path = require('path');
const fs = require('fs');

let mainWindow;

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1400,
        height: 900,
        minWidth: 1024,
        minHeight: 768,
        backgroundColor: '#1a1a1a',
        titleBarStyle: 'hiddenInset',
        show: false,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: path.join(__dirname, 'preload.js')
        },
        icon: path.join(__dirname, 'assets', 'icon.png')
    });

    mainWindow.loadFile('index.html');

    // 窗口准备好后显示
    mainWindow.once('ready-to-show', () => {
        mainWindow.show();
    });

    // 创建菜单
    createMenu();

    mainWindow.on('closed', () => {
        mainWindow = null;
    });
}

function createMenu() {
    const template = [
        {
            label: '文件',
            submenu: [
                {
                    label: '新建项目',
                    accelerator: 'CmdOrCtrl+N',
                    click: () => mainWindow.webContents.send('menu-action', 'newProject')
                },
                {
                    label: '打开项目...',
                    accelerator: 'CmdOrCtrl+O',
                    click: () => openProject()
                },
                {
                    label: '保存项目',
                    accelerator: 'CmdOrCtrl+S',
                    click: () => mainWindow.webContents.send('menu-action', 'saveProject')
                },
                {
                    label: '另存为...',
                    accelerator: 'CmdOrCtrl+Shift+S',
                    click: () => saveProjectAs()
                },
                { type: 'separator' },
                {
                    label: '导出音频...',
                    accelerator: 'CmdOrCtrl+E',
                    click: () => mainWindow.webContents.send('menu-action', 'exportAudio')
                },
                { type: 'separator' },
                {
                    label: '退出',
                    accelerator: process.platform === 'darwin' ? 'Cmd+Q' : 'Alt+F4',
                    click: () => app.quit()
                }
            ]
        },
        {
            label: '编辑',
            submenu: [
                {
                    label: '撤销',
                    accelerator: 'CmdOrCtrl+Z',
                    role: 'undo'
                },
                {
                    label: '重做',
                    accelerator: 'CmdOrCtrl+Shift+Z',
                    role: 'redo'
                },
                { type: 'separator' },
                {
                    label: '剪切',
                    accelerator: 'CmdOrCtrl+X',
                    role: 'cut'
                },
                {
                    label: '复制',
                    accelerator: 'CmdOrCtrl+C',
                    role: 'copy'
                },
                {
                    label: '粘贴',
                    accelerator: 'CmdOrCtrl+V',
                    role: 'paste'
                },
                { type: 'separator' },
                {
                    label: '全选',
                    accelerator: 'CmdOrCtrl+A',
                    role: 'selectAll'
                }
            ]
        },
        {
            label: '视图',
            submenu: [
                {
                    label: '放大',
                    accelerator: 'CmdOrCtrl+Plus',
                    click: () => mainWindow.webContents.send('menu-action', 'zoomIn')
                },
                {
                    label: '缩小',
                    accelerator: 'CmdOrCtrl+-',
                    click: () => mainWindow.webContents.send('menu-action', 'zoomOut')
                },
                { type: 'separator' },
                {
                    label: '钢琴卷帘',
                    accelerator: 'CmdOrCtrl+1',
                    click: () => mainWindow.webContents.send('menu-action', 'switchPiano')
                },
                {
                    label: '步进音序器',
                    accelerator: 'CmdOrCtrl+2',
                    click: () => mainWindow.webContents.send('menu-action', 'switchStep')
                },
                { type: 'separator' },
                {
                    label: '切换开发者工具',
                    accelerator: 'F12',
                    click: () => mainWindow.webContents.toggleDevTools()
                },
                {
                    label: '全屏',
                    accelerator: 'F11',
                    click: () => mainWindow.setFullScreen(!mainWindow.isFullScreen())
                }
            ]
        },
        {
            label: '传输',
            submenu: [
                {
                    label: '播放/暂停',
                    accelerator: 'Space',
                    click: () => mainWindow.webContents.send('menu-action', 'togglePlay')
                },
                {
                    label: '停止',
                    accelerator: 'Escape',
                    click: () => mainWindow.webContents.send('menu-action', 'stop')
                },
                { type: 'separator' },
                {
                    label: 'BPM +5',
                    accelerator: 'CmdOrCtrl+Up',
                    click: () => mainWindow.webContents.send('menu-action', 'bpmUp')
                },
                {
                    label: 'BPM -5',
                    accelerator: 'CmdOrCtrl+Down',
                    click: () => mainWindow.webContents.send('menu-action', 'bpmDown')
                }
            ]
        },
        {
            label: '帮助',
            submenu: [
                {
                    label: '关于',
                    click: () => showAbout()
                },
                {
                    label: '快捷键',
                    click: () => showShortcuts()
                }
            ]
        }
    ];

    // macOS 特殊菜单
    if (process.platform === 'darwin') {
        template.unshift({
            label: app.getName(),
            submenu: [
                { label: '关于', click: () => showAbout() },
                { type: 'separator' },
                { label: '偏好设置...', accelerator: 'Cmd+,', click: () => {} },
                { type: 'separator' },
                { label: '隐藏', accelerator: 'Cmd+H', role: 'hide' },
                { label: '隐藏其他', accelerator: 'Cmd+Alt+H', role: 'hideOthers' },
                { label: '显示全部', role: 'unhide' },
                { type: 'separator' },
                { label: '退出', accelerator: 'Cmd+Q', click: () => app.quit() }
            ]
        });
    }

    const menu = Menu.buildFromTemplate(template);
    Menu.setApplicationMenu(menu);
}

// 打开项目文件
async function openProject() {
    const result = await dialog.showOpenDialog(mainWindow, {
        title: '打开项目',
        filters: [
            { name: '音乐项目', extensions: ['flproj', 'json'] },
            { name: '所有文件', extensions: ['*'] }
        ],
        properties: ['openFile']
    });

    if (!result.canceled && result.filePaths.length > 0) {
        const filePath = result.filePaths[0];
        try {
            const data = fs.readFileSync(filePath, 'utf-8');
            const project = JSON.parse(data);
            mainWindow.webContents.send('load-project', project, filePath);
        } catch (error) {
            dialog.showErrorBox('打开失败', `无法打开项目文件：${error.message}`);
        }
    }
}

// 另存为
async function saveProjectAs() {
    const result = await dialog.showSaveDialog(mainWindow, {
        title: '保存项目',
        filters: [
            { name: '音乐项目', extensions: ['flproj'] },
            { name: 'JSON 文件', extensions: ['json'] }
        ],
        defaultPath: '未命名项目.flproj'
    });

    if (!result.canceled && result.filePath) {
        mainWindow.webContents.send('get-project-data', result.filePath);
    }
}

// 显示关于对话框
function showAbout() {
    dialog.showMessageBox(mainWindow, {
        type: 'info',
        title: '关于 FL Studio Editor',
        message: 'FL Studio Style Music Editor v4.0',
        detail: '一款类似 FL Studio 的音乐编辑软件\n\n功能特性：\n• 钢琴卷帘编辑器\n• 步进音序器\n• 多乐器音源支持\n• FM/加法/减法合成\n• 混音器面板\n• WAV 导出\n\n© 2024 Music Editor Team'
    });
}

// 显示快捷键
function showShortcuts() {
    dialog.showMessageBox(mainWindow, {
        type: 'info',
        title: '快捷键',
        message: '键盘快捷键',
        detail: `播放/暂停: Space
停止: Escape
新建项目: Ctrl+N
打开项目: Ctrl+O
保存项目: Ctrl+S
导出音频: Ctrl+E

视图切换:
  钢琴卷帘: Ctrl+1
  步进音序器: Ctrl+2

缩放:
  放大: Ctrl++
  缩小: Ctrl+-

BPM 调节:
  增加: Ctrl+Up
  减少: Ctrl+Down

钢琴卷帘:
  左键点击: 创建音符
  右键点击: 删除音符
  拖拽: 移动/调整音符`
    });
}

// IPC 处理
ipcMain.handle('save-project-file', async (event, filePath, data) => {
    try {
        fs.writeFileSync(filePath, JSON.stringify(data, null, 2));
        return { success: true };
    } catch (error) {
        return { success: false, error: error.message };
    }
});

ipcMain.handle('show-save-dialog', async (event, options) => {
    return await dialog.showSaveDialog(mainWindow, options);
});

ipcMain.handle('show-open-dialog', async (event, options) => {
    return await dialog.showOpenDialog(mainWindow, options);
});

ipcMain.handle('get-app-path', () => {
    return app.getPath('userData');
});

// 应用生命周期
app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
        createWindow();
    }
});

// 处理未捕获的异常
process.on('uncaughtException', (error) => {
    console.error('Uncaught Exception:', error);
    dialog.showErrorBox('错误', `发生未知错误：${error.message}`);
});
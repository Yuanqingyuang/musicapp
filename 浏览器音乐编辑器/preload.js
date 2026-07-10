const { contextBridge, ipcRenderer } = require('electron');

// 暴露安全的 API 给渲染进程
contextBridge.exposeInMainWorld('electronAPI', {
    // 菜单操作回调
    onMenuAction: (callback) => {
        ipcRenderer.on('menu-action', (event, action) => callback(action));
    },

    // 项目加载回调
    onLoadProject: (callback) => {
        ipcRenderer.on('load-project', (event, project, filePath) => callback(project, filePath));
    },

    // 获取项目数据请求
    onGetProjectData: (callback) => {
        ipcRenderer.on('get-project-data', (event, filePath) => callback(filePath));
    },

    // 保存项目文件
    saveProjectFile: (filePath, data) => {
        return ipcRenderer.invoke('save-project-file', filePath, data);
    },

    // 显示保存对话框
    showSaveDialog: (options) => {
        return ipcRenderer.invoke('show-save-dialog', options);
    },

    // 显示打开对话框
    showOpenDialog: (options) => {
        return ipcRenderer.invoke('show-open-dialog', options);
    },

    // 获取应用数据路径
    getAppPath: () => {
        return ipcRenderer.invoke('get-app-path');
    },

    // 移除监听器
    removeAllListeners: (channel) => {
        ipcRenderer.removeAllListeners(channel);
    }
});
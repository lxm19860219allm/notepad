# NotepadApp - 标签式记事本应用程序

![Qt](https://img.shields.io/badge/Qt-6.6.2-green.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

一个基于Qt6和C++17开发的现代化标签式记事本应用程序，支持多标签编辑、自动保存和JSON文件格式。

## ✨ 功能特性

- 🏷️ **多标签支持** - 同时编辑多个文档
- 💾 **自动保存** - 每30秒自动保存内容
- 📁 **JSON格式** - 使用JSON保存数据，易于阅读和备份
- 🎨 **现代化界面** - 基于Qt6的现代化用户界面
- ✏️ **标签编辑** - 支持重命名、复制、删除标签
- 🔍 **内容搜索** - 快速查找文本内容
- 💻 **跨平台** - 支持Windows系统

## 🖼️ 界面预览

![主界面](resources/screenshots/main_window.png)

## 📥 下载安装

### Windows 用户
1. 前往 [发布](https://github.com/lxm19860219allm/notepad/releases) 页面
2. 下载最新版本的 `NotepadApp_v1.0.0_Windows.zip`
3. 解压到任意目录
4. 运行 `NotepadApp.exe`

### 系统要求
- Windows 7/8/10/11
- 无需安装，绿色软件
- 约50MB可用空间

## 🛠️ 从源码构建

### 前提条件
- Qt 6.6.0 或更高版本
- CMake 3.16 或更高版本
- MinGW 或 Visual Studio 2019+

### 构建步骤
```bash
# 克隆仓库
git clone https://github.com/lxm19860219allm/notepad.git
cd notepad

# 使用脚本构建（Windows）
scripts\build_and_deploy.bat

# 或手动构建
mkdir build
cd build
cmake ..
cmake --build .
🎯 使用方法
新建标签: 点击"添加标签"按钮或使用右键菜单

编辑内容: 在右侧文本区域输入内容

重命名标签: 双击标签名称或使用F2快捷键

保存文件: 使用文件菜单保存为JSON格式

自动保存: 程序每30秒自动保存到当前文件
快捷键
Ctrl+N - 新建文件

Ctrl+O - 打开文件

Ctrl+S - 保存文件

Ctrl+Shift+S - 另存为

F2 - 重命名标签

Ctrl+D - 复制标签

Delete - 删除标签
🏗️ 项目结构
notepad/
├── src/                    # 源代码
│   ├── main.cpp           # 程序入口
│   ├── mainwindow.h/cpp   # 主窗口类
│   └── notepaditem.h/cpp  # 数据模型
├── scripts/               # 构建脚本
│   └── build_and_deploy.bat
├── resources/             # 资源文件
├── docs/                  # 文档
└── CMakeLists.txt         # 构建配置
🔧 技术栈
框架: Qt 6.6.2

语言: C++17

构建系统: CMake

数据格式: JSON

🤝 贡献指南
欢迎提交 Issue 和 Pull Request！

Fork 本仓库

创建特性分支 (git checkout -b feature/AmazingFeature)

提交更改 (git commit -m 'Add some AmazingFeature')

推送到分支 (git push origin feature/AmazingFeature)

开启 Pull Request

📄 许可证
本项目采用 MIT 许可证 - 查看 LICENSE 文件了解详情。

📞 联系信息
作者: lxm19860219allm

项目地址: https://github.com/lxm19860219allm/notepad
🙏 致谢
感谢 Qt 框架和开源社区的支持！

### 添加详细的文档

**docs/DEVELOPMENT.md**
```markdown
# 开发指南

## 环境配置

### 必需工具
- Qt 6.6.2+
- CMake 3.16+
- C++17兼容编译器

### 推荐开发环境
- IDE: Qt Creator 或 Visual Studio
- 编译器: MinGW 或 MSVC

## 代码结构

### 核心类说明

#### NotepadItem
- 数据模型类
- 负责单个标签的数据管理
- 实现JSON序列化

#### MainWindow
- 主窗口类
- 处理用户界面和交互
- 管理多个NotepadItem实例

## 构建说明

### 调试版本
```bash
mkdir build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
发布版本
mkdir build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

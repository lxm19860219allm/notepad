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


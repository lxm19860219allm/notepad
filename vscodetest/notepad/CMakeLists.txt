cmake_minimum_required(VERSION 3.16)
project(NotepadApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 查找Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# 包含源文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/notepaditem.cpp
)

set(HEADERS
    src/mainwindow.h
    src/notepaditem.h
)

# 创建可执行文件
add_executable(NotepadApp ${SOURCES} ${HEADERS})

# 链接Qt库 - 使用PRIVATE关键字
target_link_libraries(NotepadApp PRIVATE Qt6::Core Qt6::Widgets)
cmake_minimum_required(VERSION 3.20)
project(tooi)

# 启用 compile_commands.json 生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++23 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加可执行文件
add_executable(tooi 
    src/main.cpp
    src/core/interpreter.cpp
    src/core/scanner.cpp
    src/cli/args_parser.cpp
    src/cli/repl.cpp
    src/cli/run_from_file.cpp
)

# 指定 include 目录
target_include_directories(tooi PUBLIC include)
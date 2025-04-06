cmake_minimum_required(VERSION 3.20)
project(tooi)

# 启用 compile_commands.json 生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++23 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Linenoise Dependency ---
# Define linenoise as a library using its source file
add_library(linenoise STATIC
    third-party/linenoise/linenoise.c
)

# Tell CMake where to find linenoise.h when compiling linenoise OR your code
target_include_directories(linenoise PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/linenoise
)

# --- Your Main Executable --- 
add_executable(tooi
    src/main.cpp
    src/core/interpreter.cpp
    src/core/scanner.cpp
    src/core/error_reporter.cpp
    src/cli/args_parser.cpp
    src/cli/repl.cpp
    src/cli/run_from_file.cpp
)

# Specify include directories for *your* project AFTER defining the target
target_include_directories(tooi PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link tooi against linenoise
target_link_libraries(tooi PRIVATE linenoise)

# Ensure your executable also knows about its own include dir (if not covered by INTERFACE above)
# Usually redundant if INTERFACE is used on target_include_directories for tooi
# target_include_directories(tooi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
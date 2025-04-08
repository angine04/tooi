cmake_minimum_required(VERSION 3.20)
project(tooi)

# 启用 compile_commands.json 生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++23 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Build Options ---
option(TOOI_ENABLE_TESTS "Enable building tests within the main binary" OFF)

# --- Find Packages ---
find_package(fmt REQUIRED) # Find the fmt library installed via Brew

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
    src/core/error_registry.cpp
    src/cli/args_parser.cpp
    src/cli/repl.cpp
    src/cli/run_from_file.cpp
)

# --- Test Sources (Conditional) ---
if(TOOI_ENABLE_TESTS)
    message(STATUS "Building with tests enabled.")
    # Find all test source files
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
    # Add test sources to the main executable
    target_sources(tooi PRIVATE ${TEST_SOURCES})
    # Add test directory to includes for the main executable
    target_include_directories(tooi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    # Define a preprocessor macro to indicate tests are enabled
    target_compile_definitions(tooi PRIVATE TOOI_TESTS_ENABLED)
else()
    message(STATUS "Building without tests.")
endif()

# Specify include directories for *your* project AFTER defining the target
target_include_directories(tooi PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # The fmt::fmt target automatically adds its include directory when linked,
    # but explicitly adding might resolve issues in some setups, though often redundant.
    # $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES> # Alternative way
)

# Link tooi against dependencies
target_link_libraries(tooi PRIVATE
    linenoise
    fmt::fmt # Linking fmt::fmt SHOULD also provide include directories via its INTERFACE properties
)

# Ensure your executable also knows about its own include dir (if not covered by INTERFACE above)
# Usually redundant if INTERFACE is used on target_include_directories for tooi
# target_include_directories(tooi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
<p align="center">
  <img src="docs/assets/typeface.png" alt="Tooi Logo" width="200">
</p>

<h1 align="center">Tooi</h1>

## 简介

Tooi 是一门脚本语言。它支持通过交互式 REPL 环境或执行脚本文件来运行代码。

## 项目状态

本项目目前处于开发阶段。

- ✅ Tooi 最小语法规范
- ✅ 命令行参数解析
- ✅ 错误处理系统
- ✅ 词法分析器 (Scanner)
- ✅ 测试框架集成
- ✅ REPL 环境 (使用 linenoise)
- 🚧 Tooi 完整语法规范
- 🚧 语法分析器 (Parser)
- ❌ 语义分析器 (Semantic Analyzer)
- ❌ 解释器 (Interpreter)
- ❌ 标准库

## 依赖项

构建 Tooi 需要以下依赖：

1. **CMake:** 版本 3.20 或更高。
2. **C++ 编译器:** 支持 C++23 标准 (例如 GCC 13+, Clang 16+)。
3. **{fmt} 库:** 用于格式化输出。可以通过包管理器安装 (例如在 macOS/Linux 上使用 `brew install fmt` 或 `apt install libfmt-dev`)。

*注意: 行编辑库 `linenoise` 已包含在 `third-party` 目录下，无需单独安装。*

## 快速开始

### 构建

1. **确保依赖项已安装:**
    - CMake (>= 3.20)
    - 支持 C++23 的编译器
    - {fmt} 库 (例如 `brew install fmt` 或 `apt install libfmt-dev`)

2. **配置并构建项目:**

    ```bash
    # 创建构建目录
    cmake -B build
    
    # 编译项目
    cmake --build build
    ```

### 使用方法

构建成功后，可以在 `build` 目录下找到 `tooi` 可执行文件。

```bash
# 启动交互式 REPL 环境
./build/tooi

# 执行脚本文件
./build/tooi examples/hello.tooi

# 显示帮助信息
./build/tooi --help

# 显示版本信息
./build/tooi --version
```

## 测试

本项目使用 [Catch2](https://github.com/catchorg/Catch2) (v3 单头文件版本) 作为单元测试框架，该文件包含在 `tests/catch2.hpp` 中。

1. **启用测试构建:**
    在配置 CMake 时添加 `-DTOOI_ENABLE_TESTS=ON` 选项：

    ```bash
    # 配置并启用测试
    cmake -B build -DTOOI_ENABLE_TESTS=ON
    ```

2. **构建项目:**

    ```bash
    # 编译（包含测试代码）
    cmake --build build
    ```

3. **运行测试:**
    使用 `--run-tests` 参数执行编译后的二进制文件：

    ```bash
    # 运行所有测试
    ./build/tooi --run-tests

    # 运行特定标签的测试 (例如，只运行 scanner 测试)
    ./build/tooi --run-tests [scanner]

    # 将测试结果输出到日志文件
    ./build/tooi --run-tests --out test_results.log
    ```

## 许可证

使用 [GPL 许可证](COPYING)。

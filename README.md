<p align="center">
  <img src="docs/assets/typeface.png" alt="Tooi Logo" width="200">
</p>

<h1 align="center">Tooi</h1>

## 简介

Tooi 是一门脚本语言。它支持通过交互式 REPL 环境或执行脚本文件来运行代码。

## 依赖项

构建 Tooi 需要以下依赖：

1. **CMake:** 版本 3.20 或更高。
2. **C++ 编译器:** 支持 C++23 标准 (例如 GCC 13+, Clang 16+)。
3. **{fmt} 库:** 用于格式化输出。可以通过包管理器安装 (例如在 macOS/Linux 上使用 `brew install fmt` 或 `apt install libfmt-dev`)。

*注意: 行编辑库 `linenoise` 已包含在 `third-party` 目录下，无需单独安装。*

## 快速开始

> 目前啥都没实现

### 构建

```bash
mkdir build
cd build && cmake ..
make
```

### 使用方法

```bash
# 启动 REPL 环境
./tooi

# 执行脚本文件
./tooi path/to/script.tooi

# 显示帮助信息
./tooi --help

# 显示版本信息
./tooi --version
```

## 测试

本项目使用 [Catch2](https://github.com/catchorg/Catch2) 作为单元测试框架。

要启用测试功能，请在 CMake 配置时使用 `DTOOI_ENABLE_TESTS` 选项：

```bash
cmake -B build -DTOOI_ENABLE_TESTS=ON
```

然后，按常规方式构建项目：

```bash
cmake --build build
```

要运行测试，使用 `--run-tests` 参数执行编译后的二进制文件：

```bash
./build/tooi --run-tests
```

## 许可证

使用 [GPL 许可证](COPYING)。

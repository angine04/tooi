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
./build/tooi

# 执行脚本文件
./build/tooi path/to/script.tooi

# 显示帮助信息
./build/tooi --help

# 显示版本信息
./build/tooi --version
```

## 许可证

使用 [GPL 许可证](COPYING)。

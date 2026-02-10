# BinMerger 编译指南

## 项目概述
BinMerger 是一个 Windows GUI 应用程序，用于合并二进制文件。该项目包含一个Dev Container配置，可以在Linux环境中直接交叉编译为Windows可执行文件。

## Dev Container 环境

### 已安装的工具
- **编译器**: MinGW-w64 (x86_64-w64-mingw32-gcc/g++)
- **构建工具**: GNU Make
- **开发工具**: gcc, g++, build-essential

### 编译项目

在Dev Container中运行以下命令编译项目：

```bash
make -f Makefile.linux
```

### 编译选项

其他可用命令：

```bash
# 清理旧的编译文件
make -f Makefile.linux clean

# 显示帮助信息
make -f Makefile.linux help

# 重新编译
make -f Makefile.linux clean && make -f Makefile.linux
```

### 输出文件

编译后的可执行文件位于：
```
bin/Release/BinMerger.exe
```

文件信息：
- 类型: PE32+ Windows GUI 可执行文件
- 架构: x86-64
- 大小: ~6KB (已优化)

## 编译配置

### Makefile.linux 特性

- **交叉编译**: 使用MinGW-w64从Linux编译Windows程序
- **字符编码**: 支持GBK和UTF-8混合编码
- **优化级别**: -O2 (Release优化)
- **目标子系统**: Windows GUI应用
- **链接库**: gdi32, user32, comdlg32 (Windows核心库)

### 编译标志

```
-Wall              : 显示所有警告
-O2                : 优化级别2
-finput-charset=UTF-8  : 输入文件字符集
-fexec-charset=GBK     : 输出GBK编码
-municode          : 使用Unicode标准
-DUNICODE          : 定义UNICODE宏
-D_UNICODE         : 定义_UNICODE宏
-mwindows          : Windows GUI子系统
```

## 源文件

- `main.cpp`: 主程序（132行）
  - Windows GUI界面实现
  - 二进制文件合并逻辑
  - UTF-8和中文支持

## 项目结构

```
binmerge/
├── main.cpp                 # 源代码
├── BinMerger.cbp           # Code::Blocks项目文件
├── Makefile.linux          # Linux交叉编译Makefile
├── .devcontainer/
│   └── devcontainer.json   # Dev Container配置
└── bin/Release/
    └── BinMerger.exe       # 编译输出
```

## 故障排除

### 编译失败
1. 确保已进入Dev Container环境
2. 检查MinGW工具是否正确安装: `which x86_64-w64-mingw32-gcc`
3. 清理旧文件后重新编译: `make -f Makefile.linux clean && make -f Makefile.linux`

### 字符编码问题
如果输出文件中的中文显示不正确，编译选项中已包含UTF-8和GBK转换配置，应该会自动处理。

## 在Windows上运行

编译生成的 `.exe` 文件可以直接在Windows (x86-64) 上运行。

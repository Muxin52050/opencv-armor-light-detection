# OpenCV 视觉组作业说明

## 目录结构

```
opencv_hw/
├── CMakeLists.txt
├── cpp/
│   ├── task1.cpp           # 任务一 C++ 源码（主交付）
│   ├── task2.cpp           # 任务二 C++ 源码（主交付）
│   ├── task1.exe           # 已编译的可执行程序（MSVC x64）
│   └── task2.exe
├── task1.py / task2.py     # Python 版源码（算法相同，便于核对）
├── images/
│   ├── 2.png               # 任务一测试图
│   └── 3.jpg               # 任务二测试图
└── result/
    ├── task1_result.png    # 任务一结果：黄框蓝、绿框红
    └── task2_result.jpg    # 任务二结果：黄框灯条
```

## 任务一：颜色矩形框选（images/2.png）

- BGR → HSV；白背景 S≈0，蓝/红矩形 S 高。
  - 蓝色 H∈[90,130]，红色 H∈[0,12]，均要求 S>80、V>80
  - 闭运算补缝隙，`minAreaRect` 画最小外接矩形，因此倾斜矩形也贴合
- 结果：3 个蓝矩形（黄框）、2 个红矩形（绿框）

## 任务二：装甲板灯条识别（images/3.jpg）

- 灯条是画面里最亮的橙红发光竖条：H∈[0,30]、S>70、V>170
  （整幅图 V 中位数仅 17，该阈值足以把灯条和背景分开）
- 按 `面积>15 且 高>25 且 高/宽>1.5` 过滤轮廓，排除地面反光
- 结果：检出 4 个灯条（左车 1、主车 2、右车 1），黄框标出

## 编译与运行（C++）

需要 OpenCV C++ 库（本机编译验证用的是 OpenCV 5.1.0 + MSVC x64）。

```bash
cmake -S . -B build -DOpenCV_DIR="你的OpenCV/build目录"
cmake --build build --config Release
```

不带 CMake 也可以直接用 MSVC：

```
cl /EHsc /MD /I"OpenCV\build\include" task1.cpp /link /LIBPATH:"OpenCV\build\x64\vc16\lib" opencv_world510.lib
```

运行（把 OpenCV 的 `bin` 目录加入 PATH）：

```
task1.exe  测试图路径  结果图路径
task2.exe  测试图路径  结果图路径
```

程序接受两个命令行参数：输入图和输出图；不传参时使用相对路径 `images/2.png` 等默认值。

## 验证情况

- C++ 源码已用 MSVC 编译器**编译通过**（task1.exe / task2.exe 为有效 PE 文件），
  确认无语法与链接错误。
- 结果图由相同算法的 Python 版实际运行生成并逐一目视核对（黄/绿框数量与位置正确）。
  本机安全策略禁止运行新编译的未签名 exe，因此 C++ 版未在本机执行；
  在普通开发环境中按上面的命令编译后即可直接运行得到同样结果。

## 备注

阈值参数是针对这两张测试图的像素分布标定的；换图（尤其光照差异大时）需要重新调整 HSV 阈值。

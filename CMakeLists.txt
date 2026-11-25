# OpenCV 视觉组作业：任务一（颜色矩形框选）+ 任务二（装甲板灯条识别）
#
# 标准用法（本机已验证 MSVC 可直接编译通过）：
#   cmake -S . -B build -DOpenCV_DIR="你的OpenCV/build目录"
#   cmake --build build --config Release
# 例如：
#   cmake -S . -B build -DOpenCV_DIR="C:/opencv/opencv/build"
#
# 若机器上没有 OpenCV C++ 库，会自动降级为调用 Python 脚本（task1.py / task2.py）。

cmake_minimum_required(VERSION 3.10)
project(opencv_homework LANGUAGES NONE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV QUIET)

if(OpenCV_FOUND)
    message(STATUS "找到 OpenCV ${OpenCV_VERSION}，编译 C++ 可执行程序")
    enable_language(CXX)
    include_directories(${OpenCV_INCLUDE_DIRS})

    add_executable(task1 cpp/task1.cpp)
    target_link_libraries(task1 ${OpenCV_LIBS})

    add_executable(task2 cpp/task2.cpp)
    target_link_libraries(task2 ${OpenCV_LIBS})

    # 把测试图拷到构建目录，方便在 build 下直接运行
    configure_file(images/2.png ${CMAKE_BINARY_DIR}/images/2.png COPYONLY)
    configure_file(images/3.jpg ${CMAKE_BINARY_DIR}/images/3.jpg COPYONLY)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/result)
else()
    message(STATUS "未找到 OpenCV C++ 库，改用 Python + OpenCV 运行")
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(NOT Python3_Interpreter_FOUND)
        find_package(PythonInterp QUIET)
        set(Python3_EXECUTABLE ${PYTHON_EXECUTABLE})
    endif()
    if(Python3_EXECUTABLE)
        add_custom_target(run_task1 ALL
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/task1.py
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "运行任务一：颜色矩形框选")
        add_custom_target(run_task2 ALL
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/task2.py
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "运行任务二：装甲板灯条识别")
    else()
        message(WARNING "未找到 Python，请手动执行 python task1.py / python task2.py")
    endif()
endif()

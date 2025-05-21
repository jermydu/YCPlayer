# CMakeList.txt: YCAV 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
#编译器启用 UTF-8 编码支持
add_compile_options("/utf-8")

# include
include_directories(${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/include)
include_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/include)

# lib
link_directories(${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/lib)
link_libraries("avcodec.lib")
link_libraries("avdevice.lib")
link_libraries("avfilter.lib")
link_libraries("avformat.lib")
link_libraries("avutil.lib")
link_libraries("postproc.lib")
link_libraries("swresample.lib")
link_libraries("swscale.lib")

# 设置 Debug 和 Release 模式的静态库路径
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    link_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/lib/Debug)
    link_libraries("spdlogd.lib") 
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    link_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/lib/Release)
    link_libraries("spdlog.lib") 
else()
    message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif()

# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/YCAV/*.cpp)
add_library (YCAV ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCAV PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

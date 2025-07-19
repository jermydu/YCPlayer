# CMakeList.txt: YCPlayer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

#编译器启用 UTF-8 编码支持
add_compile_options("/utf-8")

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/YCPlayer/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/YCAV)
include_directories(${PROJECT_SOURCE_DIR}/YCLogger/include)

link_directories(${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/lib)
link_directories(${PROJECT_SOURCE_DIR}/lib/YCLogger)

# 将源代码添加到此项目的可执行文件。
add_executable (YCPlayer ${SRC})
target_link_libraries(YCPlayer PRIVATE
    avcodec
    avdevice
    avfilter
    avformat
    avutil
    postproc
    swresample
    swscale
    YCAV
    YCLogger
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCPlayer PROPERTY CXX_STANDARD 20)
endif()


# TODO: 如有需要，请添加测试并安装目标。

# CMakeList.txt: YCAV 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
#编译器启用 UTF-8 编码支持
add_compile_options("/utf-8")

# include
include_directories(${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/include)
include_directories(${PROJECT_SOURCE_DIR}/YCLogger/include)

# link
link_directories(${PROJECT_SOURCE_DIR}/lib/YCLogger)
link_directories(${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/lib)

# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/YCAV/*.cpp)
add_library (YCAV SHARED ${SRC})

#导出所有符号
set_target_properties(YCAV PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_link_libraries(YCAV PRIVATE
    avcodec
    avdevice
    avfilter
    avformat
    avutil
    postproc
    swresample
    swscale
    YCLogger
)

 install(TARGETS YCAV
       ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib/YCAV)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCAV PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

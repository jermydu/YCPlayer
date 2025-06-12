# CMakeList.txt: YCPlayer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

#编译器启用 UTF-8 编码支持
add_compile_options("/utf-8")

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/YCPlayer/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/YCAV)


# 将源代码添加到此项目的可执行文件。
add_executable (YCPlayer ${SRC})

# 设置 Debug 和 Release 模式的静态库路径
if(CMAKE_CONFIGURATION_TYPES)
    # 多配置生成器，如 Visual Studio
    foreach(build_type IN LISTS CMAKE_CONFIGURATION_TYPES)
        if(build_type STREQUAL "Debug")
            # 为 Debug 配置设置库路径和链接库
            target_link_directories(YCPlayer PRIVATE
                ${PROJECT_SOURCE_DIR}/dependents/spdlog/lib/Debug
                ${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/lib
            )
            target_link_libraries(YCPlayer PRIVATE
            spdlogd.lib
            avcodec.lib
            avdevice.lib
            avfilter.lib
            avformat.lib
            avutil.lib
            postproc.lib
            swresample.lib
            swscale.lib
            YCAV
            )
        elseif(build_type STREQUAL "Release")
            # 为 Release 配置设置库路径和链接库
            target_link_directories(YCPlayer PRIVATE
                ${PROJECT_SOURCE_DIR}/dependents/spdlog/lib/Release
                ${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/lib
            )
            target_link_libraries(YCPlayer PRIVATE
            spdlog.lib
            avcodec.lib
            avdevice.lib
            avfilter.lib
            avformat.lib
            avutil.lib
            postproc.lib
            swresample.lib
            swscale.lib
            YCAV
        ) 
        endif()
    endforeach()
else()
    # 单配置生成器
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        link_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/lib/Debug)
        link_libraries("spdlogd.lib") 
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        link_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/lib/Release)
        link_libraries("spdlog.lib") 
    else()
        message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()


link_directories(${PROJECT_SOURCE_DIR}/dependents/ffmpeg-6.0-full_build-shared/lib)

link_libraries("avcodec.lib")
link_libraries("avdevice.lib")
link_libraries("avfilter.lib")
link_libraries("avformat.lib")
link_libraries("avutil.lib")
link_libraries("postproc.lib")
link_libraries("swresample.lib")
link_libraries("swscale.lib")

target_link_libraries(YCPlayer YCAV) 

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCPlayer PROPERTY CXX_STANDARD 20)
endif()


# TODO: 如有需要，请添加测试并安装目标。

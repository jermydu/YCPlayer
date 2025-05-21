# CMakeList.txt: YCPlayer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

#编译器启用 UTF-8 编码支持
add_compile_options("/utf-8")

# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/YCPlayer/*.cpp)
add_executable (YCPlayer ${SRC})

include_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/include)

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



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCPlayer PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

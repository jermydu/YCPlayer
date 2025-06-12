add_compile_options("/utf-8")

# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/YCThread/*.cpp)
add_library (YCThread ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCThread PROPERTY CXX_STANDARD 20)
endif()

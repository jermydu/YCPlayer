cmake_minimum_required (VERSION 3.8)

add_compile_options(/utf-8)

include_directories(./include)
include_directories(${PROJECT_SOURCE_DIR}/dependents/spdlog/include)

add_library(YCLogger SHARED src/YCLogger.cpp)

#导出所有符号
set_target_properties(YCLogger PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

install(TARGETS YCLogger DESTINATION ${PROJECT_SOURCE_DIR}/lib/YCLogger)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET YCLogger PROPERTY CXX_STANDARD 20)
endif()

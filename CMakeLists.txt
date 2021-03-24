#指定 cmake 的最小版本
cmake_minimum_required(VERSION 2.8.8)
#项目名称/工程名
project(detector)
# generating build/compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#设置c++编译器
# Set C++14 as standard for the whole project
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(DETECTOR_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${DETECTOR_INCLUDE_DIR})

# 将源码目录保存到变量中
aux_source_directory(./src SRCS)  # 搜索当前目录下的所有.cpp文件
aux_source_directory(./ SRCS)  # 搜索当前目录下的所有.cpp文件

add_library(detector SHARED ${SRCS})

# Associate header files
set_target_properties(detector PROPERTIES VERSION 1.0 SOVERSION 1)

target_link_libraries(detector tensorflow yaml-cpp)

# make install
install(TARGETS detector
    LIBRARY DESTINATION lib ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.h")

#添加要编译的可执行文件
add_executable(main ${SRCS})    ## 生成可执行文件
#设置 target 需要链接的库
target_link_libraries(main detector)

# using the following command to install benchmark library
# apt install libbenchmark-dev
add_subdirectory(bench)
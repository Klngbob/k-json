#CMake 最低版本号要求
cmake_minimum_required ( VERSION 2.8 )

#项目信息
project (K-json)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -03 -Wall")

#查找test/目录下的所有源文件
#并将名称保存在 DIR_SRCS 变量
aux_source_directory(test/ DIR_SRCS)

#添加 source 子目录
add_subdirectory(source)

add_executable(json-test ${DIR_SRCS})

#添加链接库
target_link_libraries(json-test JsonFunctions)
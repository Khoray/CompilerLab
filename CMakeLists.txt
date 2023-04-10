cmake_minimum_required(VERSION 3.10)
project(compiler)

# in our docker env, this can avoid cross compile problemes, if this is ur own linux or macos, should delete this
if (UNIX) 
    message("in docker env")
    set(CMAKE_C_COMPILER    "/usr/bin/x86_64-linux-gnu-gcc-7")
    set(CMAKE_CXX_COMPILER  "/usr/bin/x86_64-linux-gnu-g++-7")
endif()


# compile flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告
# debug flags
# add_definitions(-DDEBUG_DFA)
# add_definitions(-DDEBUG_SCANNER)
# add_definitions(-DDEBUG_PARSER)

# output settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


# include
include_directories(./include)

# third party libs
add_library(jsoncpp ./src/third_party/jsoncpp/jsoncpp.cpp)

# build library
# link libIR.a
# link_directories(./lib)   FIXME 在docker下重新构建该库
# every src file directory should be compile into a lib
aux_source_directory(./src/front FRONT_SRC)
add_library(Front ${FRONT_SRC})

# executable
add_executable(compiler main.cpp)


# link
# every lib should be linked with [compiler]
target_link_libraries(compiler Front jsoncpp)
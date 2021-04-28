cmake_minimum_required(VERSION 2.4)
project(nvindex)

#set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -DPERF_LATENCY -DUSE_NVM_MALLOC -DNO_CONCURRENT -DNDEBUG -g -O2 -mrtm")
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -O3 -mrtm -lpmem -lvmem -fpermissive")

link_directories("${PROJECT_SOURCE_DIR}/third-party-lib")
link_libraries("${PROJECT_SOURCE_DIR}/third-party-lib/libtbb.so.2")
link_directories(/usr/lib/x86_64-linux-gnu/)
link_libraries(boost_system boost_thread pthread)

include_directories("third-party-lib")
include_directories("include")
include_directories("test")

add_executable(benchmark
		test/main.cpp
		)

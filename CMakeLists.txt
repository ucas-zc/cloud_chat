# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12.0)

if(CMAKE_BUILD_RPATH)
    message("CMAKE_BUILD_RPATH: ${CMAKE_BUILD_RPATH}")
endif()

# 项目信息
project (cloud_chat)
set (CC_MAX_VERSION 1)
set (CC_MID__VERSION 0)
set (CC_MIN_VERSION 0)

# 设置编译选项
if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES DEBUG)) 
    add_definitions(-D_DEBUG_)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage") 
endif()    

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES RELEASE))
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

if (CMAKE_BUILD_RPATH AND (CMAKE_BUILD_TYPE MATCHES GTEST)) 
    add_definitions(-D_GTEST_)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -fprofile-arcs -ftest-coverage") 
endif()    

# 目录下的所有源文件
aux_source_directory(src/core CC_CORE_FILES)
aux_source_directory(src/event CC_EVENT_FILES)
aux_source_directory(src/event/modules CC_EVENT_MODULES_FILES)
aux_source_directory(src/log CC_LOG_FILES)
aux_source_directory(src/os/unix CC_OS_UNIX_FILES)
if (CMAKE_BUILD_TYPE MATCHES GTEST)
	aux_source_directory(gtest CC_TEST_FILES)
	aux_source_directory(gtest/src CC_TEST_SRC_FILES)
endif()

# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 库文件目录
include_directories(${CMAKE_BUILD_RPATH}/include)
link_directories(${CMAKE_BUILD_RPATH}/lib)

# 指定生成目标
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_executable(cloud_chat ${CC_CORE_FILES}
					${CC_EVENT_FILES}
					${CC_EVENT_MODULES_FILES}
					${CC_OS_UNIX_FILES}
					${CC_LOG_FILES})
	target_link_libraries(cloud_chat PRIVATE gcov)
endif()

if (CMAKE_BUILD_TYPE MATCHES RELEASE)
	add_executable(cloud_chat ${CC_CORE_FILES}
					${CC_EVENT_FILES}
					${CC_EVENT_MODULES_FILES}
					${CC_OS_UNIX_FILES}
					${CC_LOG_FILES})
endif()

if (CMAKE_BUILD_TYPE MATCHES GTEST)
    #添加编译选项
    add_executable(cloud_chat ${CC_TEST_SRC_FILES} 
                    ${CC_TEST_FILES})
    target_link_libraries(cloud_chat PRIVATE gtest)
    target_link_libraries(cloud_chat PRIVATE gcov)

   # #执行程序
   add_custom_target(test
   COMMAND ./cloud_chat
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
   COMMENT "run test suit.")

   add_custom_target(gen_coverage
   COMMAND lcov -d CMakeFiles/cloud_chat.dir/gtest/src -t cloud_chat -o 'cloud_chat.cov.info' -b . -c
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
   COMMENT "genarate coverage info")

   add_custom_target(gen_coverage_html
   COMMAND genhtml -o cov_result cloud_chat.cov.info
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
   COMMENT "genarate coverage info to ./cov_result")
   
   add_dependencies(gen_coverage test)
   add_dependencies(gen_coverage cloud_chat)
   add_dependencies(gen_coverage_html gen_coverage)
endif()

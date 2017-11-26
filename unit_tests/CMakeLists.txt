cmake_minimum_required(VERSION 3.7)
project(unit_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

include_directories(../include)
link_directories(../lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES
        unit_tests.cpp
        ../include/apidisk.h
        ../include/t2fs.h
        ../include/fsmanager.h
        ../include/file_operations.h
        ../src/t2fs.c
        ../src/fsmanager.c
        ../src/file_operations.c
        ../lib/apidisk.o)

add_executable(unit_tests ${SOURCE_FILES})

target_link_libraries(unit_tests gtest gtest_main)

set_target_properties(unit_tests PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_subdirectory(gtest)

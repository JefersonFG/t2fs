cmake_minimum_required(VERSION 3.7)
project(teste)

set(CMAKE_C_STANDARD 99)

include_directories(../include)
link_directories(../lib)

set(SOURCE_FILES
        main.c
        ../include/apidisk.h
        ../include/t2fs.h
        ../include/fsmanager.h
        ../include/file_operations.h
        ../src/t2fs.c
        ../src/fsmanager.c
        ../src/file_operations.c
        ../lib/apidisk.o)

add_executable(teste ${SOURCE_FILES})

set_target_properties(teste PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
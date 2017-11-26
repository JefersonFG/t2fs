cmake_minimum_required(VERSION 3.7)
project(t2fs)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        include/apidisk.h
        include/t2fs.h
        include/fsmanager.h
        include/file_operations.h
        src/t2fs.c
        src/fsmanager.c
        src/file_operations.c)

add_library(t2fs ${SOURCE_FILES})
target_link_libraries(t2fs lib/apidisk.o)
set_target_properties(t2fs PROPERTIES COMPILE_FLAGS "-m32 -Wall" LINK_FLAGS "-m32 -Wall")

add_subdirectory(teste)

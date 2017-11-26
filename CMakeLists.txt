cmake_minimum_required(VERSION 3.7)
project(t2fs)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        include/apidisk.h
        include/t2fs.h
        src/t2fs.c include/fsmanager.h src/fsmanager.c include/file_operations.h src/file_operations.c)

add_library(t2fs ${SOURCE_FILES})
target_link_libraries(t2fs lib/apidisk.o)
set_target_properties(t2fs PROPERTIES COMPILE_FLAGS "-m32 -Wall" LINK_FLAGS "-m32 -Wall")

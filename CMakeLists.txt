cmake_minimum_required(VERSION 3.10)
project(glove)
set(C_STANDARD 17)

set(
    BIN_SOURCES 
    src/glove.c 
    src/main.c
)
add_executable(
    example
    ${BIN_SOURCES}
)
target_link_libraries(
    example
    m
)

set(
    LIB_SOURCES 
    src/glove.c
    src/jni.c
)
add_library(
    ${CMAKE_PROJECT_NAME}
    SHARED
    ${LIB_SOURCES}
)
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    m
)
target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    "/usr/lib/jvm/java-17-openjdk-amd64/include"
    "/usr/lib/jvm/java-17-openjdk-amd64/include/linux"
)

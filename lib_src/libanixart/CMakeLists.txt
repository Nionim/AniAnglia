cmake_minimum_required(VERSION 3.20)
project(anixart)

enable_language(CXX)
enable_language(OBJC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE OBJC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${OBJC_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -fobjc-arc
    -frtti
    -fexceptions
)
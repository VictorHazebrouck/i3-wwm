cmake_minimum_required(VERSION 3.20)
project(run LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

file(GLOB_RECURSE CORE_SRCS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/core/*.cpp)
file(GLOB_RECURSE APP_SRCS  CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/app/*.cpp)

set(SOURCES ${CORE_SRCS} ${APP_SRCS})

add_executable(run ${SOURCES})

target_include_directories(run PRIVATE ${CMAKE_SOURCE_DIR})

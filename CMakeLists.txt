cmake_minimum_required(VERSION 3.8)

set(This conanTemplate)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions("-std=c++17")

enable_testing()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
    ${CMAKE_SOURCE_DIR}/external/tabulate,
    ${CMAKE_SOURCE_DIR}/external
)

set(Headers
    src/my_SqliteCpp.h
)

set(Sources
    src/main.cpp
)

add_executable(${This} ${Sources} ${Headers})
target_link_libraries(${This} ${CONAN_LIBS})

add_subdirectory(test)

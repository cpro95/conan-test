cmake_minimum_required(VERSION 2.8.12)
project(imgui-sfml-test)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
    ${CMAKE_SOURCE_DIR}/src/Third_Party/tabulate,
    ${CMAKE_SOURCE_DIR}/src/Third_Party

)

add_executable(conanTest src/main.cpp)
target_link_libraries(conanTest ${CONAN_LIBS})


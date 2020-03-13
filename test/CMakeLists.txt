cmake_minimum_required(VERSION 3.8)

set(This conanTest)

set(Sources
    conanTests.cpp
)
set(Headers
    ../src/my_SqliteCpp.h
)

add_executable(${This} ${Sources} ${Headers})
target_link_libraries(${This} gtest_main ${CONAN_LIBS})

add_test(
    NAME ${This}
    COMMAND ${This}
)
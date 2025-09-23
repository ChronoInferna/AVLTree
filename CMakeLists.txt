cmake_minimum_required(VERSION 3.22)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

# Compile flags to match Gradescope test environment
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
# Gradescope does use -Werror, so if you remove it here you may run into issues when trying to submit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(src)

add_executable(Main
        src/main.cpp
        src/AVL.hpp
        src/AVL.cpp
        src/Student.hpp
        src/Student.cpp
        )
        
# These tests can use the Catch2-provided main
add_executable(Tests
        test/test.cpp
        src/AVL.hpp
        src/AVL.cpp
        src/Student.hpp
        src/Student.cpp
        )
        
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

# find_package(Catch2 3.10 REQUIRED)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)
catch_discover_tests(Tests
    ADD_TAGS_AS_LABELS
)

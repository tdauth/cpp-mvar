cmake_minimum_required(VERSION 3.12)
project(cpp-mvar)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(mvar src/test/mvar.cpp)
add_test(MVarTest mvar)

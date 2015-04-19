project(PRNG)
cmake_minimum_required(VERSION 3.0)


set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/tests.cpp
    ${CMAKE_SOURCE_DIR}/PRNG.h
)

include_directories(
    ${CMAKE_SOURCE_DIR} include
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

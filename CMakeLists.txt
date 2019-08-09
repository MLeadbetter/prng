project(PRNG)
cmake_minimum_required(VERSION 3.0)

# Download gtest ready for use
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
 RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
   message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
 RESULT_VARIABLE result
 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                  ${CMAKE_BINARY_DIR}/googletest-build
                                  EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
   include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Configure project
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -std=c++11")

set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/tests.cpp
    ${CMAKE_SOURCE_DIR}/PRNG.h
)

include_directories(
    ${CMAKE_SOURCE_DIR} include
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} gtest_main)

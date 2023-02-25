cmake_minimum_required(VERSION 3.8)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(JobySimulation
  VERSION 1.0
  LANGUAGES C CXX
)

add_compile_options(-Wall -Werror)

set(APP_NAME my_program)
set(APP_LIB_NAME my_program_lib)

option(COMPILE_UT_HARNESS "Compile CppUTest" OFF)
if (COMPILE_UT_HARNESS)
  set(ENV{CPPUTEST_HOME} ${CMAKE_SOURCE_DIR}/cpputest)
  add_subdirectory(cpputest)
endif(COMPILE_UT_HARNESS)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

option(COMPILE_TESTS "Compile the tests" ON)
if (COMPILE_TESTS)
  add_subdirectory(UnitTest)
endif(COMPILE_TESTS)

add_subdirectory(src)

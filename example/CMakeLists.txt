cmake_minimum_required(VERSION 3.16)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(
  filter_example
  LANGUAGES CXX
  VERSION 0.1)

find_package(LibButterworth REQUIRED)

add_executable(filter_example main.cpp)
target_link_libraries(filter_example libbutterworth::libbutterworth)

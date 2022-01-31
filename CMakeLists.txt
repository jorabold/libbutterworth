cmake_minimum_required(VERSION 3.16)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(
  butterlib
  LANGUAGES CXX
  VERSION 0.1)

# setup googletest (http://google.github.io/googletest/quickstart-cmake.html)
set(INSTALL_GTEST
    OFF
    CACHE BOOL "Install Googletest")
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Configure project sources
add_library(
  libbutterworth STATIC
  libbutterworth/biquad.h
  libbutterworth/biquad.cpp
  libbutterworth/butterworth.h
  libbutterworth/butterworth.cpp
  libbutterworth/filter_design.h
  libbutterworth/filter_design.cpp
  libbutterworth/utils.h
  libbutterworth/utils.cpp)
target_include_directories(
  libbutterworth PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                        $<INSTALL_INTERFACE:include>)

add_executable(filter_example example/main.cpp)
target_link_libraries(filter_example libbutterworth)

# add the tests (using google-test)
enable_testing()
add_executable(
  filter_tests
  libbutterworth/butterworth_tests.cpp libbutterworth/biquad_tests.cpp
  libbutterworth/filter_design_tests.cpp libbutterworth/utils_tests.cpp)
target_link_libraries(filter_tests gtest_main libbutterworth)
include(GoogleTest)
gtest_discover_tests(filter_tests)

# setup installation process
install(
  TARGETS libbutterworth
  EXPORT LibButterworthTargets
  ARCHIVE DESTINATION lib
  INCLUDES
  DESTINATION include)

install(
  EXPORT LibButterworthTargets
  FILE LibButterworthTargets.cmake
  NAMESPACE libbutterworth::
  DESTINATION lib/cmake/libbutterworth)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/LibButterworthConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_LIST_DIR}/LibButterworthConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/LibButterworthConfigVersion.cmake
        DESTINATION lib/cmake/libbutterworth)

# Install header files.
install(
  DIRECTORY libbutterworth/
  DESTINATION include/libbutterworth
  FILES_MATCHING
  PATTERN "*.h")

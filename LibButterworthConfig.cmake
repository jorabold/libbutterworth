# Config file for LibButterworth - Find LibButterworth.
#
# This file is used by CMake when find_package(LibButterworth) is invoked. It's
# based on Ceres' config file.
#
# This module defines the following variables:
#
# LibButterworth_FOUND / LIBBUTTERWORTH_FOUND: True if LibButterworth has been
# successfully found. Both variables are set as although find_package() only
# references LibButterworth_FOUND in Config mode, given the conventions for
# <package>_FOUND when find_package() is called in Module mode, users could
# reasonably expect to use LIBBUTTERWORTH_FOUND instead.
#
# LIBBUTTERWORTH_VERSION: Version of LibButterworth found.
#
# LIBBUTTERWORTH_LIBRARIES: Libraries for LibButterworth.
#
# NOTE: There is no equivalent of LIBBUTTERWORTH_INCLUDE_DIRS as the exported
# CMake target already includes the definition of its public include
# directories.

# libbutterworth_message([mode] "message text")
#
# Wraps the standard cmake 'message' command, but suppresses output if the QUIET
# flag was passed to the find_package(LibButterworth ...) call.
function(libbutterworth_message)
  if(NOT LibButterworth_FIND_QUIETLY)
    message(${ARGN})
  endif()
endfunction()

get_filename_component(LibButterworth_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}"
                       PATH)
include(CMakeFindDependencyMacro)

if(NOT TARGET libbutterworth::libbutterworth)
  include("${LibButterworth_CMAKE_DIR}/LibButterworthTargets.cmake")
  include("${LibButterworth_CMAKE_DIR}/LibButterworthConfigVersion.cmake")
endif()

set(LIBBUTTERWORTH_LIBRARIES libbutterworth::libbutterworth)
set(LIBBUTTERWORTH_FOUND TRUE)
set(LIBBUTTERWORTH_VERSION ${LibButterworth_VERSION})

libbutterworth_message(
  STATUS
  "Found LibButterworth ${LibButterworth_VERSION} (in ${LibButterworth_DIR})")

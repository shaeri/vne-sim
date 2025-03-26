cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(vne-mcts CXX)

# The version number.
set(vne-mcts_VERSION_MAJOR 1.0)
set(vne-mcts_VERSION_MINOR 0)

# Make sure compiler version is at least 4.9
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
    message(FATAL_ERROR "GCC 4.9 is required to compile VNE-Sim..")
  endif()
endif()

# Download and build dependencies
if(NOT EXISTS "${CMAKE_BINARY_DIR}/deps")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/deps)
  execute_process(
    COMMAND ${PROJECT_SOURCE_DIR}/scripts/build-deps.sh
    WORKING_DIRECTORY deps COMMAND_ECHO STDOUT
    OUTPUT_FILE stdout.log
    ERROR_FILE stderr.log COMMAND_ERROR_IS_FATAL ANY)
endif()

# Enable C++11 compilation list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -w")
list(APPEND CMAKE_CXX_FLAGS "-std=c++14 -w")
# TODO: remove -w by fixing warning messages in code

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_SHARED_LINKER_FLAGS " -ldl")
  add_definitions(-DDARWIN)
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_SHARED_LINKER_FLAGS " -Wl,--no-as-needed -ldl")
endif()

# Set the build directory
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Set library path
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Set executable path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(HIBERLITE_LIBRARY hiberlite)
add_library(${HIBERLITE_LIBRARY} STATIC IMPORTED)
set_property(
  TARGET ${HIBERLITE_LIBRARY}
  PROPERTY IMPORTED_LOCATION ${PROJECT_BINARY_DIR}/deps/lib/libhiberlite.a)

# Download toml.hpp
file(
  DOWNLOAD
  "https://raw.githubusercontent.com/ToruNiina/toml11/refs/tags/v4.4.0/single_include/toml.hpp"
  "${PROJECT_BINARY_DIR}/deps/include/toml.hpp"
  TLS_VERIFY ON)
# Download cli11.hpp
file(
  DOWNLOAD
  "https://github.com/CLIUtils/CLI11/releases/download/v2.5.0/CLI11.hpp"
  "${PROJECT_BINARY_DIR}/deps/include/CLI11.hpp"
  TLS_VERIFY ON)

# Add the include directories of external-libraries
include_directories(${PROJECT_BINARY_DIR}/deps/include)
# For BRITE Network Generator
include_directories(${PROJECT_SOURCE_DIR}/deps)

# Local projec headers
include_directories(${PROJECT_SOURCE_DIR}/src)

# Local modules directory
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# Find boost libraries
find_package(
  Boost 1.56.0
  COMPONENTS filesystem
             log
             log_setup
             program_options
             thread
             system
             unit_test_framework
  REQUIRED)
find_package(GSL REQUIRED)
find_package(GLPK REQUIRED)
find_package(SQLITE3 REQUIRED)
option(WITH_FNSS_SUPPORT "WITH_FNSS_SUPPORT" OFF)
if(${WITH_FNSS_SUPPORT})
  set(Python3_FIND_FRAMEWORK NEVER)
  set(Python3_FIND_VIRTUALENV FIRST)
  find_package(Python3 COMPONENTS Interpreter Development)
  message(STATUS "..EXEC ${Python3_EXECUTABLE}")
  message(STATUS "..LIBS ${Python3_LIBRARIES}")
  message(STATUS "..LIB_DIRS ${Python3_LIBRARY_DIRS}")
  message(STATUS "..RUNTIME_LIB_DIRS ${Python3_RUNTIME_LIBRARY_DIRS}")
  message(STATUS "..INC ${Python3_INCLUDE_DIRS}")
  if(Python3_RUNTIME_LIBRARY_DIRS)
    message(STATUS "Python Libraries were found! Compiling with FNSS support.")
    add_definitions(-DPYTHON_AVAILABLE)
    include_directories(${Python3_INCLUDE_DIRS})
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/deps/src/fnss)
      message(STATUS "FNSS DOES NOT EXIST...")

    endif()
    set(FNSS_LIBRARY fnss)
    add_library(${FNSS_LIBRARY} STATIC IMPORTED)
    link_directories(${Python3_LIBRARY_DIRS})
    set_property(
      TARGET ${FNSS_LIBRARY} PROPERTY IMPORTED_LOCATION
                                      ${PROJECT_BINARY_DIR}/deps/lib/libfnss.a)
    include_directories(${PROJECT_BINARY_DIR}/deps/include)
    add_definitions(-DWITH_FNSS_SUPPORT)
  else()
    message(
      STATUS "Python Libraries were not found! Compiling without FNSS support.")
  endif()
endif()

# MPI Support
option(ENABLE_MPI "ENABLE_MPI" OFF)
if(${ENABLE_MPI})
  message(STATUS "Trying to compile with MPI. Searching for MPI Libraries...")
  find_package(MPI)
  if(${MPI_FOUND})
    message(STATUS "Compiling with MPI support.")
    add_definitions(-DENABLE_MPI)
    set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    # set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
    include_directories(${MPI_INCLUDE_PATH})
  else()
    message(STATUS "Could not find the MPI Libraries...")

  endif()
else()
  message(
    STATUS
      "Compiling without MPI support. Use option -DENABLE_MPI=ON to compile with MPI support."
  )
endif()

# add subdirectories that need to be compiled
add_subdirectory(src)
add_subdirectory(deps/BRITE)

# Enable C++11 standard set_property(TARGET vne-mcts PROPERTY CXX_STANDARD 11)
# set_property(TARGET vne-mcts PROPERTY CXX_STANDARD_REQUIRED ON)

# target_link_libraries(src hiberlite)

project (vne-mcts CXX)

cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

# The version number.
set (vne-mcts_VERSION_MAJOR 1.0)
set (vne-mcts_VERSION_MINOR 0)

# Enable C++11 compilation
list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(CMAKE_SHARED_LINKER_FLAGS " -ldl")
ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(CMAKE_SHARED_LINKER_FLAGS " -Wl,--no-as-needed -ldl")
ENDIF()

# Set the build directory
SET(PROJECT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Set library path
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Set executable path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Enable include external libraries (adevs and hiberlite)
include (ExternalProject) 

# ADEVS does not require building. We just make sure it exists in the external-libs dir.
# Otherwise, download it from the URL.
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/external-libs/adevs)
MESSAGE (STATUS "ADEVS DOES NOT EXIST")
	ExternalProject_Add (adevs
			     #DOWNLOAD_DIR /tmp/
  			     URL http://web.ornl.gov/~1qn/adevs/adevs.tar.gz  
			     SOURCE_DIR ${PROJECT_SOURCE_DIR}/external-libs/adevs
			     CONFIGURE_COMMAND ""
			     BUILD_COMMAND ""
			     INSTALL_COMMAND ""
			     LOG_DOWNLOAD ON
                            )
endif()

# We are using a modified version of hiberlite, therefore, we are bound to use
# the version that comes with the source code and we cannot download it fresh from
# its git repository.
ExternalProject_Add (hiberlite_project
                             SOURCE_DIR ${PROJECT_SOURCE_DIR}/external-libs/hiberlite
                             CONFIGURE_COMMAND ""
			     BUILD_IN_SOURCE 1
			     LOG_BUILD 1
                             BUILD_COMMAND make
                             INSTALL_COMMAND ""
                            )


# Add the include directories of ADEVS and hiberlite and BRITE
include_directories(${PROJECT_SOURCE_DIR}/external-libs/adevs/include)
include_directories(${PROJECT_SOURCE_DIR}/external-libs/hiberlite/include)
include_directories(${PROJECT_SOURCE_DIR}/external-libs/BRITE/C++)

set(HIBERLITE_LIBRARY hiberlite)
add_library(${HIBERLITE_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${HIBERLITE_LIBRARY} PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external-libs/hiberlite/libhiberlite.a)

# Local projec headers
include_directories(${PROJECT_SOURCE_DIR}/src)

# Local modules directory
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# Find boost libraries
find_package(Boost
             1.54.0
	     COMPONENTS	filesystem
			log
			log_setup
			thread
			system
			unit_test_framework
             REQUIRED)
find_package(GSL REQUIRED)
find_package(GLPK REQUIRED)		
find_package(SQLITE3 REQUIRED)

#add subdirectories that need to be compiled
add_subdirectory(src)
add_subdirectory(external-libs/BRITE)

# add_executable (	 )

# Enable C++11 standard
# set_property(TARGET vne-mcts PROPERTY CXX_STANDARD 11)
# set_property(TARGET vne-mcts PROPERTY CXX_STANDARD_REQUIRED ON)

#target_link_libraries(src hiberlite)

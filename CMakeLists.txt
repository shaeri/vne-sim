project (vne-mcts CXX)

cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

# The version number.
set (vne-mcts_VERSION_MAJOR 1.0)
set (vne-mcts_VERSION_MINOR 0)

#Make sure compiler version is at least 4.9
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
    message(FATAL_ERROR "GCC 4.9 is required to compile VNE-Sim..")
  endif()
endif()

# Enable C++11 compilation
list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(CMAKE_SHARED_LINKER_FLAGS " -ldl")
	add_definitions (-DDARWIN)
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
  			     URL http://web.ornl.gov/~1qn/adevs/adevs-2.8.tar.gz  
			     SOURCE_DIR ${PROJECT_SOURCE_DIR}/external-libs/adevs
			     CONFIGURE_COMMAND ""
			     BUILD_COMMAND ""
			     INSTALL_COMMAND ""
			     LOG_DOWNLOAD 1
                            )
endif()

# We are using a modified version of hiberlite, therefore, we are bound to use
# the version that comes with the source code and we cannot download it fresh from
# its git repository.
IF (NOT EXISTS ${PROJECT_SOURCE_DIR}/external-libs/hiberlite)
MESSAGE (STATUS "HIBERLITE DOES NOT EXIST...")
ExternalProject_Add (hiberlite_project
			     GIT_REPOSITORY https://github.com/paulftw/hiberlite.git 
                             SOURCE_DIR ${PROJECT_SOURCE_DIR}/external-libs/hiberlite
                             CONFIGURE_COMMAND ""
			     PATCH_COMMAND git apply ../../cmake/patches/hiberlite.local.patch
			     BUILD_IN_SOURCE 1
			     LOG_BUILD 1
                             BUILD_COMMAND make
                             INSTALL_COMMAND ""
                            )
ENDIF ()

set(HIBERLITE_LIBRARY hiberlite)
add_library(${HIBERLITE_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${HIBERLITE_LIBRARY} PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external-libs/hiberlite/libhiberlite.a)

# Add the include directories of external-libraries 
include_directories(${PROJECT_SOURCE_DIR}/external-libs/)

# Local projec headers
include_directories(${PROJECT_SOURCE_DIR}/src)

# Local modules directory
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# Find boost libraries
find_package(Boost
             1.56.0
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
option (WITH_FNSS_SUPPORT "WITH_FNSS_SUPPORT" ON)
IF (${WITH_FNSS_SUPPORT})
    find_package(PythonLibs)
    IF(${PYTHONLIBS_FOUND})
            MESSAGE (STATUS "Python Libraries were found! Compiling with FNSS support.") 
            add_definitions (-DPYTHON_AVAILABLE)
            include_directories(${PYTHON_INCLUDE_DIRS})
            IF (NOT EXISTS ${PROJECT_SOURCE_DIR}/external-libs/fnss)
              MESSAGE (STATUS "FNSS DOES NOT EXIST...")
			  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
				ExternalProject_Add (fnss_project
                                 #DOWNLOAD_DIR /tmp/
                                 GIT_REPOSITORY https://github.com/fnss/fnss.git 
                                 SOURCE_DIR ${PROJECT_SOURCE_DIR}/external-libs/fnss
                                 PATCH_COMMAND git apply ../../cmake/patches/fnss.fix_quantity_clang.patch
												&& git apply ../../cmake/patches/fnss.fix_macos_sed.patch
                                 CONFIGURE_COMMAND ""
                                 BUILD_COMMAND cd ${PROJECT_SOURCE_DIR}/external-libs/fnss/core && 
												sudo python setup.py clean && sudo python setup.py build &&  
												sudo python setup.py install && cd ${PROJECT_SOURCE_DIR}/external-libs/fnss/cpp && make
                                 INSTALL_COMMAND ""
                                 LOG_DOWNLOAD ON
                                )
			  ELSE()
				ExternalProject_Add (fnss_project
                                 #DOWNLOAD_DIR /tmp/
                                 GIT_REPOSITORY https://github.com/fnss/fnss.git 
                                 SOURCE_DIR ${PROJECT_SOURCE_DIR}/external-libs/fnss
                                 PATCH_COMMAND git apply ../../cmake/patches/fnss.fix_quantity_clang.patch
                                 CONFIGURE_COMMAND ""
                                 BUILD_COMMAND cd ${PROJECT_SOURCE_DIR}/external-libs/fnss/core && 
												sudo python setup.py clean && sudo python setup.py build &&  
												sudo python setup.py install && cd ${PROJECT_SOURCE_DIR}/external-libs/fnss/cpp && make
                                 INSTALL_COMMAND ""
                                 LOG_DOWNLOAD ON
                                )
			  ENDIF()
      ENDIF() 
      set(FNSS_LIBRARY fnss)
      add_library(${FNSS_LIBRARY} STATIC IMPORTED)
      set_property(TARGET ${FNSS_LIBRARY} PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/external-libs/fnss/cpp/lib/libfnss.a) 
      include_directories(${PROJECT_SOURCE_DIR}/external-libs/fnss/cpp/deps)
      add_definitions(-DWITH_FNSS_SUPPORT)
    ELSE()
            MESSAGE (STATUS "Python Libraries were not found! Compiling without FNSS support.")  
    ENDIF()
ENDIF ()

#MPI Support
option (ENABLE_MPI "ENABLE_MPI" OFF)
IF (${ENABLE_MPI})
	MESSAGE (STATUS "Trying to compile with MPI. Searching for MPI Libraries...")
	find_package(MPI)
	IF (${MPI_FOUND})
		MESSAGE (STATUS "Compiling with MPI support.") 
		add_definitions (-DENABLE_MPI)
		set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
		# set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
		include_directories(${MPI_INCLUDE_PATH})	
	ELSE()
		MESSAGE (STATUS "Could not find the MPI Libraries...")
		 
	ENDIF()
ELSE()
MESSAGE (STATUS "Compiling without MPI support. Use option -DENABLE_MPI=ON to compile with MPI support.") 
ENDIF()

#add subdirectories that need to be compiled
add_subdirectory(src)
add_subdirectory(external-libs/BRITE)

# add_executable (	 )

# Enable C++11 standard
# set_property(TARGET vne-mcts PROPERTY CXX_STANDARD 11)
# set_property(TARGET vne-mcts PROPERTY CXX_STANDARD_REQUIRED ON)

#target_link_libraries(src hiberlite)

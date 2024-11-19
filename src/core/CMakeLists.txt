set(Config_File_Path "\"configurations.xml\"")

# TODO: add command line switch to read configurations.xml file

configure_file (
  		"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  		"${CMAKE_CURRENT_SOURCE_DIR}/config.h"
  )

add_library (core STATIC
		config-manager.cc
		db-manager.cc
		id-generator.cc
		rng.cc
		experiment-parameters.cc
	    )

target_link_libraries (core ${GSL_LIBRARIES} ${HIBERLITE_LIBRARY} ${MPI_LIBRARIES})

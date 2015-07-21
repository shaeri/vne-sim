set(Config_File_Path "\"${PROJECT_SOURCE_DIR}/configurations.xml\"")

configure_file (
  		"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  		"${CMAKE_CURRENT_SOURCE_DIR}/config.h"
  )

add_library (core SHARED
		config-manager.cc
		db-manager.cc
		id-generator.cc
		rng.cc
		experiment-parameters.cc
	    )

target_link_libraries (core ${GSL_LIBRARIES} ${HIBERLITE_LIBRARY} ${MPI_LIBRARIES})

add_executable(experiments-test experiments-test.cc)

target_link_libraries(experiments-test experiments utilities ${MPI_LIBRARIES})

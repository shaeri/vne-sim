include_directories(${Boost_INCLUDE_DIR})

add_executable(vnesim main.cc)
target_link_libraries(vnesim nfg experiments utilities ${Boost_LIBRARIES}
                      ${MPI_LIBRARIES} fmt::fmt-header-only)

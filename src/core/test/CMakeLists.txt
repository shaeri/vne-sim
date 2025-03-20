add_executable(core-test core-test.cc)

target_link_libraries(core-test core utilities ${BOOST_LIBRARIES})

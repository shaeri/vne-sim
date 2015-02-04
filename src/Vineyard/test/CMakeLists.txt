add_executable(vineyard-test vy-test.cc)

target_link_libraries (vineyard-test vineyard utilities ${Boost_LIBRARIES})

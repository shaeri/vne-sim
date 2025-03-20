add_library(utilities STATIC boost-test-driver.cc logger.cc python_utils.cpp
                             python_utils.h)
# message(STATUS  ${Boost_LIBRARIES})
target_link_libraries(utilities core ${Boost_LIBRARIES})

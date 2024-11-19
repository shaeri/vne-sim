add_library (utilities STATIC
		boost-test-driver.cc 
		logger.cc
            )
# message(STATUS  ${Boost_LIBRARIES})
target_link_libraries (utilities core ${Boost_LIBRARIES})

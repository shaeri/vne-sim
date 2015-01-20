add_library (utilities SHARED
		boost-test-driver.cc 
		logger.cc
            )
# message(STATUS  ${Boost_LIBRARIES})
target_link_libraries (utilities core ${Boost_LIBRARIES})

add_executable(nfg-test network-file-generator-test.cc)

if(${WITH_FNSS_SUPPORT})
  target_link_libraries(nfg-test nfg utilities ${FNSS_LIBRARY})
else()
  target_link_libraries(nfg-test nfg utilities)
endif()

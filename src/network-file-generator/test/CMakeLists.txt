add_executable(nfg-test network-file-generator-test.cc)

target_link_libraries (nfg-test nfg utilities ${FNSS_LIBRARY})



add_executable(nfg-test network-file-generator-test.cc)

IF (${WITH_FNSS_SUPPORT})
    target_link_libraries (nfg-test nfg utilities ${FNSS_LIBRARY})
ELSE()
    target_link_libraries (nfg-test nfg utilities)
ENDIF()


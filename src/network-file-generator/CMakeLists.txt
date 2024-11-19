add_library (nfg STATIC network-file-generator.cc)

target_link_libraries (nfg core vineyard brite ${Python3_LIBRARIES} ${FNSS_LIBRARY})
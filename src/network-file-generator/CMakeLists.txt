add_library (nfg SHARED network-file-generator.cc)

target_link_libraries (nfg core vineyard brite)


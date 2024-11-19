add_library (nfg STATIC network-file-generator.cc)

target_link_libraries (nfg core vineyard brite python3.13 ${FNSS_LIBRAR})

# TODO: Fix things so python3.13 isn't required in addition to the Python3_... find stuff
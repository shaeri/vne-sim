add_library (nfg SHARED brite-handler.cc
			network-file-generator.cc
	    )

target_link_libraries (nfg core vineyard brite)


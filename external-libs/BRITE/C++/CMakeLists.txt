add_library (brite SHARED
		Edge.cc
		Graph.cc
		Node.cc
		Parser.cc
		Topology.cc
		Util.cc	    
		Models/ASBarabasiAlbertModel-1.cc
		Models/ASBarabasiAlbertModel-2.cc
		Models/ASBarabasiAlbertModel.cc
		Models/ASGLPModel.cc
		Models/ASModel.cc
		Models/ASWaxmanModel.cc
		Models/BottomUpHierModel.cc
		Models/ImportedFileModel.cc
		Models/Model.cc
		Models/RouterBarabasiAlbertModel-1.cc
		Models/RouterBarabasiAlbertModel-2.cc
		Models/RouterBarabasiAlbertModel.cc
		Models/RouterGLPModel.cc
		Models/RouterModel.cc
		Models/RouterWaxmanModel.cc
		Models/TopDownHierModel.cc)

# target_link_libraries (brite)


add_library (mcts SHARED mcts-simulator.cc
			 mcts.cc
			 tree-node.cc
	    )
target_link_libraries (mcts core utilities)


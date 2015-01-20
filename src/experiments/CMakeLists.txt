add_library (experiments SHARED
		mcvne-experiments.cc
		vineyard-experiments.cc	
	    )

target_link_libraries (experiments core mcts mcvne vineyard)


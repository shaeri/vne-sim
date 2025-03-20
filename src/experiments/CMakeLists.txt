add_library(experiments STATIC grc-experiments.cc mcvne-experiments.cc
                               vineyard-experiments.cc)

target_link_libraries(experiments core mcts mcvne vineyard grc)

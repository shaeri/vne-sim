add_library(
  mcvne STATIC mcvne-node-embedding-algo.cc mcvne-bfs-link-embedding-algo.cc
               mcvne-simulator.cc vne-nm-state.cc)

target_link_libraries(mcvne core vineyard mcts)

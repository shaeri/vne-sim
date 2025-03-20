add_library(grc STATIC grc-node-embedding-algo.cc)

target_link_libraries(grc core vineyard)

add_library (grc SHARED grc-node-embedding-algo.cc)

target_link_libraries (grc core vineyard)

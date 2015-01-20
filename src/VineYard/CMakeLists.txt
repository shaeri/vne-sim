add_library (vineyard SHARED
			vy-coordinate.cc
			vy-statistics.cc
			vy-substrate-link.cc
			vy-substrate-network-builder.cc
			vy-substrate-node.cc
			vy-vine-embedding-algo-file-based.cc
			vy-vine-embedding-algo.cc
			vy-vine-link-embedding-algo.cc
			vy-vine-node-embedding-algo.cc
			vy-vine-two-stage-embedding-algo.cc
			vy-virtual-link.cc
			vy-virtual-net-request.cc
			vy-virtual-node.cc
			vy-vnr-embedding-proc.cc
			vy-vnr-generator.cc
			vy-vnr-proc-digraph.cc
			vy-vnr-proc-observer.cc
			vy-vnr-release-proc.cc
			    )

target_link_libraries (vineyard core ${Boost_LIBRARIES} ${GLPK_LIBRARIES})

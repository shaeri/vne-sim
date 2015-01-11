/**
 * @file mcvne-node-embedding-algo.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date 7/16/14
 *
 * @copyright Copyright (c) 7/16/14                      SOROUSH HAERI
 *     All Rights Reserved
 *
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for any purpose and without fee is hereby granted, provided
 *     that the above copyright notice appear in all copies and that both that
 *     copyright notice and this permission notice appear in supporting
 *     documentation, and that the name of the author not be used in advertising or
 *     publicity pertaining to distribution of the software without specific,
 *     written prior permission.
 *
 *     THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *     ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *     DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *     AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **/

#ifndef MCVNE_MCVNE_NODE_EMBEDDING_ALGO_
#define MCVNE_MCVNE_NODE_EMBEDDING_ALGO_

#include "mcvne/mcvne-simulator.h"

#include "core/node-embedding-algorithm.h"

namespace vne {
    namespace mcvne {
        template<typename = Network<VYSubstrateNode<>,VYSubstrateLink<>> ,
        typename = VYVirtualNetRequest<>>
        class MCVNENodeEmbeddingAlgo : public NodeEmbeddingAlgorithm
                                        <Network<VYSubstrateNode<>,VYSubstrateLink<>>, VYVirtualNetRequest<>>
        {
        public:
            MCVNENodeEmbeddingAlgo ();
            ~MCVNENodeEmbeddingAlgo ();
            virtual Embedding_Result  embeddVNRNodes (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr);
        private:
            std::shared_ptr<LinkEmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> link_embedder;
        };
    }
}
#endif /* defined(__vne_mcts__mcvne_node_embedding_algo__) */

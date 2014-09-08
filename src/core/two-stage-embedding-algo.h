/**
 * @file two-stage-embedding-algo.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 12, 2014
 *
 * @copyright Copyright (c) Jun 12, 2014                      SOROUSH HAERI
 *            All Rights Reserved
 *
 *            Permission to use, copy, modify, and distribute this software and its
 *            documentation for any purpose and without fee is hereby granted, provided
 *            that the above copyright notice appear in all copies and that both that
 *            copyright notice and this permission notice appear in supporting
 *            documentation, and that the name of the author not be used in advertising or
 *            publicity pertaining to distribution of the software without specific,
 *            written prior permission.
 *
 *            THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *            ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *            AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *            DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *            AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *            OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef TWO_STAGE_EMBEDDING_ALGO_H_
#define TWO_STAGE_EMBEDDING_ALGO_H_

#include "embedding-algorithm.h"
#include "node-embedding-algorithm.h"
#include "link-embedding-algorithm.h"

namespace vne {
    template<typename SUBNET, typename VNR>
    class TwoStageEmbeddingAlgo : public EmbeddingAlgorithm<SUBNET, VNR>
    {
    public:
        virtual Embedding_Result embeddVNR (std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::VNR_TYPE> vnr,
                                            std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> node_embedding_algo,
                                            std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> link_embedding_algo);
        
    protected:
        TwoStageEmbeddingAlgo (NetworkBuilder<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE>& _sb);
        TwoStageEmbeddingAlgo (std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE> _sn);
        
    };
    template<typename SUBNET, typename VNR>
    TwoStageEmbeddingAlgo<SUBNET,VNR>::TwoStageEmbeddingAlgo (NetworkBuilder<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE>& _sb):
    EmbeddingAlgorithm<SUBNET, VNR>::EmbeddingAlgorithm(_sb)
    {
    }
    template<typename SUBNET, typename VNR>
    TwoStageEmbeddingAlgo<SUBNET,VNR>::TwoStageEmbeddingAlgo (std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE> _sn):
    EmbeddingAlgorithm<SUBNET, VNR>::EmbeddingAlgorithm(_sn)
    {
    }
    template<typename SUBNET, typename VNR>
    Embedding_Result TwoStageEmbeddingAlgo<SUBNET,VNR>
    ::embeddVNR(std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::VNR_TYPE> vnr,
                                            std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> node_embedding_algo,
                                            std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> link_embedding_algo)
    {
        node_embedding_algo->embeddVNRNode (vnr);
        link_embedding_algo->embeddVNRLink (vnr);
    }
    
    
}
#endif

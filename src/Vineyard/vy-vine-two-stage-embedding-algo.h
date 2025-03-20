/**
 * @file vy-vine-two-stage-embedding-algo.h
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

#ifndef VINEYARD_VY_VINE_TWO_STAGE_EMBEDDING_ALGO_
#define VINEYARD_VY_VINE_TWO_STAGE_EMBEDDING_ALGO_

#include "core/network.h"
#include "core/two-stage-embedding-algo.h"

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-net-request.h"
#include "Vineyard/vy-substrate-network-builder.h"

namespace vne
{
namespace vineyard
{
    template <typename = Network<VYSubstrateNode<>, VYSubstrateLink<>>,
              typename = VYVirtualNetRequest<>>
    class VYVineTwoStageEmbeddingAlgo
        : public TwoStageEmbeddingAlgo<Network<VYSubstrateNode<>, VYSubstrateLink<>>,
                                       VYVirtualNetRequest<>>
    {
       public:
        VYVineTwoStageEmbeddingAlgo(
            std::shared_ptr<SUBSTRATE_TYPE> _sn,
            std::shared_ptr<NodeEmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> _node_embedding_algo,
            std::shared_ptr<LinkEmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>>
                _link_embedding_algo);
        VYVineTwoStageEmbeddingAlgo(
            NetworkBuilder<SUBSTRATE_TYPE> &_nb,
            std::shared_ptr<NodeEmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> _node_embedding_algo,
            std::shared_ptr<LinkEmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>>
                _link_embedding_algo);
        virtual ~VYVineTwoStageEmbeddingAlgo();
    };
}  // namespace vineyard
}  // namespace vne
#endif /* defined(__vne_mcts__vy_vine_two_stage_embedding_algo__) */

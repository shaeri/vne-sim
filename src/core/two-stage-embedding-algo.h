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

namespace vne
{
template <typename SUBNET, typename VNR>
class TwoStageEmbeddingAlgo : public EmbeddingAlgorithm<SUBNET, VNR>
{
   public:
    virtual Embedding_Result embeddVNR(
        std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::VNR_TYPE> vnr);
    virtual ~TwoStageEmbeddingAlgo() {};

   protected:
    TwoStageEmbeddingAlgo(
        NetworkBuilder<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE> &_sb,
        std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> _node_embedding_algo,
        std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> _link_embedding_algo);
    TwoStageEmbeddingAlgo(
        std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE> _sn,
        std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> _node_embedding_algo,
        std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> _link_embedding_algo);
    std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> node_embedding_algo;
    std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> link_embedding_algo;
};
template <typename SUBNET, typename VNR>
TwoStageEmbeddingAlgo<SUBNET, VNR>::TwoStageEmbeddingAlgo(
    NetworkBuilder<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE> &_sb,
    std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> _node_embedding_algo,
    std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> _link_embedding_algo)
    : EmbeddingAlgorithm<SUBNET, VNR>::EmbeddingAlgorithm(_sb),
      node_embedding_algo(_node_embedding_algo),
      link_embedding_algo(_link_embedding_algo)
{
}
template <typename SUBNET, typename VNR>
TwoStageEmbeddingAlgo<SUBNET, VNR>::TwoStageEmbeddingAlgo(
    std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::SUBSTRATE_TYPE> _sn,
    std::shared_ptr<NodeEmbeddingAlgorithm<SUBNET, VNR>> _node_embedding_algo,
    std::shared_ptr<LinkEmbeddingAlgorithm<SUBNET, VNR>> _link_embedding_algo)
    : EmbeddingAlgorithm<SUBNET, VNR>::EmbeddingAlgorithm(_sn),
      node_embedding_algo(_node_embedding_algo),
      link_embedding_algo(_link_embedding_algo)
{
}
template <typename SUBNET, typename VNR>
Embedding_Result TwoStageEmbeddingAlgo<SUBNET, VNR>::embeddVNR(
    std::shared_ptr<typename EmbeddingAlgorithm<SUBNET, VNR>::VNR_TYPE> vnr)
{
    if (node_embedding_algo->embeddVNRNodes(this->substrate_network, vnr) ==
            Embedding_Result::SUCCESSFUL_EMBEDDING &&
        link_embedding_algo->embeddVNRLinks(this->substrate_network, vnr) ==
            Embedding_Result::SUCCESSFUL_EMBEDDING) {
        //finalize the node mappings

        for (auto it = vnr->getNodeMap()->begin(); it != vnr->getNodeMap()->end(); it++) {
            assert(this->substrate_network->getNode(it->second)
                           ->embedNode(vnr->getVN()->getNode(it->first)) ==
                       Embedding_Result::SUCCESSFUL_EMBEDDING &&
                   "Trying to map to a more than you have resources!!");
        }
        //finalize the link mappings
        for (auto it1 = vnr->getLinkMap()->begin(); it1 != vnr->getLinkMap()->end(); it1++) {
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                assert(this->substrate_network->getLink(it2->first)
                               ->embedLink(vnr->getVN()->getLink(it1->first), it2->second) ==
                           Embedding_Result::SUCCESSFUL_EMBEDDING &&
                       "Trying to map to a link more than you have resources!!");
            }
        }
        return Embedding_Result::SUCCESSFUL_EMBEDDING;
    }
    return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
}
}  // namespace vne
#endif
/**
 * @file vy-vine-node-embedding-algo.h
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

#ifndef VINEYARD_VY_VINE_NODE_EMBEDDING_ALGO_
#define VINEYARD_VY_VINE_NODE_EMBEDDING_ALGO_

#include "glpk.h"

#include "core/network.h"
#include "core/node-embedding-algorithm.h"

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-net-request.h"

#define META_EDGE_BW 1000000

namespace vne
{
namespace vineyard
{

    template <typename = Network<VYSubstrateNode<>, VYSubstrateLink<>>,
              typename = VYVirtualNetRequest<>>
    class VYVineNodeEmbeddingAlgo
        : public NodeEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>,
                                        VYVirtualNetRequest<>>
    {
       public:
        VYVineNodeEmbeddingAlgo();
        ~VYVineNodeEmbeddingAlgo();
        virtual Embedding_Result embeddVNRNodes(std::shared_ptr<SUBSTRATE_TYPE> substrate_network,
                                                std::shared_ptr<VNR_TYPE> vnr);

       private:
        glp_prob *lp_problem;
        std::string LPmodelFile;
        std::string LPdataFile;

        std::shared_ptr<const std::set<int>> substrateNodeIdSet;
        std::shared_ptr<const std::set<int>> substrateLinkIdSet;
        std::shared_ptr<const std::set<int>> virtualNodeIdSet;
        std::shared_ptr<const std::set<int>> virtualLinkIdSet;

        std::vector<int> allNodeIds;
        //this is based on local ids (vector index)
        std::map<int, std::vector<int>> nodesWithinReach;

        bool setAlpha;
        bool setBeta;
        // 0 is returned if methods are successful otherwise 1 is returned
        inline int solveNodeMappingLP(std::shared_ptr<SUBSTRATE_TYPE> substrate_network,
                                      std::shared_ptr<VNR_TYPE> vnr);
        inline void writeDataFile(std::shared_ptr<SUBSTRATE_TYPE> substrate_network,
                                  std::shared_ptr<VNR_TYPE> vnr);
        inline void cleanUp(std::shared_ptr<SUBSTRATE_TYPE> substrate_network);
        inline Embedding_Result deterministicNodeMapping(
            std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
            std::vector<std::vector<double>> &xVec);
        inline Embedding_Result randomizedNodeMapping(
            std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
            std::vector<std::vector<double>> &xVec);
        //inline Embedding_Result parseLPSolution ();

        enum { DETERMINISTIC, RANDOMIZED } nodeMappingType;

        struct VYNodesReachabilityCondition {
            bool operator()(const VYSubstrateNode<> &lhs, const VYVirtualNode<> &rhs,
                            double maxD) const
            {
                if (IgnoreLocationConstrain())
                    return (lhs.getCPU() >= rhs.getCPU());
                return (lhs.getCoordinates().distanceFrom(rhs.getCoordinates()) <= maxD &&
                        lhs.getCPU() >= rhs.getCPU());
            }
            bool operator()(const std::shared_ptr<const VYSubstrateNode<>> lhs,
                            const std::shared_ptr<const VYVirtualNode<>> rhs, double maxD) const
            {
                if (IgnoreLocationConstrain())
                    return (lhs->getCPU() >= rhs->getCPU());
                return (lhs->getCoordinates().distanceFrom(rhs->getCoordinates()) <= maxD &&
                        lhs->getCPU() >= rhs->getCPU());
            }
        };
    };
}  // namespace vineyard
}  // namespace vne

#endif

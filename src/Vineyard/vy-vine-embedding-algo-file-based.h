/**
 * @file vy-vine-embedding-algo-file-based.h
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
#if 0
#ifndef VINEYARD_VY_VINE_EMBEDDING_ALGO_FILE_BASED_
#define VINEYARD_VY_VINE_EMBEDDING_ALGO_FILE_BASED_

#include "glpk.h"

#include "core/network.h"
#include "core/embedding-algorithm.h"
#include "core/network-builder.h"

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-net-request.h"

namespace vne {
    namespace vineyard{
        
        template<typename = Network<VYSubstrateNode<>,VYSubstrateLink<>> ,
        typename = VYVirtualNetRequest<>>
        class VYVineEmbeddingAlgoFileBased : public EmbeddingAlgorithm <Network<VYSubstrateNode<>,VYSubstrateLink<>>,
        VYVirtualNetRequest<>>
        {
        public:
            VYVineEmbeddingAlgoFileBased (NetworkBuilder<SUBSTRATE_TYPE>& _sb);
            VYVineEmbeddingAlgoFileBased (std::shared_ptr<SUBSTRATE_TYPE> _sn);
            ~VYVineEmbeddingAlgoFileBased ();
            virtual Embedding_Result embeddVNR (std::shared_ptr<VNR_TYPE> vnr);
            
        private:
            glp_prob* lp_problem;
            
            std::string LPmodelFile;
            std::string LPdataFile;
            std::string MCFmodelFile;
            std::string MCFdataFile;
            
            bool setAlpha;
            bool setBeta;
            // 0 is returned if methods are successful otherwise 1 is returned
            inline int solveNodeMappingLP (std::shared_ptr<VNR_TYPE> vnr);
            inline void writeDataFiles (std::shared_ptr<VNR_TYPE> vnr);
            //inline Embedding_Result parseLPSolution ();
            
            enum {DETERMINISTIC,RANDOMIZED} nodeMappingType;
            
            struct VYNodesReachabilityCondition
            {
                bool operator()(const VYSubstrateNode<>& lhs, const VYVirtualNode<>& rhs, double maxD) const
                {
                    return (lhs.getCoordinates().distanceFrom(rhs.getCoordinates())<=maxD &&
                            lhs.getCPU()>=rhs.getCPU() && lhs.touched == false);
                }
                bool operator()(const std::shared_ptr<const VYSubstrateNode<>> lhs, const std::shared_ptr<const VYVirtualNode<>> rhs, double maxD) const
                {
                    return (lhs->getCoordinates().distanceFrom(rhs->getCoordinates())<=maxD &&
                            lhs->getCPU()>=rhs->getCPU() && lhs->touched == false);
                }
            };
        };
    }
}

#endif /* defined(__vne_mcts__vy_vine_embedding_algo_v2__) */
#endif
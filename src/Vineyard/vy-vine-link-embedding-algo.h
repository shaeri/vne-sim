/**
 * @file vy-vine-link-embedding-algo.h
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

#ifndef VINEYARD_VY_VINE_LINK_EMBEDDING_ALGO_
#define VINEYARD_VY_VINE_LINK_EMBEDDING_ALGO_

#include "glpk.h"

#include "core/network.h"
#include "core/link-embedding-algorithm.h"

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-net-request.h"

namespace vne {
    namespace vineyard{
        
        template<typename = Network<VYSubstrateNode<>,VYSubstrateLink<>> ,
        typename = VYVirtualNetRequest<>>
        class VYVineLinkEmbeddingAlgo :
        public LinkEmbeddingAlgorithm <Network<VYSubstrateNode<>,VYSubstrateLink<>>, VYVirtualNetRequest<>>
        {
        public:
            VYVineLinkEmbeddingAlgo ();
            ~VYVineLinkEmbeddingAlgo ();
            virtual Embedding_Result  embeddVNRLinks (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr);
            virtual Embedding_Result  embeddVNRLinksForIdSets (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr, const std::map<int,int>* nodeIdMap,
                std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap) override;
            
        private:
            glp_prob* lp_problem;
            
            std::string MCFmodelFile;
            std::string MCFdataFile;
            
            std::shared_ptr<const std::set<int>> substrateNodeIdSet;
            std::shared_ptr<const std::set<int>> substrateLinkIdSet;
            std::shared_ptr<const std::set<int>> virtualNodeIdSet;
            std::shared_ptr<const std::set<int>> virtualLinkIdSet;
            
            std::vector <int> allNodeIds;
            
            bool setAlpha;
            bool setBeta;
            // 0 is returned if methods are successful otherwise 1 is returned
            inline int solveLinkMappingLP (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr);
            inline void writeDataFile (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
                                       const std::map<int,int>* nodeIdMap);
            
            inline void cleanUp ();
            //inline Embedding_Result parseLPSolution ();
            inline Embedding_Result embeddLinks (
                std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
                const std::map<int,int>* nodeIdMap = nullptr,
                std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap = nullptr);
        };
    }
}
#endif

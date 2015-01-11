/**
 * @file mcvne-experiments.cc
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

#include "mcvne-experiments.h"
#include "mcvne/mcvne-node-embedding-algo.h"
#include "Vineyard/vy-vine-link-embedding-algo.h"

#include <cstring>

namespace vne {
    namespace experiments {
        template<>
        MCVNENodeMCFLinkExp<>::MCVNENodeMCFLinkExp ()
        {
            sb  = std::make_shared<VYSubstrateNetworkBuilder<>>(VYSubstrateNetworkBuilder<>());
            
            std::shared_ptr<VYVineTwoStageEmbeddingAlgo<>> embeddingAlgo (new VYVineTwoStageEmbeddingAlgo<>
                                                                          (*sb,
                                                                           std::make_shared<MCVNENodeEmbeddingAlgo<>>(MCVNENodeEmbeddingAlgo<>()),
                                                                           std::make_shared<VYVineLinkEmbeddingAlgo<>>(VYVineLinkEmbeddingAlgo<>())
                                                                           )
                                                                          );
            
            std::shared_ptr<ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>> releaseAlgo (new ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>(*sb));
            
            VYVNREmbeddingProc<>* embeddingProc (new VYVNREmbeddingProc<>(embeddingAlgo));
            VYVNRReleaseProc<>* releaseProc (new VYVNRReleaseProc<>(releaseAlgo));
            VYVNRProcObserver<>* observer (new VYVNRProcObserver<> (sb->getNetwork()));
            VYVNRGenerator<>* gen (new VYVNRGenerator<>());
            observer->registerSubscriber(this);
            
            graph = new VYVNRProcDigraph<> (embeddingProc, releaseProc, gen, observer);
            
            std::string dirName = ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.dir");
            
            
            vector<int> tokens (8);
            tokens[0] = -1;
            for (int i = 0; i<8 ;i++)
            {
                int pos = (int) dirName.find_first_of("-");
                std::string tok = dirName.substr(0,pos);
                if (i != 0)
                    tokens[i] = atoi(tok.c_str());
                dirName = dirName.substr(pos+1,dirName.length());
            }
            
            
            std::string link_algo ("MCF Link Mapping");
            std::string node_algo ("MCVNE Node Mapping");
            
            Embedding_Algorithm_Types algo_type = Embedding_Algorithm_Types::TWO_STAGE;
            this->initialize (graph, algo_type, node_algo, link_algo, tokens[2],tokens[1],tokens[3],tokens[4],0,tokens[7],tokens[6],tokens[5]);
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setBeta");
        }
        
        template<>
        MCVNENodeMCFLinkExp<>::~MCVNENodeMCFLinkExp()
        {};
        
        template<>
        void MCVNENodeMCFLinkExp<>::statisticsGenerated (Statistics& stat)
        {
            statistics.push_back(*static_cast<VYStatistics*> (&stat));
        }
    }
}
using namespace vne::experiments;
using namespace vne;
HIBERLITE_EXPORT_CLASS_WITH_NAME(MCVNENodeMCFLinkExp<>, MCVNENodeMCFLink)

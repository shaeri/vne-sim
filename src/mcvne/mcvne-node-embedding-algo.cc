/**
 * @file mcvne-node-embedding-algo.cc
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

#include "mcvne-node-embedding-algo.h"

#include "mcts/mcts.h"
#include "Vineyard/vy-vine-link-embedding-algo.h"
#include "core/config-manager.h"

namespace vne {
    namespace mcvne {
        
        template<>
        MCVNENodeEmbeddingAlgo<>::MCVNENodeEmbeddingAlgo()
        :
        NodeEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> > ()
        {
            std::string linkEmbedderName = ConfigManager::Instance()->getConfig<std::string>("MCVNE.NodeEmbeddingAlgo.LinkEmbedder");
            // For now this if statement is useless later if I add more link embedding algorithms this
            // if statement will come in handy.
            if (linkEmbedderName.compare("MCF") == 0)
                link_embedder = std::make_shared<VYVineLinkEmbeddingAlgo<>>();
            
            else
                link_embedder = std::make_shared<VYVineLinkEmbeddingAlgo<>>();

        };
        
        template<>
        MCVNENodeEmbeddingAlgo<>::~MCVNENodeEmbeddingAlgo() {};
        
        template<>
        Embedding_Result
        MCVNENodeEmbeddingAlgo<>::embeddVNRNodes (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            std::shared_ptr<MCVNESimulator<>> sim (new MCVNESimulator<>(substrate_network,vnr,link_embedder));
            MCTS mcts (sim);
            
            std::shared_ptr<VNENMState> st =  std::static_pointer_cast<VNENMState> (sim->createStartState());
            
            bool terminate;
            double reward;
            int action;
            do
            {
                action = mcts.selectAction();
                int vnId = st->getCurrentVNId();
                terminate = sim->step (st, action, reward);
                if (reward <= -Infinity)
                {
                	terminate = true;
                }
                else {
                	vnr->addNodeMapping(action, vnId);
                	if (vnr->getNodeMap()->size() < vnr->getVN()->getNumNodes())
                		mcts.update(action, reward);
                }
            } while (!terminate);
            
            //assert(vnr->getNodeMap()->size() == vnr->getVN()->getNumNodes());
            if (vnr->getNodeMap()->size() != vnr->getVN()->getNumNodes())
                return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
            return Embedding_Result::SUCCESSFUL_EMBEDDING;
        }
    }
}

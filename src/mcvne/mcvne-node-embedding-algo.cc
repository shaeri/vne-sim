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
#include "mcvne-bfs-link-embedding-algo.h"

#include "mcts/mcts.h"
#include "Vineyard/vy-vine-link-embedding-algo.h"
#include "core/config-manager.h"

namespace vne
{
namespace mcvne
{

    template <>
    MCVNENodeEmbeddingAlgo<>::MCVNENodeEmbeddingAlgo()
        : NodeEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>,
                                 VYVirtualNetRequest<>>()
    {
        std::string linkEmbedderName = ConfigManager::Instance()->getConfig<std::string>(
            "MCVNE", "NodeEmbeddingAlgo", "LinkEmbedder");

        if (linkEmbedderName.compare("MCF") == 0)
            link_embedder = std::shared_ptr<VYVineLinkEmbeddingAlgo<>>(
                new VYVineLinkEmbeddingAlgo<>());
        else if (linkEmbedderName.compare("BFS-SP") == 0)
            link_embedder = std::shared_ptr<MCVNEBFSLinkEmbeddingAlgo<>>(
                new MCVNEBFSLinkEmbeddingAlgo<>());
        else
            link_embedder = std::shared_ptr<VYVineLinkEmbeddingAlgo<>>(
                new VYVineLinkEmbeddingAlgo<>());
    };

    template <>
    MCVNENodeEmbeddingAlgo<>::~MCVNENodeEmbeddingAlgo(){};

    template <>
    Embedding_Result MCVNENodeEmbeddingAlgo<>::embeddVNRNodes(
        std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
    {
        std::shared_ptr<MCVNESimulator<>> sim(
            new MCVNESimulator<>(substrate_network, vnr, link_embedder));
        MCTS mcts(sim);

        std::shared_ptr<VNENMState> st = std::static_pointer_cast<VNENMState>(
            sim->createStartState());

        bool terminate;
        double reward;
        int action;
        do {
            action = mcts.selectAction();
            terminate = sim->step(st, action, reward);
            if (reward <= -Infinity)
                terminate = true;
            else {
                if (st->getNodeMap()->size() < vnr->getVN()->getNumNodes())
                    mcts.update(action, reward);
            }
        } while (!terminate);

#ifdef ENABLE_MPI
        if (ConfigManager::Instance()->getConfig<int>("MCTS", "MCTSParameters",
                                                      "ParallelizationType") == 1) {
            struct {
                double val;
                int rank;
            } ObjectiveValueIn, ObjectiveValueOut;
            ObjectiveValueIn.val = reward;
            ObjectiveValueIn.rank = MPI::COMM_WORLD.Get_rank();
            MPI::COMM_WORLD.Allreduce(&ObjectiveValueIn, &ObjectiveValueOut, 1, MPI::DOUBLE_INT,
                                      MPI::MAXLOC);

            int nodeMapSize;
            if (ObjectiveValueOut.rank == ObjectiveValueIn.rank) {
                nodeMapSize = (int)st->getNodeMap()->size();
            }

            MPI::COMM_WORLD.Bcast(&nodeMapSize, 1, MPI::INT, ObjectiveValueOut.rank);

            if (nodeMapSize != vnr->getVN()->getNumNodes())
                return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;

            struct {
                int sNodeId;
                int vNodeId;
            } nodeMap[nodeMapSize];

            if (ObjectiveValueOut.rank == ObjectiveValueIn.rank) {
                int count = 0;
                for (auto it = st->getNodeMap()->begin(); it != st->getNodeMap()->end(); it++) {
                    nodeMap[count].sNodeId = it->second;
                    nodeMap[count].vNodeId = it->first;
                    count++;
                }
            }

            MPI::COMM_WORLD.Bcast(nodeMap, nodeMapSize, MPI_2INT, ObjectiveValueOut.rank);

            for (int i = 0; i < nodeMapSize; i++) {
                vnr->addNodeMapping(nodeMap[i].sNodeId, nodeMap[i].vNodeId);
            }
        } else {
            if (st->getNodeMap()->size() != vnr->getVN()->getNumNodes())
                return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
            for (auto it = st->getNodeMap()->begin(); it != st->getNodeMap()->end(); it++) {
                vnr->addNodeMapping(it->second, it->first);
            }
        }
#else
        if (st->getNodeMap()->size() != vnr->getVN()->getNumNodes())
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
        for (auto it = st->getNodeMap()->begin(); it != st->getNodeMap()->end(); it++) {
            vnr->addNodeMapping(it->second, it->first);
        }
#endif
        return Embedding_Result::SUCCESSFUL_EMBEDDING;
    }
}  // namespace mcvne
}  // namespace vne

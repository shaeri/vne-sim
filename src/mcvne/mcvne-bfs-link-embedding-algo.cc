/**
 * @file mcvne-bfs-link-embedding-algo.cc
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

#include "mcvne-bfs-link-embedding-algo.h"
#include "core/config-manager.h"

namespace vne {
    namespace mcvne{
        template<>
        MCVNEBFSLinkEmbeddingAlgo<>::MCVNEBFSLinkEmbeddingAlgo () :
        LinkEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> > (Link_Embedding_Algo_Types::NO_PATH_SPLITTING),
        substrateLinkIdSet(nullptr),
        substrateNodeIdSet(nullptr)
        {
        }
        template<>
        MCVNEBFSLinkEmbeddingAlgo<>::~MCVNEBFSLinkEmbeddingAlgo()
        {
        }
        
        
        template<>
        inline Embedding_Result MCVNEBFSLinkEmbeddingAlgo<>::embeddLinks (
                                    std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
                                    const std::map<int,int>* nodeIdMap,
                                std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap)
        {
            virtualNodeIdSet = vnr->getVN()->getNodeIdSet();
            virtualLinkIdSet = vnr->getVN()->getLinkIdSet();
            
            if (nodeIdMap == nullptr)
                    nodeIdMap = vnr->getNodeMap();
            
            std::map<int,double> usedSLBW;
            for (auto it = virtualLinkIdSet->begin(); it !=  virtualLinkIdSet->end() ; it++)
            {
                const std::shared_ptr<VYVirtualLink<>> currentVL = vnr->getVN()->getLink(*it);
                
                int VNfromId  = currentVL->getNodeFromId();
                int VNtoId = currentVL->getNodeToId();
                
                double vlBW = currentVL->getBandwidth();
                
                int SNfromId = nodeIdMap->find(VNfromId)->second;
                int SNtoId = nodeIdMap->find(VNtoId)->second;
                
                std::set<int> visitedSubstrateNodesIds;
                
                // Create a queue for BFS
                // It holds the the last visited substrate node id and a list of substrate link ids
                // used to get there
                std::list<std::pair<int, std::list<int>>> queue;
                
                visitedSubstrateNodesIds.insert(SNfromId);
                queue.push_back(std::make_pair (SNfromId , std::list<int>()));
                
                bool foundSNtoId = false;
                std::list<int> shortestPath;
                
                while(!queue.empty() && !foundSNtoId)
                {
                    int currentSNId = queue.front().first;
                    std::list<int> currentPath = queue.front().second;
                    queue.pop_front();
                    
                    const std::shared_ptr<const std::vector<std::shared_ptr<VYSubstrateLink<>>> >
                     slAttachedToCurrentSN = substrate_network->getLinksForNodeId (currentSNId);
                    
                    
                    for (int i = 0; i < slAttachedToCurrentSN -> size(); i++)
                    {
                        //check if the link meets bandwidth constrains otherwise do not consider the link
                        std::shared_ptr<VYSubstrateLink<>> currentSL = slAttachedToCurrentSN->at(i);
                        double residualBW = currentSL->getBandwidth();
                        std::list<int> currentPathCopy = currentPath;
                        if (usedSLBW.find(currentSL->getId()) != usedSLBW.end())
                            residualBW -= usedSLBW[currentSL->getId()];
                        if (residualBW >=  vlBW)
                        {
                            int nextSNId = -1;
                            if (slAttachedToCurrentSN->at(i)->getNodeToId() != currentSNId)
                                nextSNId = slAttachedToCurrentSN->at(i)->getNodeToId();
                            else
                                nextSNId = slAttachedToCurrentSN->at(i)->getNodeFromId();
                            
                            //if the nextnode is destination:
                            if (nextSNId == SNtoId)
                            {
                                foundSNtoId = true;
                                currentPathCopy.push_back(currentSL->getId());
                                usedSLBW[currentSL->getId()] += vlBW;
                                shortestPath = currentPathCopy;
                                break;
                            }
                            //if the destination node is not visited
                            if(visitedSubstrateNodesIds.find(nextSNId) == visitedSubstrateNodesIds.end())
                            {
                                visitedSubstrateNodesIds.insert(nextSNId);
                                currentPathCopy.push_back(slAttachedToCurrentSN->at(i)->getId());
                                usedSLBW[currentSL->getId()] += vlBW;
                                queue.push_back(std::make_pair (nextSNId,currentPathCopy));
                            }
                        }
                    }
                }
                if (queue.empty() && !foundSNtoId)
                   return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
                
                std::shared_ptr<Resources<double>> _res (new Resources<double>(vlBW));
                for (auto it = shortestPath.begin(); it != shortestPath.end(); it++)
                {
                    if (linkMap == nullptr)
                        vnr->addLinkMapping(*it, currentVL->getId(), _res);
                    else
                        (*linkMap)[currentVL->getId()].push_back(std::make_pair(*it, _res));
                }
            
            }
            
            if ((vnr->getLinkMap()->size() == virtualLinkIdSet->size()
                 || (linkMap != nullptr && (linkMap->size() ==  virtualLinkIdSet->size()))))
                return Embedding_Result::SUCCESSFUL_EMBEDDING;
                
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
        }
        
        template<>
        Embedding_Result MCVNEBFSLinkEmbeddingAlgo<>::embeddVNRLinks
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            return (embeddLinks (substrate_network,vnr));
        }
        
        template<>
        Embedding_Result  MCVNEBFSLinkEmbeddingAlgo<>::embeddVNRLinksForIdSets
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
         const std::map<int,int>* nodeIdMap,
         std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap)
        {
            return (embeddLinks (substrate_network,vnr,nodeIdMap,linkMap));
        }
    }
}

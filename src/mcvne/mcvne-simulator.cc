/**
 * @file mcvne-simulator.cc
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

#include "mcvne-simulator.h"

namespace vne {
    namespace mcvne {
        template<>
        MCVNESimulator<>::MCVNESimulator(std::shared_ptr<SUBSTRATE_TYPE> subs_net, std::shared_ptr<VNR_TYPE> vnr,
                                    std::shared_ptr<LinkEmbeddingAlgorithm <SUBSTRATE_TYPE,VNR_TYPE>> _link_embedder)
        : VNEMCTSSimulator<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> >(subs_net,vnr,_link_embedder)
        {
        }
        
        template<>
        MCVNESimulator<>::~MCVNESimulator()
        {
        }
        
        template<>
        std::shared_ptr<std::set<int>>
        MCVNESimulator<>::getValidSubstrateNodeIdSetForVNNodeId (int vn_id, std::shared_ptr<std::set<int>> used_sn_ids) const
        {
            double distance = vnr->getMaxDistance();
            
            return substrate_net->getNodesIDsWithConditions<ReachabilityCondition> (vnr->getVN()->getNode(vn_id), vnr->getVN()->getLinksForNodeId(vn_id), distance, used_sn_ids);
        }
        
        template<>
        double
        MCVNESimulator<>::calculateImmediateReward (std::shared_ptr<VNENMState> st, int action) const
        {
            double beta = 1.0;
            double sn_cpu = substrate_net->getNode(action)->getCPU();
            double vn_cpu = vnr->getVN()->getNode(st->getPreviousVNId())->getCPU();
            if(setBeta)
                beta = sn_cpu;
            return (-(beta/(sn_cpu*1E-6)*vn_cpu));
        }
        
        template<>
        double
        MCVNESimulator<>::calculateFinalReward (std::shared_ptr<VNENMState> st,
        const std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap) const
        {
            double finalReward = 0.0;
            for (auto it = linkMap->begin(); it != linkMap->end(); it++)
            {
                for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
                {
                    double alpha = 1.0;
                    ;
                    double sl_bw = substrate_net->getLink(it2->first)->getBandwidth();
                    double flow_bw = std::get<0>(*(it2->second));
                    if (setAlpha)
                        alpha = sl_bw;
                    
                    finalReward+= (alpha/(sl_bw+1E-6))*flow_bw;
                }
            }
            for (auto it = st->getNodeMap()->begin(); it != st->getNodeMap()->end(); it++)
            {
                double beta = 1.0;
                double sn_cpu = substrate_net->getNode(it->second)->getCPU();
                double vn_cpu = vnr->getVN()->getNode(it->first)->getCPU();
                if(setBeta)
                    beta = sn_cpu;
                finalReward += (beta/(sn_cpu*1E-6))*vn_cpu;
            }
            return -finalReward;
        }
    }
}
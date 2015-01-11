/**
 * @file mcvne-simulator.h
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

#ifndef MCVNE_MCVNE_SIMULATOR_
#define MCVNE_MCVNE_SIMULATOR_

#include "mcvne/vne-mcts-simulator.h"

#include "Vineyard/vy-substrate-link.h"
#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-net-request.h"

#include "core/network.h"

using namespace vne::vineyard;

namespace vne {
    namespace mcvne {
        template<typename = Network<VYSubstrateNode<>,VYSubstrateLink<>> , typename = VYVirtualNetRequest<>>
        class MCVNESimulator :
        public VNEMCTSSimulator <Network<VYSubstrateNode<>,VYSubstrateLink<>>, VYVirtualNetRequest<>>
        {
        public:
            MCVNESimulator(std::shared_ptr<SUBSTRATE_TYPE> subs_net, std::shared_ptr<VNR_TYPE> vnr,
                           std::shared_ptr<LinkEmbeddingAlgorithm <SUBSTRATE_TYPE,VNR_TYPE>> _link_embedder);
            
            ~MCVNESimulator();
            
        protected:
            virtual std::shared_ptr<std::set<int>> getValidSubstrateNodeIdSetForVNNodeId (int vn_id, std::shared_ptr<std::set<int>> used_sn_ids) const override;
            virtual double calculateImmediateReward (std::shared_ptr<VNENMState> st, int action) const override;
            virtual double calculateFinalReward (std::shared_ptr<VNENMState> st,
                                                 const std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap) const override;
        private:
            struct ReachabilityCondition
            {
                bool operator()(const std::shared_ptr<const VYSubstrateNode<>> lhs, const std::shared_ptr<const std::vector<std::shared_ptr<VYSubstrateLink<>>> > linksConnectedToLhs, const std::shared_ptr<const VYVirtualNode<>> rhs, const std::shared_ptr<const std::vector<std::shared_ptr<VYVirtualLink<>>> > linksConnectedToRhs, double maxD, std::shared_ptr<std::set<int>> used_sn_ids) const
                {
                    double sum_sn_link_bw = 0.0;
                    double sum_vn_link_bw = 0.0;
                    for (auto it = linksConnectedToLhs->begin(); it != linksConnectedToLhs->end(); it++)
                    {
                        sum_sn_link_bw += (*it)->getBandwidth();
                    }
                    for (auto it = linksConnectedToRhs->begin(); it != linksConnectedToRhs->end(); it++)
                    {
                        sum_vn_link_bw += (*it)->getBandwidth();
                    }
                    return (sum_sn_link_bw >= sum_vn_link_bw && used_sn_ids->find(lhs->getId()) == used_sn_ids->end() && lhs->getCoordinates().distanceFrom(rhs->getCoordinates())<=maxD &&
                            lhs->getCPU()>=rhs->getCPU() );
                }
            };
        };
    }
}
#endif /* defined(__vne_mcts__mcvne_simulator__) */
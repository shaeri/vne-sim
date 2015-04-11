/**
 * @file vne-mcts-simulator.h
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

#ifndef MCVNE_VNE_MCTS_SIMULATOR_
#define MCVNE_VNE_MCTS_SIMULATOR_

#include "mcts/tree-node.h"
#include "mcts/mcts-simulator.h"
#include "mcvne/vne-nm-state.h"

#include "core/rng.h"

#include "core/network.h"
#include "core/substrate-link.h"
#include "core/substrate-node.h"
#include "core/virtual-network-request.h"
#include "core/config-manager.h"
#include "core/link-embedding-algorithm.h"

using namespace vne::mcts;

namespace vne{
    namespace mcvne{
        
        template<typename,typename> class VNEMCTSSimulator;
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        class VNEMCTSSimulator <Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>> : public MCTSSimulator
        {
            static_assert (std::is_base_of<SubstrateNode<SNODERES...>, SNODECLASS<SNODERES...>>::value,
                           "Template arguments are not correctly set.");
            static_assert (std::is_base_of<VirtualNode<VNODERES...>, VNODECLASS<VNODERES...>>::value,
                           "Template arguments are not correctly set.");
            static_assert (std::is_base_of<SubstrateLink<SLINKRES...>, SLINKCLASS<SLINKRES...>>::value,
                           "Template arguments are not correctly set.");
            static_assert (std::is_base_of<VirtualLink<VLINKRES...>, VLINKCLASS<VLINKRES...>>::value,
                           "Template arguments are not correctly set.");
            static_assert (std::is_base_of<VirtualNetworkRequest<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>, VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>::value, "Template arguments are not correctly set.");
        public:
            
            typedef VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>> VNR_TYPE;
            typedef Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>> SUBSTRATE_TYPE;
            
            virtual ~VNEMCTSSimulator () {};
            
            virtual std::shared_ptr<State> createStartState () const override;
            
            // Update state according to action, and get observation and reward.
            // Return value of true indicates termination of episode (if episodic)
            // Action is the ID of the substrate node that will be used to host the current VNR node.
            virtual bool step (std::shared_ptr<State> state, int action, double& reward) const override;
            
            // Sanity check
            virtual bool validate(const std::shared_ptr<State> state) const override;
            
            
            // Generate set of legal actions
            virtual void generateLegal(const std::shared_ptr<State> state, const std::vector<int>& history,
                                       std::vector<int>& actions, const Status& status) const override;
            
            // Generate set of preferred actions
            virtual void generatePreferred(const std::shared_ptr<State> state, const std::vector<int>& history,
                                           std::vector<int>& actions, const Status& status) const override;

        protected:
            
            VNEMCTSSimulator (std::shared_ptr<SUBSTRATE_TYPE> subs_net, std::shared_ptr<VNR_TYPE> vnr,
                              std::shared_ptr<LinkEmbeddingAlgorithm <SUBSTRATE_TYPE,VNR_TYPE>> _link_embedder);

            virtual std::shared_ptr<std::set<int>> getValidSubstrateNodeIdSetForVNNodeId (int vn_id, const std::shared_ptr<std::set<int>> used_sn_ids) const = 0;
            virtual double calculateImmediateReward (std::shared_ptr<VNENMState> st, int action) const = 0;
            virtual double calculateFinalReward (std::shared_ptr<VNENMState> st,
                    const std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<SLINKRES...>>>>>* linkMap) const = 0;
            std::shared_ptr<LinkEmbeddingAlgorithm <SUBSTRATE_TYPE,VNR_TYPE>> link_embedder;
            std::shared_ptr<SUBSTRATE_TYPE> substrate_net;
            std::shared_ptr<VNR_TYPE> vnr;
            
            bool setAlpha;
            bool setBeta;
        private:
            bool isActionLegal (int action, std::shared_ptr<VNENMState> st) const;
        };
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        inline bool
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::isActionLegal (int action, std::shared_ptr<VNENMState> st) const
        {
            std::shared_ptr<std::set<int>> validActionSet =
                getValidSubstrateNodeIdSetForVNNodeId(st->getCurrentVNId(),st->getUsedSNIds());
            bool ret = validActionSet->find(action) != validActionSet->end();
            return ret;
        }
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::VNEMCTSSimulator (std::shared_ptr<VNEMCTSSimulator::SUBSTRATE_TYPE> _substrate_net,
                            std::shared_ptr<VNEMCTSSimulator::VNR_TYPE> _vnr,
                            std::shared_ptr<LinkEmbeddingAlgorithm <SUBSTRATE_TYPE,VNR_TYPE>> _link_embedder)
        : MCTSSimulator (_substrate_net->getNumNodes()),
          substrate_net(_substrate_net),
          vnr (_vnr),
          link_embedder (_link_embedder),
          setAlpha(ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setAlpha")),
          setBeta(ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setBeta"))
        {
        }
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        std::shared_ptr<State>
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::createStartState () const
        {
            const std::shared_ptr<std::set<int>> VNNodeIdSet = vnr->getVN()->getNodeIdSet();
            std::shared_ptr<VNENMState> st (new VNENMState
                                            (VNNodeIdSet, vnr->getId(), vnr->getNodeMap()));
            return st;
        }
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        bool
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::step
         (std::shared_ptr<State> state, int action, double& reward) const
        {
            std::shared_ptr<VNENMState> st = std::static_pointer_cast<VNENMState>(state);
            
            if (!isActionLegal(action,st))
            {
                reward = -Infinity;
                return true;
            }
            
            st->addNodeMapping(action);
            
            if (st->isTreminal())
            {
                std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<SLINKRES...>>>>> linkMap;
                Embedding_Result result = link_embedder->embeddVNRLinksForIdSets(substrate_net,vnr,st->getNodeMap(),&linkMap);
                if (result != Embedding_Result::SUCCESSFUL_EMBEDDING)
                    reward = -Infinity;
                else
                    reward = calculateFinalReward (st, &linkMap);
                return true;
            }
            else
                reward += calculateImmediateReward(st, action);
            return false;
        }
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        bool
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::validate
        (const std::shared_ptr<State> state) const
        {
            std::shared_ptr<VNENMState> st = std::static_pointer_cast<VNENMState>(state);
            return (vnr->getId()==st->getVNRId());
            
        }
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        void
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::generateLegal
        (const std::shared_ptr<State> state, const std::vector<int>& history,
                        std::vector<int>& actions, const Status& status) const
        {
            std::shared_ptr<VNENMState> st = std::static_pointer_cast<VNENMState>(state);
            std::shared_ptr<std::set<int>> validActionSet =
                getValidSubstrateNodeIdSetForVNNodeId(st->getCurrentVNId(), st->getUsedSNIds());
            actions = std::vector<int>
                (validActionSet->begin(), validActionSet->end());
        }
        
        template<
        typename ... SNODERES, template <typename ...> class SNODECLASS,
        typename... SLINKRES, template <typename...> class SLINKCLASS,
        typename ... VNODERES, template <typename ...> class VNODECLASS,
        typename... VLINKRES, template <typename...> class VLINKCLASS,
        template<typename> class VNRCLASS>
        void
        VNEMCTSSimulator<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
        VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
        ::generatePreferred
        (const std::shared_ptr<State> state, const std::vector<int>& history,
                            std::vector<int>& actions, const Status& status) const
        {
            generateLegal(state, history, actions, status);
        }
    }
}
#endif

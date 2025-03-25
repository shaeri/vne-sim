/**
 * @file mcts-simulator.h
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

#include "mcts-simulator.h"
#include "core/config-manager.h"

#include <math.h>

namespace vne
{
namespace mcts
{

    MCTSSimulator::Knowledge::Knowledge()
        :

          TreeLevel(ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator",
                                                              "Knowledge", "TreeLevel")),
          RolloutLevel(ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator",
                                                                 "Knowledge", "RolloutLevel")),
          SmartTreeCount(ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator",
                                                                   "Knowledge.SmartTreeCount")),
          SmartTreeValue(ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator",
                                                                      "Knowledge", "SmartTreeValue"))
    {
    }

    MCTSSimulator::Status::Status()
        :

          Phase(TREE)
    {
    }

    MCTSSimulator::MCTSSimulator()
        :

          knowledge(Knowledge()),
          discount(ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator", "discount")),
          rewardRange(
              ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator", "rewardRange"))
    {
        assert(discount > 0 && discount <= 1);
    }

    MCTSSimulator::MCTSSimulator(int _numActions)
        :

          numActions(_numActions),
          knowledge(Knowledge()),
          discount(ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator", "discount")),
          rewardRange(
              ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator", "rewardRange"))
    {
        assert(discount > 0 && discount <= 1);
    }

    MCTSSimulator::~MCTSSimulator() {}

    inline int MCTSSimulator::Random(int max) const
    {
        return (int)gsl_rng_uniform_int(RNG::Instance()->getGeneralRNG(), max);
    }

    void MCTSSimulator::generateLegal(const std::shared_ptr<State> state,
                                      const std::vector<int> &history, std::vector<int> &actions,
                                      const Status &status) const
    {
        for (int a = 0; a < numActions; ++a)
            actions.push_back(a);
    }

    void MCTSSimulator::generatePreferred(const std::shared_ptr<State> state,
                                          const std::vector<int> &history,
                                          std::vector<int> &actions, const Status &status) const
    {
    }

    int MCTSSimulator::selectRandom(const std::shared_ptr<State> state,
                                    const std::vector<int> &history, const Status &status) const
    {
        static std::vector<int> actions;

        if (knowledge.RolloutLevel >= Knowledge::SMART) {
            actions.clear();
            generatePreferred(state, history, actions, status);
            if (!actions.empty())
                return actions[Random((int)actions.size())];
        }

        if (knowledge.RolloutLevel >= Knowledge::LEGAL) {
            actions.clear();
            generateLegal(state, history, actions, status);
            if (!actions.empty())
                return actions[Random((int)actions.size())];
        }

        return Random(numActions);
    }

    void MCTSSimulator::prior(std::shared_ptr<TreeNode> node, const std::vector<int> &history,
                              const Status &status) const
    {
        static std::vector<int> actions;
        if (knowledge.TreeLevel == Knowledge::PURE || node->getState() == nullptr) {
            node->setChildren(0, 0);
            return;
        } else {
            node->setChildren(+LargeInteger, -Infinity);
        }

        if (knowledge.TreeLevel >= Knowledge::LEGAL) {
            actions.clear();
            generateLegal(node->getState(), history, actions, status);

            for (auto i_action = actions.begin(); i_action != actions.end(); ++i_action) {
                int a = *i_action;
                std::shared_ptr<TreeNode> node_child = node->child(a);
                node_child->value.set(0, 0);
                node_child->AMAF.set(0, 0);
            }
        }

        if (knowledge.TreeLevel >= Knowledge::SMART) {
            actions.clear();
            generatePreferred(node->getState(), history, actions, status);

            for (auto i_action = actions.begin(); i_action != actions.end(); ++i_action) {
                int a = *i_action;
                std::shared_ptr<TreeNode> node_child = node.get()->child(a);
                node_child->value.set(knowledge.SmartTreeCount, knowledge.SmartTreeValue);
                node_child->AMAF.set(knowledge.SmartTreeCount, knowledge.SmartTreeValue);
            }
        }
    }

    double MCTSSimulator::getHorizon(double accuracy, int undiscountedHorizon) const
    {
        if (discount == 1)
            return undiscountedHorizon;
        return log(accuracy) / log(discount);
    }
}  // namespace mcts
}  // namespace vne

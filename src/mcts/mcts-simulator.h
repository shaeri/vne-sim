/**
 * @file mcts-simulator.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date 7/16/14
 *
 * @note This class is based on the POMCP implementation by David Silver and Joel Veness.
 * POMCP is published in NIPS 2010: "Online Monte-Carlo Plannin/g in Large POMDPs".
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

#ifndef MCTS_MCTS_SIMULATOR_
#define MCTS_MCTS_SIMULATOR_

#include "mcts/tree-node.h"
#include "mcts/state.h"

#include "core/rng.h"

#include "core/network.h"
#include "core/substrate-link.h"
#include "core/substrate-node.h"
#include "core/virtual-network-request.h"

#include "core/core-types.h"

namespace vne
{
namespace mcts
{
    class MCTSSimulator : public RNGSubscriber
    {
       public:
        struct Knowledge {
            enum { PURE, LEGAL, SMART, NUM_LEVELS };

            Knowledge();

            int RolloutLevel;
            int TreeLevel;
            int SmartTreeCount;
            double SmartTreeValue;

            int Level(int phase) const
            {
                assert(phase < Status::NUM_PHASES);
                if (phase == Status::TREE)
                    return TreeLevel;
                else
                    return RolloutLevel;
            }
        };

        struct Status {
            Status();

            enum { TREE, ROLLOUT, NUM_PHASES };

            int Phase;
        };

        virtual std::shared_ptr<State> createStartState() const = 0;

        // Update state according to action, and get observation and reward.
        // Return value of true indicates termination of episode (if episodic)
        // Action is the ID of the substrate node that will be used to host the current VNR node.
        virtual bool step(std::shared_ptr<State> state, int action, double &reward) const = 0;

        // Sanity check
        virtual bool validate(const std::shared_ptr<State> state) const { return true; };

        // Modify state stochastically to some related state
        virtual bool localMove(std::shared_ptr<State> state, const std::vector<int> &history,
                               const Status &status) const
        {
            return true;
        };

        // Use domain knowledge to assign prior value and confidence to actions
        // Should only use fully observable state variables
        void prior(std::shared_ptr<TreeNode> node, const std::vector<int> &history,
                   const Status &status) const;

        // Use domain knowledge to select actions stochastically during rollouts
        int selectRandom(const std::shared_ptr<State> state, const std::vector<int> &history,
                         const Status &status) const;

        // Generate set of legal actions
        virtual void generateLegal(const std::shared_ptr<State> state,
                                   const std::vector<int> &history, std::vector<int> &actions,
                                   const Status &status) const;

        // Generate set of preferred actions
        virtual void generatePreferred(const std::shared_ptr<State> state,
                                       const std::vector<int> &history, std::vector<int> &actions,
                                       const Status &status) const;

        virtual ~MCTSSimulator();

        int getNumActions() { return numActions; };
        double getDiscount() const { return discount; }
        double getRewardRange() const { return rewardRange; }
        double getHorizon(double accuracy, int undiscountedHorizon = 100) const;

       protected:
        MCTSSimulator();
        MCTSSimulator(int numActions);

        int numActions;
        Knowledge knowledge;
        double discount, rewardRange;
        inline int Random(int max) const;
    };
}  // namespace mcts
}  // namespace vne
#endif

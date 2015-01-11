/**
 * @file mcts.h
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

#ifndef MCTS_MCTS_
#define MCTS_MCTS_


#include "mcts/tree-node.h"

#include "mcts/mcts-simulator.h"
#include "mcts/mcts-statistics.h"

namespace vne{
    namespace mcts{

        class MCTS {
        public:
            
            struct Parameters
            {
                Parameters();
                
                int MaxDepth;
                int NumSimulations;
                /** How many nodes to add at each expansion step */
                int ExpandCount;
                bool AutoExploration;
                /** One option is to set Exploration Constant c = R_{hi}-R_{lo}*/
                double ExplorationConstant;
                bool UseRave;
                double RaveDiscount;
                double RaveConstant;
                /** When set, the baseline rollout algorithm is run. */
                bool DisableTree;
            };
            
            MCTS(std::shared_ptr<MCTSSimulator> sim);
            ~MCTS();
            
            int selectAction();
            bool update(int action, double reward);
            
            void UCTSearch();
            void rolloutSearch();
            
            double rollout(std::shared_ptr<State> st);
            
            const MCTSSimulator::Status& getStatus() const { return status; }
            
            static void initFastUCB(double exploration);
            
        private:
            
            const std::shared_ptr<MCTSSimulator> simulator;
            std::vector<int> history;
            int treeDepth, peakTreeDepth;
            Parameters params;
            std::shared_ptr<TreeNode> root;
            MCTSSimulator::Status status;
            
            void clearStatistics();
            MCTSStatistics statTreeDepth;
            MCTSStatistics statRolloutDepth;
            MCTSStatistics statTotalReward;
            
            int greedyUCB(std::shared_ptr<TreeNode> node, bool ucb) const;
            int selectRandom() const;
            double simulateNode(std::shared_ptr<TreeNode> node);
            void addRave(std::shared_ptr<TreeNode> node, double totalReward);
            std::shared_ptr<TreeNode> expandNode(const std::shared_ptr<State> state);
            
            // Fast lookup table for UCB
            static const int UCB_N = 10000, UCB_n = 100;
            static double UCB[UCB_N][UCB_n];
            static bool initialisedFastUCB;
            
            double fastUCB(int N, int n, double logN) const;
        };
    }
}
#endif
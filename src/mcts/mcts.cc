/**
 * @file mcts.cc
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

#include "mcts.h"
#include "core/config-manager.h"
#include "utilities/logger.h"

#include <math.h>
#include <algorithm>

using namespace std;

namespace vne {
    namespace mcts{
        MCTS::Parameters::Parameters()
        :
        MaxDepth(ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.MaxDepth")),
        NumSimulations(ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.NumSimulations")),
        ExpandCount(ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.ExpandCount")),
        AutoExploration(ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.AutoExploration")),
        ExplorationConstant(ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.ExplorationConstant")),
        UseRave(ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseRave")),
        RaveDiscount(ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveDiscount")),
        RaveConstant(ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveConstant")),
        DisableTree(ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.DisableTree")),
        UseSinglePlayerMCTS(ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseSinglePlayerMCTS")),
        SPMCTSConstant(ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.SPMCTSConstant"))
        {
        }
        MCTS::MCTS(const std::shared_ptr<MCTSSimulator> sim)
        :
        simulator(sim),
        params(Parameters()),
        treeDepth(0),
        history(vector<int>())
        {
            if (params.AutoExploration)
            {
                if (params.UseRave) {
                    params.ExplorationConstant = 0;
                }
                else
                    params.ExplorationConstant = simulator->getRewardRange();
            }
            initFastUCB(params.ExplorationConstant);
            TreeNode::NumChildren = simulator->getNumActions();
            root = expandNode(simulator->createStartState());
        }

        MCTS::~MCTS()
        {
        }

        bool MCTS::update(int action, double reward)
        {
            BOOST_LOG_TRIVIAL(debug) << "Roots value: " << root->value.getValue() << std::endl;
        	history.push_back(action);
            // Find matching vnode from the rest of the tree
            std::shared_ptr<TreeNode> child_node = root->child(action);
            BOOST_LOG_TRIVIAL(debug) << "Child's node value: " << child_node->value.getValue() <<std::endl;
            // Delete old tree and create new root
            //const std::shared_ptr<State> st = root->getState()->getCopy();
            //double dummyReward;
            //simulator->step(st, action, dummyReward);
            //std::shared_ptr<TreeNode> newRoot = expandNode(st);
            *root = *child_node;
            return true;
        }

        int MCTS::selectAction()
        {
            if (params.DisableTree)
                rolloutSearch();
            else
                UCTSearch();
            return greedyUCB(root, false);
        }

        void MCTS::rolloutSearch()
        {
            //std::vector<double> totals(simulator->getNumActions(), 0.0);
            int historyDepth = (int) history.size();
            std::vector<int> legal;
            assert(root->getState() != nullptr);
            simulator->generateLegal(root->getState(), history, legal, status);
            random_shuffle(legal.begin(), legal.end());
            
            for (int i = 0; i < params.NumSimulations; i++)
            {
                int action = legal[i % legal.size()];
                std::shared_ptr<State> st = root->getState()->getCopy();
                simulator->validate(st);
                
                double immediateReward, delayedReward, totalReward;
                bool terminal = simulator->step(st, action, immediateReward);
                
                std::shared_ptr<TreeNode> node = root->child(action);
                if (node->getState()==nullptr && !terminal)
                {
                    *node = *(expandNode(st));
                }
               
                history.push_back(action);
                delayedReward = rollout(st->getCopy());
                totalReward = immediateReward + (simulator->getDiscount() * delayedReward);
                root->child(action)->value.add(totalReward);
                st.reset();
                history.resize(historyDepth);
            }
        }

        void MCTS::UCTSearch()
        {
            clearStatistics();
            
            int historyDepth = (int) history.size();
            
            for (int n = 0; n < params.NumSimulations; n++)
            {
                status.Phase = MCTSSimulator::Status::TREE;
                
                treeDepth = 0;
                peakTreeDepth = 0;
                
                double totalReward = simulateNode(root);
                root->value.add(totalReward);
                //addRave(root, totalReward);
                statTotalReward.Add(totalReward);
                statTreeDepth.Add(peakTreeDepth);
                
                history.resize(historyDepth);
            }
        }
        
        double MCTS::simulateNode(std::shared_ptr<TreeNode> node)
        {
            double immediateReward, delayedReward = 0;
            
            int action = greedyUCB(node, true);
            
            peakTreeDepth = treeDepth;
            
            if (treeDepth >= params.MaxDepth) // search horizon reached
                return 0;
            
            std::shared_ptr<State> st = node->getState()->getCopy();
            bool terminal = simulator->step(st, action, immediateReward);
            
            history.push_back(action);
            
            std::shared_ptr<TreeNode> child_node = node->child(action);
            
            if (child_node->getState()==nullptr && !terminal && child_node->value.getCount() >= params.ExpandCount)
                *child_node = *(expandNode(st));
            
            if (!terminal)
            {
                treeDepth++;
                if (!(child_node->getState()==nullptr))
                    delayedReward = simulateNode(child_node);
                else
                    delayedReward = rollout(st->getCopy());
                treeDepth--;
            }
            
            double totalReward = immediateReward + simulator->getDiscount() * delayedReward;
            
            child_node->value.add(totalReward);
            addRave(node, totalReward);
            return totalReward;
        }
        
        void MCTS::addRave(std::shared_ptr<TreeNode> node, double totalReward)
        {
            double totalDiscount = 1.0;
            for (int t = treeDepth; t < history.size(); ++t)
            {
                std::shared_ptr<TreeNode> child_node = node->child(history[t]);
                child_node->AMAF.add(totalReward, totalDiscount);
                totalDiscount *= params.RaveDiscount;
            }
        }
        
        std::shared_ptr<TreeNode> MCTS::expandNode(const std::shared_ptr<State> state)
        {
            std::shared_ptr<TreeNode> node (new TreeNode(state));
            node->value.set(0,0);
            simulator->prior(node, history, status);
            return node;
        }

        int MCTS::greedyUCB(std::shared_ptr<TreeNode> node, bool ucb) const
        {
            static vector<int> besta;
            besta.clear();
            double bestq = -Infinity;
            int N = node->value.getCount();
            double logN = log(N + 1);
            
            for (int action = 0; action < simulator->getNumActions(); action++)
            {
                double q;
                int n;
                
                std::shared_ptr<TreeNode> child_node = node->child(action);
                q = child_node->value.getValue();
                n = child_node->value.getCount();
                
                if (params.UseRave && child_node->AMAF.getCount() > 0)
                {
                    double n2 = child_node->AMAF.getCount();
                    double beta = n2 / (n + n2 + params.RaveConstant * n * n2);
                    q = (1.0 - beta) * q + beta * child_node->AMAF.getValue();
                }
                
                if (ucb)
                    q += fastUCB(N, n, logN);
                
                if (params.UseSinglePlayerMCTS && child_node->value.getCount() > 0
                     && child_node->value.getValue()> -Infinity)
                    q += sqrt( (child_node->value.getSumSquaredValue() - n * child_node->value.getValue() * child_node->value.getValue() + params.SPMCTSConstant) / n );
                
                if (q >= bestq)
                {
                    if (q > bestq)
                        besta.clear();
                    bestq = q;
                    besta.push_back(action);
                }
            }
            
            assert(!besta.empty());
            int randomIndex = (int) gsl_rng_uniform_int (RNG::Instance()->getGeneralRNG(), besta.size());
            return besta[randomIndex];
        }

        double MCTS::rollout(std::shared_ptr<State> state)
        {
            status.Phase = MCTSSimulator::Status::ROLLOUT;
            
            double totalReward = 0.0;
            double discount = 1.0;
            bool terminal = false;
            int numSteps;
            for (numSteps = 0; numSteps + treeDepth < params.MaxDepth && !terminal; ++numSteps)
            {
                double reward;
                
                int action = simulator->selectRandom(state, history, status);
                terminal = simulator->step(state, action, reward);
                
                totalReward += reward * discount;
                discount *= simulator->getDiscount();
            }
            
            statRolloutDepth.Add(numSteps);
            
            return totalReward;
        }

       /*double MCTS::rollout(std::shared_ptr<TreeNode> node, std::shared_ptr<State> state)
        {
            status.Phase = MCTSSimulator::Status::ROLLOUT;
            
            double totalReward = 0.0;
            double discount = 1.0;
            bool terminal = false;
            int numSteps;
            for (numSteps = 0; numSteps + treeDepth < params.MaxDepth && !terminal; ++numSteps)
            {
                double reward;
                
                int action = simulator->selectRandom(state, history, status);
                terminal = simulator->step(state, action, reward);
                node->state = state;
                totalReward += reward * discount;
                discount *= simulator->getDiscount();
            }
            
            statRolloutDepth.Add(numSteps);
            
            return totalReward;
        }
        */
        double MCTS::UCB[UCB_N][UCB_n];
        bool MCTS::initialisedFastUCB = false;

        void MCTS::initFastUCB(double exploration)
        {
            for (int N = 0; N < UCB_N; ++N)
                for (int n = 0; n < UCB_n; ++n)
                    if (n == 0)
                        UCB[N][n] = Infinity;
                    else
                        UCB[N][n] = exploration * sqrt(log(N + 1) / n);
            initialisedFastUCB = true;
        }

        inline double MCTS::fastUCB(int N, int n, double logN) const
        {
            if (initialisedFastUCB && N < UCB_N && n < UCB_n)
                return UCB[N][n];
            
            if (n == 0)
                return Infinity;
            else
                return params.ExplorationConstant * sqrt(logN / n);
        }

        void MCTS::clearStatistics()
        {
            statTreeDepth.Clear();
            statRolloutDepth.Clear();
            statTotalReward.Clear();
        }
    }
}

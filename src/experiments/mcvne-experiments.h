/**
 * @file mcvne-experiments.h
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

#ifndef EXPERIMENTS_MCVNE_EXPERIMENTS_
#define EXPERIMENTS_MCVNE_EXPERIMENTS_

#include "core/experiment.h"

#include "mcvne/mcvne-node-embedding-algo.h"

#include "Vineyard/vy-vine-two-stage-embedding-algo.h"
#include "Vineyard/vy-substrate-network-builder.h"
#include "Vineyard/vy-vnr-proc-digraph.h"
#include "Vineyard/vy-statistics.h"

#include "hiberlite.h"

using namespace vne::vineyard;
using namespace vne::mcvne;

namespace vne {
    namespace experiments {
        /**************************************************************
         * This is MCVNE with MCF link embedding
         ***************************************************************/
        template<typename = VYVNRProcDigraph<>>
        class MCVNENodeMCFLinkExp : public Experiment<VYVNRProcDigraph<>>
        {
            friend class hiberlite::access;
            template<class Archive>
            void hibernate(Archive & ar)
            {
                ar & HIBERLITE_BASE_CLASS_WITH_NAME(vne::Experiment<VYVNRProcDigraph<>>,Experiment);
                ar & HIBERLITE_NVP(setAlpha);
                ar & HIBERLITE_NVP(setBeta);
                ar & HIBERLITE_NVP(statistics);
                ar & HIBERLITE_NVP (mcts_num_simulations);
                ar & HIBERLITE_NVP (mcts_max_depth);
                ar & HIBERLITE_NVP (mcts_use_rave);
                ar & HIBERLITE_NVP (mcts_use_sp_mcts);
                ar & HIBERLITE_NVP (mcts_expand_count);
                ar & HIBERLITE_NVP (mcts_exploration_constant);
                ar & HIBERLITE_NVP (mcts_rave_constant);
                ar & HIBERLITE_NVP (mcts_rave_discount);
                ar & HIBERLITE_NVP (mcts_discount);
            }
        public:
            MCVNENodeMCFLinkExp ();
            virtual void statisticsGenerated (Statistics& stat);
            virtual ~MCVNENodeMCFLinkExp ();
            
        protected:
            //set Alpha
            int setAlpha;
            //setBeta
            int setBeta;
            int mcts_num_simulations;
            int mcts_max_depth;
            int mcts_use_rave;
            int mcts_use_sp_mcts;
            int mcts_expand_count;
            double mcts_exploration_constant;
            double mcts_rave_constant;
            double mcts_rave_discount;
            double mcts_discount;
            double mcts_sp_constant;
            
        private:
            VYVNRProcDigraph<>* graph;
            std::shared_ptr<VYSubstrateNetworkBuilder<>> sb;
            std::list<VYStatistics> statistics;
        };
        
        /**************************************************************
        * This is MCVNE with BFS based shortest path link embedding
        ***************************************************************/
        template<typename = VYVNRProcDigraph<>>
        class MCVNENodeBFSLinkExp : public Experiment<VYVNRProcDigraph<>>
        {
            friend class hiberlite::access;
            template<class Archive>
            void hibernate(Archive & ar)
            {
                ar & HIBERLITE_BASE_CLASS_WITH_NAME(vne::Experiment<VYVNRProcDigraph<>>,Experiment);
                ar & HIBERLITE_NVP (statistics);
                ar & HIBERLITE_NVP (mcts_num_simulations);
                ar & HIBERLITE_NVP (mcts_max_depth);
                ar & HIBERLITE_NVP (mcts_use_rave);
                ar & HIBERLITE_NVP (mcts_use_sp_mcts);
                ar & HIBERLITE_NVP (mcts_expand_count);
                ar & HIBERLITE_NVP (mcts_exploration_constant);
                ar & HIBERLITE_NVP (mcts_rave_constant);
                ar & HIBERLITE_NVP (mcts_rave_discount);
                ar & HIBERLITE_NVP (mcts_discount);
                                   
            }
        public:
            MCVNENodeBFSLinkExp ();
            virtual void statisticsGenerated (Statistics& stat);
            virtual ~MCVNENodeBFSLinkExp ();
            
        protected:
            int mcts_num_simulations;
            int mcts_max_depth;
            int mcts_use_rave;
            int mcts_use_sp_mcts;
            int mcts_expand_count;
            double mcts_exploration_constant;
            double mcts_rave_constant;
            double mcts_rave_discount;
            double mcts_discount;
            double mcts_sp_constant;
        
        private:
            VYVNRProcDigraph<>* graph;
            std::shared_ptr<VYSubstrateNetworkBuilder<>> sb;
            std::list<VYStatistics> statistics;
        };
    }
}
#endif
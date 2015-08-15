/**
 * @file vineyard-experiments.h
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

#ifndef EXPERIMENTS_VINEYARD_EXPERIMENTS_
#define EXPERIMENTS_VINEYARD_EXPERIMENTS_

#include "core/experiment.h"

#include "Vineyard/vy-vine-two-stage-embedding-algo.h"
#include "Vineyard/vy-substrate-network-builder.h"
#include "Vineyard/vy-vnr-proc-digraph.h"
#include "Vineyard/vy-statistics.h"

#include "hiberlite/include/hiberlite.h"

using namespace vne::vineyard;

namespace vne {
    namespace experiments {
        template<typename = VYVNRProcDigraph<>>
        class VineNodeMCFLinkExp : public Experiment<VYVNRProcDigraph<>>
        {
           friend class hiberlite::access;
            template<class Archive>
            void hibernate(Archive & ar)
            {
                ar & HIBERLITE_BASE_CLASS_WITH_NAME(vne::Experiment<VYVNRProcDigraph<>>,Experiment);
                ar & HIBERLITE_NVP(node_embedding_type);
                ar & HIBERLITE_NVP(setAlpha);
                ar & HIBERLITE_NVP(setBeta);
                ar & HIBERLITE_NVP(statistics);
            }
        public:
            VineNodeMCFLinkExp ();
            virtual void statisticsGenerated (Statistics& stat);
            virtual ~VineNodeMCFLinkExp ();
            
        protected:
            //deterministic or
            std::string node_embedding_type;
            //set Alpha
            int setAlpha;
            //setBeta
            int setBeta;
        private:
            VYVNRProcDigraph<>* graph;
            std::shared_ptr<VYSubstrateNetworkBuilder<>> sb;
            std::list<VYStatistics> statistics;
        };
    }
}
#endif

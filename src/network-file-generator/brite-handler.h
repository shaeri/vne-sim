/**
 * @file brite-handler.h
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
/****************************************************************************/
/*                  Copyright 2001, Trustees of Boston University.          */
/*                               All Rights Reserved.                       */
/*                                                                          */
/* Permission to use, copy, or modify this software and its documentation   */
/* for educational and research purposes only and without fee is hereby     */
/* granted, provided that this copyright notice appear on all copies and    */
/* supporting documentation.  For any other uses of this software, in       */
/* original or modified form, including but not limited to distribution in  */
/* whole or in part, specific prior permission must be obtained from Boston */
/* University.  These programs shall not be used, rewritten, or adapted as  */
/* the basis of a commercial software or hardware product without first     */
/* obtaining appropriate licenses from Boston University.  Boston University*/
/* and the author(s) make no representations about the suitability of this  */
/* software for any purpose.  It is provided "as is" without express or     */
/* implied warranty.                                                        */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*  Author:     Alberto Medina                                              */
/*              Anukool Lakhina                                             */
/*  Title:     BRITE: Boston university Representative Topology gEnerator   */
/*  Revision:  2.0         4/02/2001                                        */
/****************************************************************************/

#ifndef  NFG_BRITE_HANDLER_
#define  NFG_BRITE_HANDLER_

#include "Brite.h"
#include "network-file-generator.h"

#include "core/network.h"
#include "core/rng.h"
#include "core/config-manager.h"

#include "utilities/logger.h"

#include "Vineyard/vy-substrate-link.h"
#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-link.h"
#include "Vineyard/vy-virtual-node.h"

using namespace vne::vineyard;

namespace vne{
    namespace nfg {
        class BriteHandler
        {
        public:
            struct Parameters {
                Parameters ();
                std::string BriteSeedFile;
                int nodePlacement;
                int numNeighbors;
                int innerGridSize;
                int outerGridSize;
                
                struct RTWaxman {
                    RTWaxman ();
                    int growthType;
                    double alpha;
                    double beta;
                } rtWaxman;
                
                boost::property_tree::ptree pt;
            };
            
            const Parameters& getParams () const;
            
            static std::shared_ptr<BriteHandler> Instance();
            
            //bw and cpu types: Constant = 1, Uniform =2, HeavyTailed = 3, Exponential =4
            template<typename A, typename B>
            std::shared_ptr<Network<A, B>> getNetwork_RTWaxman
            (int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3);
        protected:
            BriteHandler ();
            
        private:
            static std::shared_ptr<BriteHandler> _instance;
            Parameters params;
            template<typename A, typename B>
            std::shared_ptr<Network<A, B>>
            createNetFromBRITETopo (std::shared_ptr<Topology> t,
                            Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                            Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                            Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3);
            inline void InitSeeds ();
            //void ParseModel ();
        };
       
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> BriteHandler::getNetwork_RTWaxman
        (int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
        {
            std::shared_ptr<RouterWaxPar> par = std::shared_ptr<RouterWaxPar> (
                        new RouterWaxPar(n, params.outerGridSize , params.innerGridSize, params.nodePlacement, params.rtWaxman.growthType,
                                        min (params.numNeighbors, n), params.rtWaxman.alpha, params.rtWaxman.beta, 1, 10, 1024));
            std::shared_ptr<RouterWaxman> rt_wax_model =  std::shared_ptr<RouterWaxman> (new RouterWaxman(par.get()));
            std::shared_ptr<Topology> t = std::shared_ptr<Topology> (new Topology(rt_wax_model.get()));
            return createNetFromBRITETopo<A,B> (t, cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist, bw_param1, bw_param2, bw_param3,
                                                  delay_dist, delay_param1, delay_param2, delay_param3);
        }
        
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> BriteHandler::createNetFromBRITETopo
        (std::shared_ptr<Topology> t, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3, Distribution bw_dist,
         double bw_param1, double bw_param2, double bw_param3, Distribution delay_dist,
         double delay_param1, double delay_param2, double delay_param3)
        {
            Graph* g = t->GetGraph();
            std::shared_ptr<Network<A, B>> net (new Network<A, B>());
                            
            //we need to have the offset of the node ids because the VirtualNode Ids will be generated
            //in increasing order irrespective of the number of request.
            int nodeID_offset = IdGenerator::Instance()->peekId<A>();
            
            for (int i = 0; i < g->GetNumNodes(); i++) {
                double node_cpu = RNG::Instance()->sampleDistribution<double,double,double,double>
                    (cpu_dist, std::tuple<double,double,double> (cpu_param1, cpu_param2, cpu_param3)) ;
                std::shared_ptr<A> n (new A (node_cpu,(int) g->GetNodePtr(i)->GetNodeInfo()->GetCoordX(), g->GetNodePtr(i)->GetNodeInfo()->GetCoordY()));
                net->addNode(n);
                BOOST_LOG_TRIVIAL(debug) << g->GetNodePtr(i)->GetId() << " "
                << (int) g->GetNodePtr(i)->GetNodeInfo()->GetCoordX() << " "
                << (int) g->GetNodePtr(i)->GetNodeInfo()->GetCoordY() << std::endl;
            }
            for (auto el = g->GetEdges()->begin(); el != g->GetEdges()->end(); el++) {
                
                double link_bw = RNG::Instance()->sampleDistribution<double,double,double,double>
                    (bw_dist, std::tuple<double,double,double> (bw_param1, bw_param2, bw_param3));
                
                double link_delay = RNG::Instance()->sampleDistribution<double,double,double,double>
                    (delay_dist, std::tuple<double,double,double> (delay_param1, delay_param2, delay_param3));

                std::shared_ptr<B> l (new B (link_bw, link_delay, (*el)->GetSrc()->GetId()+nodeID_offset, (*el)->GetDst()->GetId()+nodeID_offset));
                net->addLink(l);
                 BOOST_LOG_TRIVIAL(debug) <<  " "
                << (*el)->GetSrc()->GetId() << " "
                << (*el)->GetDst()->GetId() << link_bw << link_delay <<std::endl;
            }
            return net;
        }
    }
}

#endif /* defined(__vne_mcts__brite_handler__) */

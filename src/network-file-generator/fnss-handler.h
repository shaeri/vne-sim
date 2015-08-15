/**
 * @file fnss-handler.h
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

#ifdef PYTHON_AVAILABLE

#ifndef  NFG_FNSS_HANDLER_
#define  NFG_FNSS_HANDLER_

#include "external-lib-handler.h"
#include "fnss/cpp/include/topology.h"
#include "network-file-generator.h"
#include <Python.h>

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
      template<typename A, typename B> 
      class FNSSHandler : public ExternalLibHandler<A,B>
        {
        public:
            struct Parameters {
                Parameters ();
		Topology_Type topo_type;
		struct DCNBCube
		{
		  DCNBCube ();
		  int n;
		  int k;
		} bcube;
                
		struct DCNTwoTier
		{
		  DCNTwoTier ();
		  int n_core;
		  int n_hosts;
		  int n_edges;
		} twotier;
		
		struct DCNFatTree
		{
		  DCNFatTree ();
		  int k;
		} fattree;
            };
           
            static std::shared_ptr<FNSSHandler<A,B>> Instance();
	    
	    virtual std::shared_ptr<Network<A, B>> getNetwork
            (Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3) override;

	    virtual std::string getPreferredFileName () override;
            
        protected:
            FNSSHandler ();
            
        private:
            static std::shared_ptr<FNSSHandler<A,B>> _instance;
            Parameters params;
	    
            std::shared_ptr<Network<A, B>> getNetwork_DCNBCube
            (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3); 
	    
            std::shared_ptr<Network<A, B>>
            createNetFromFNSSTopo (std::shared_ptr<fnss::Topology> t,
                            Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                            Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                            Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3);
        };
	
	template <typename A, typename B>
	std::string FNSSHandler<A,B>::getPreferredFileName ()
	{
	    return "";
	}
	
	template <typename A, typename B>
	std::shared_ptr<Network<A, B>> FNSSHandler<A,B>::getNetwork
            (Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
	{
	  if (tt == Topology_Type::DCNBCube)
	    return getNetwork_DCNBCube (cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist, bw_param1, bw_param2, bw_param3, 
					    delay_dist, delay_param1, delay_param2, delay_param3);
	    
	}
	
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> FNSSHandler<A,B>::getNetwork_DCNBCube
        (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
        {
            std::shared_ptr<RouterWaxPar> par = std::shared_ptr<RouterWaxPar> (
                        new RouterWaxPar(n, params.outerGridSize , params.innerGridSize, params.nodePlacement, params.rtWaxman.growthType,
                                        min (params.numNeighbors, n), params.rtWaxman.alpha, params.rtWaxman.beta, 1, 10, 1024));
            std::shared_ptr<RouterWaxman> rt_wax_model =  std::shared_ptr<RouterWaxman> (new RouterWaxman(par.get()));
            std::shared_ptr<Topology> t = std::shared_ptr<Topology> (new Topology(rt_wax_model.get()));
            return createNetFromBRITETopo (t, cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist, bw_param1, bw_param2, bw_param3,
                                                  delay_dist, delay_param1, delay_param2, delay_param3);
        }
        
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> BriteHandler<A,B>::createNetFromBRITETopo
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
        
        template <typename A, typename B>
        std::shared_ptr<FNSSHandler<A,B>> FNSSHandler<A,B>::_instance = nullptr;
	
	template <typename A, typename B>	
        std::shared_ptr<FNSSHandler<A,B>> FNSSHandler<A,B>::Instance ()
        {
            if (_instance==nullptr) {
                _instance = std::shared_ptr<FNSSHandler<A,B>> (new FNSSHandler<A,B>());
            }
            return _instance;
        }
        
   
        template <typename A, typename B> 
        FNSSHandler<A,B>::Parameters::DCNBCube::DCNBCube () :
        n(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNBCube.N")),
        k(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNBCube.K"))
        {
        }
        
        template <typename A, typename B> 
        FNSSHandler<A,B>::Parameters::DCNTwoTier::DCNTwoTier () :
        n_core(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNTwoTier.n_core")),
        n_edges(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNTwoTier.n_edges")),
	n_hosts(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNTwoTier.n_hosts")) 
        {
        } 
        
        template <typename A, typename B> 
        FNSSHandler<A,B>::Parameters::DCNFatTree::DCNFatTree () :
        k(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNFatTree.K"))
        {
        } 
        
        
	template<typename A, typename B>
	FNSSHandler<A,B>::Parameters::Parameters () :
        bcube (DCNBCube()),
        twotier (DCNTwoTier()),
        fattree (DCNFatTree()),
        topo_type (get_Topology_Type(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.FNSSHandler.TopoType"))
        {
        }
        
        template <typename A, typename B> 
        FNSSHandler<A,B>::FNSSHandler () :
        params(Parameters())
        {
	    this->pt.put ("Topology", get_Topology_Type_Str(params.topo_type));
	    if (params.topo_type == Topology_Type::DCNBCube)
	    {
	      this->pt.put ("DCNBCube.N", params.bcube.n);
	      this->pt.put ("DCNBCube.K", params.bcube.k);
	    }
	    else if (params.topo_type == Topology_Type::DCNFatTree) 
	      this->pt.put ("DCNFatTree.K", params.fattree.k);
	    else if (params.topo_type == Topology_Type::DCNTwoTier)
	    {
	      this->pt.put ("DCNTwoTier.n_core", params.twotier.n_core);
	      this->pt.put ("DCNTwoTier.n_edges", params.twotier.n_edges); 
	      this->pt.put ("DCNTwoTier.n_hosts", params.twotier.n_hosts);  
	    }
        }
    }
}
#endif
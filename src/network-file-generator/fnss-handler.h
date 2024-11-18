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

#ifndef  NFG_FNSS_HANDLER_
#define  NFG_FNSS_HANDLER_

#ifdef  WITH_FNSS_SUPPORT
#include "external-lib-handler.h"
//#include "network-file-generator.h"

#include "core/config-manager.h"

#include "utilities/logger.h"

#include "fnss-cpp/src/topology.h"
#include "fnss-cpp/src/edge.h"
#include "fnss-cpp/src/node.h"
#include "fnss-cpp/src/pair.h"
#include "fnss-cpp/src/parser.h"

//Handling Python header in mac os x 
#ifdef DARWIN
#include <Python.h>
#else
#include <Python.h>
#endif

using namespace vne::vineyard;
using namespace fnss;

namespace vne{
    namespace nfg {
      template<typename A, typename B> 
      class FNSSHandler : public ExternalLibHandler<A,B>
        {
        public:
            FNSSHandler ();
            virtual ~FNSSHandler () {};
            struct Parameters
            {
                Parameters ();
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
                    int coreBWMultiplier;
                } twotier;
		
                struct DCNFatTree
                {
                    DCNFatTree ();
                    int k;
                    int coreBWMultiplier;
                } fattree;
            };
            virtual std::shared_ptr<Network<A, B>> getNetwork
            (Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3) override;

            virtual std::string getPreferredFileName () override;
            
        private:
            int numHosts;
            int numSwitches;
            Topology_Type _Topology_Type;
            
            Parameters params;
            
            std::shared_ptr<Network<A, B>> getNetwork_DCNBCube
            (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3);
            
            std::shared_ptr<Network<A, B>> getNetwork_DCNTwoTier
            (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3);
            
            std::shared_ptr<Network<A, B>> getNetwork_DCNFatTree
            (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3);
        };
        
        template <typename A, typename B>
        std::string FNSSHandler<A,B>::getPreferredFileName ()
        {
            stringstream ss;
            
            if (_Topology_Type == Topology_Type::DCNBCube)
                ss << "_n_" << params.bcube.n << "_k_" << params.bcube.k << "_nHosts_" << numHosts << "_nSwitches_" << numSwitches;
            else if (_Topology_Type == Topology_Type::DCNTwoTier)
                ss << "_nCore_" << params.twotier.n_core << "_nEdges_" << params.twotier.n_edges << "_nHosts_" << params.twotier.n_hosts <<
                    "_coreMultiplier_" << params.twotier.coreBWMultiplier << "_nHosts_" << numHosts << "_nSwitches_" << numSwitches;
            else
                ss << "_k_" << params.fattree.k << "_coreMultiplier_" << params.fattree.coreBWMultiplier <<
                    "_nHosts_" << numHosts << "_nSwitches_" << numSwitches;
            
            return ss.str();
        }
	
        template <typename A, typename B>
        std::shared_ptr<Network<A, B>> FNSSHandler<A,B>::getNetwork
            (Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
        {
            _Topology_Type = tt;
            if (tt == Topology_Type::DCNBCube)
                return getNetwork_DCNBCube (cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist, bw_param1, bw_param2, bw_param3,
                                            delay_dist, delay_param1, delay_param2, delay_param3);
            else if (tt == Topology_Type::DCNTwoTier)
                return getNetwork_DCNTwoTier (cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist, bw_param1, bw_param2, bw_param3,
                                            delay_dist, delay_param1, delay_param2, delay_param3);
            else
                return getNetwork_DCNFatTree (cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist, bw_param1, bw_param2, bw_param3,
                                            delay_dist, delay_param1, delay_param2, delay_param3);
        }
	
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> FNSSHandler<A,B>::getNetwork_DCNBCube
        (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
        {
            std::stringstream pythonScript;
            Py_Initialize();
            pythonScript << "import fnss\n";
            //pythonScript << "import networkx as nx\n";
            pythonScript << "topology = " << "fnss.bcube_topology(n=" << params.bcube.n << ", k= " << params.bcube.k << ")\n";
            pythonScript << "fnss.write_topology(topology, '.datacenter_topology.xml')\n";
            PyRun_SimpleString(pythonScript.str().c_str());
            Py_Finalize();
            
            fnss::Topology t = fnss::Parser::parseTopology(".datacenter_topology.xml");
            std::set<std::pair <std::string, std::string> > edges = t.getAllEdges();
            std::set<std::string> nodes = t.getAllNodes();
            assert (nodes.size() > 0 && edges.size() > 0);
            
            std::shared_ptr<Network<A, B>> net (new Network<A, B>());
            std::map<std::string, int> fnssNodeIdToVNESimNodeId;
            
            for(set<string>::iterator it = nodes.begin(); it != nodes.end(); it++)
            {
                fnss::Node fnssNode = t.getNode(*it);

                std::shared_ptr<A> n = nullptr;
                // If the node is a host create it with a cpu capacity
                if (fnssNode.getProperty("type").compare("host") == 0)
                {
                    numHosts++;
                    double node_cpu = RNG::Instance()->sampleDistribution<double,double,double,double>
                    (cpu_dist, std::tuple<double,double,double> (cpu_param1, cpu_param2, cpu_param3)) ;
                    n.reset (new A (node_cpu, 0, 0));
                }
                else
                {
                    numSwitches++;
                    n.reset (new A (0,0,0));
                }
                fnssNodeIdToVNESimNodeId[*it] = n->getId();
                net->addNode (n);
            }
            
            for(set<pair <string, string> >::iterator it = edges.begin(); it != edges.end(); it++)
            {
                double link_bw = RNG::Instance()->sampleDistribution<double,double,double,double>
                (bw_dist, std::tuple<double,double,double> (bw_param1, bw_param2, bw_param3));
                
                double link_delay = RNG::Instance()->sampleDistribution<double,double,double,double>
                (delay_dist, std::tuple<double,double,double> (delay_param1, delay_param2, delay_param3));
                
                int nodeFromId = fnssNodeIdToVNESimNodeId[(*it).first];
                int nodeToId = fnssNodeIdToVNESimNodeId[(*it).second];
                
                std::shared_ptr<B> l (new B (link_bw, link_delay, nodeFromId, nodeToId));
                net->addLink (l);
            }
            this->pt.put ("n_switches", numSwitches);
            this->pt.put ("n_hosts", numHosts);
            this->pt.put ("n_links", net->getNumLinks());
            return net;
        }
        
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> FNSSHandler<A,B>::getNetwork_DCNTwoTier
        (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
         Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
         Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
        {
            std::stringstream pythonScript;
            Py_Initialize();
            pythonScript << "import fnss\n";
            //pythonScript << "import networkx as nx\n";
            pythonScript << "topology = " << "fnss.two_tier_topology(n_core=" << params.twotier.n_core << ", n_edge= " << params.twotier.n_edges <<
            ", n_hosts=" << params.twotier.n_hosts << ")\n";
            pythonScript << "fnss.write_topology(topology, '.datacenter_topology.xml')\n";
            PyRun_SimpleString(pythonScript.str().c_str());
            Py_Finalize();
            
            fnss::Topology t = fnss::Parser::parseTopology(".datacenter_topology.xml");
            std::set<std::pair <std::string, std::string> > edges = t.getAllEdges();
            std::set<std::string> nodes = t.getAllNodes();
            assert (nodes.size() > 0 && edges.size() > 0);
            
            std::shared_ptr<Network<A, B>> net (new Network<A, B>());
            std::map<std::string, int> fnssNodeIdToVNESimNodeId;
            
            for(set<string>::iterator it = nodes.begin(); it != nodes.end(); it++)
            {
                fnss::Node fnssNode = t.getNode(*it);
                
                std::shared_ptr<A> n = nullptr;
                // If the node is a host create it with a cpu capacity
                if (fnssNode.getProperty("type").compare("host") == 0)
                {
                    numHosts++;
                    double node_cpu = RNG::Instance()->sampleDistribution<double,double,double,double>
                    (cpu_dist, std::tuple<double,double,double> (cpu_param1, cpu_param2, cpu_param3)) ;
                    n.reset (new A (node_cpu, 0, 0));
                }
                else
                {
                    numSwitches++;
                    n.reset (new A (0,0,0));
                }
                fnssNodeIdToVNESimNodeId[*it] = n->getId();
                net->addNode (n);
            }
            
            for(set<pair <string, string> >::iterator it = edges.begin(); it != edges.end(); it++)
            {
                double link_bw = RNG::Instance()->sampleDistribution<double,double,double,double>
                (bw_dist, std::tuple<double,double,double> (bw_param1, bw_param2, bw_param3));
                
                double link_delay = RNG::Instance()->sampleDistribution<double,double,double,double>
                (delay_dist, std::tuple<double,double,double> (delay_param1, delay_param2, delay_param3));
                
                std::shared_ptr<B> l = nullptr;
                int nodeFromId = fnssNodeIdToVNESimNodeId[(*it).first];
                fnss::Node fnssNodeFrom = t.getNode ((*it).first);
                int nodeToId = fnssNodeIdToVNESimNodeId[(*it).second];
                fnss::Node fnssNodeTo = t.getNode ((*it).second);
                
                if ((fnssNodeFrom.getProperty("type").compare("host") == 0 ||  fnssNodeTo.getProperty ("type").compare("host") == 0 ))
                {
                    l.reset (new B (link_bw, link_delay, nodeFromId, nodeToId));
                }
                else
                    l.reset (new B (params.twotier.coreBWMultiplier * link_bw, link_delay, nodeFromId, nodeToId));
                net->addLink (l);
            }
            this->pt.put ("n_switches", numSwitches);
            this->pt.put ("n_hosts", numHosts);
            this->pt.put ("n_links", net->getNumLinks());
            return net;
        }
        
        template<typename A, typename B>
        std::shared_ptr<Network<A, B>> FNSSHandler<A,B>::getNetwork_DCNFatTree
        (Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
         Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
         Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
        {
            std::stringstream pythonScript;
            Py_Initialize();
            pythonScript << "import fnss\n";
            //pythonScript << "import networkx as nx\n";
            pythonScript << "topology = " << "fnss.fat_tree_topology(k=" << params.fattree.k << ")\n";
            pythonScript << "fnss.write_topology(topology, '.datacenter_topology.xml')\n";
            PyRun_SimpleString(pythonScript.str().c_str());
            Py_Finalize();
            
            fnss::Topology t = fnss::Parser::parseTopology(".datacenter_topology.xml");
            std::set<std::pair <std::string, std::string> > edges = t.getAllEdges();
            std::set<std::string> nodes = t.getAllNodes();
            assert (nodes.size() > 0 && edges.size() > 0);
            
            std::shared_ptr<Network<A, B>> net (new Network<A, B>());
            std::map<std::string, int> fnssNodeIdToVNESimNodeId;
            
            for(set<string>::iterator it = nodes.begin(); it != nodes.end(); it++)
            {
                fnss::Node fnssNode = t.getNode(*it);
                
                std::shared_ptr<A> n = nullptr;
                // If the node is a host create it with a cpu capacity
                if (fnssNode.getProperty("type").compare("host") == 0)
                {
                    numHosts++;
                    double node_cpu = RNG::Instance()->sampleDistribution<double,double,double,double>
                    (cpu_dist, std::tuple<double,double,double> (cpu_param1, cpu_param2, cpu_param3)) ;
                    n.reset (new A (node_cpu, 0, 0));
                }
                else
                {
                    numSwitches++;
                    n.reset (new A (0,0,0));
                }
                fnssNodeIdToVNESimNodeId[*it] = n->getId();
                net->addNode (n);
            }
            
            for(set<pair <string, string> >::iterator it = edges.begin(); it != edges.end(); it++)
            {
                double link_bw = RNG::Instance()->sampleDistribution<double,double,double,double>
                (bw_dist, std::tuple<double,double,double> (bw_param1, bw_param2, bw_param3));
                
                double link_delay = RNG::Instance()->sampleDistribution<double,double,double,double>
                (delay_dist, std::tuple<double,double,double> (delay_param1, delay_param2, delay_param3));
                
                std::shared_ptr<B> l = nullptr;
                int nodeFromId = fnssNodeIdToVNESimNodeId[(*it).first];
                fnss::Node fnssNodeFrom = t.getNode ((*it).first);
                int nodeToId = fnssNodeIdToVNESimNodeId[(*it).second];
                fnss::Node fnssNodeTo = t.getNode ((*it).second);
                
                if ((fnssNodeFrom.getProperty("layer").compare("core") == 0 ||  fnssNodeTo.getProperty ("layer").compare("core") == 0 ))
                    l.reset (new B (params.fattree.coreBWMultiplier * link_bw, link_delay, nodeFromId, nodeToId));
                    
                else
                    l.reset (new B (link_bw, link_delay, nodeFromId, nodeToId));
                net->addLink (l);
            }
            this->pt.put ("n_switches", numSwitches);
            this->pt.put ("n_hosts", numHosts);
            this->pt.put ("n_links", net->getNumLinks());
            return net;
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
        n_hosts(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNTwoTier.n_hosts")),
        coreBWMultiplier (ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNTwoTier.coreBWMultiplier"))
        {
        } 
        
        template <typename A, typename B> 
        FNSSHandler<A,B>::Parameters::DCNFatTree::DCNFatTree () :
        k(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNFatTree.K")),
        coreBWMultiplier (ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.FNSSHandler.DCNFatTree.coreBWMultiplier"))
        {
        } 
        
        
        template<typename A, typename B>
        FNSSHandler<A,B>::Parameters::Parameters () :
        bcube (DCNBCube()),
        twotier (DCNTwoTier()),
        fattree (DCNFatTree())
        {
        }
        
        template <typename A, typename B> 
        FNSSHandler<A,B>::FNSSHandler () :
        ExternalLibHandler<A,B> (),
        params(Parameters())
        {
	      this->pt.put ("DCNBCube.N", params.bcube.n);
	      this->pt.put ("DCNBCube.K", params.bcube.k);
          this->pt.put ("DCNFatTree.coreBWMultiplier", params.fattree.coreBWMultiplier);
	      this->pt.put ("DCNFatTree.K", params.fattree.k);
	      this->pt.put ("DCNTwoTier.n_core", params.twotier.n_core);
	      this->pt.put ("DCNTwoTier.n_edges", params.twotier.n_edges);
	      this->pt.put ("DCNTwoTier.n_hosts", params.twotier.n_hosts);
          this->pt.put ("DCNTwoTier.coreBWMultiplier", params.twotier.coreBWMultiplier);
        }
    }
}
#endif
#endif

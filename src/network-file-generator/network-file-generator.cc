/**
 * @file network-file-generator.cc
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

#include "network-file-generator.h"

#include <boost/filesystem.hpp>

namespace vne {
    namespace nfg {
        
        NetworkFileGenerator::Parameters::Parameters() :
        DirToSaveFiles(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.DirToSaveFiles")),
        totalTime(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.TotalTime")),
        SubstrateNodeNum(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.SubstrateNodeNum")),
        VNRLinkSplittingRate (ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRLinkSplittingRate")),
        Handler(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.Handler")),
        vn_topo_type (get_Topology_Type(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.VNTopologyType"))),
        sn_topo_type (get_Topology_Type(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.SNTopologyType"))),
        
        VNRNumNodesDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VNRNumNodesDist")),
        VNRNumNodesDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRNumNodesDistParam1")),
        VNRNumNodesDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRNumNodesDistParam2")),
        VNRNumNodesDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRNumNodesDistParam3")),
        
        VNRDurationDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VNRDurationDist")),
        VNRDurationDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRDurationDistParam1")),
        VNRDurationDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRDurationDistParam2")),
        VNRDurationDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRDurationDistParam3")),
        
        VNRArrivalDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VNRArrivalDist")),
        VNRArrivalDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRArrivalDistParam1")),
        VNRArrivalDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRArrivalDistParam2")),
        VNRArrivalDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRArrivalDistParam3")),
        
        VNRMaxDistanceDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VNRDurationDist")),
        VNRMaxDistanceDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRMaxDistanceDistParam1")),
        VNRMaxDistanceDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRMaxDistanceDistParam2")),
        VNRMaxDistanceDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNRMaxDistanceDistParam3")),
        
        SNCPUDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.SNCPUDist")),
        SNCPUDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SNCPUDistParam1")),
        SNCPUDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SNCPUDistParam2")),
        SNCPUDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SNCPUDistParam3")),
        
        SLBWDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.SLBWDist")),
        SLBWDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SLBWDistParam1")),
        SLBWDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SLBWDistParam2")),
        SLBWDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SLBWDistParam3")),
        
        SLDelayDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.SLDelayDist")),
        SLDelayDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SLDelayDistParam1")),
        SLDelayDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SLDelayDistParam2")),
        SLDelayDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.SLDelayDistParam3")),
        
        VNCPUDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VNCPUDist")),
        VNCPUDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNCPUDistParam1")),
        VNCPUDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNCPUDistParam2")),
        VNCPUDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VNCPUDistParam3")),
        
        VLBWDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VLBWDist")),
        VLBWDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VLBWDistParam1")),
        VLBWDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VLBWDistParam2")),
        VLBWDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VLBWDistParam3")),
        
        VLDelayDist((Distribution)ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.VLDelayDist")),
        VLDelayDistParam1(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VLDelayDistParam1")),
        VLDelayDistParam2(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VLDelayDistParam2")),
        VLDelayDistParam3(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.VLDelayDistParam3"))
        
        {
            
            snpt.put ("SubstrateNodeNum", SubstrateNodeNum);
            snpt.put ("SNTopologyType", get_Topology_Type_Str(sn_topo_type));
            snpt.put ("SNCPUDist",  (int) SNCPUDist);
            snpt.put ("SNCPUDistParam1",  SNCPUDistParam1);
            snpt.put ("SNCPUDistParam2",  SNCPUDistParam2);
            snpt.put ("SNCPUDistParam3",  SNCPUDistParam3);
            
            snpt.put ("SLBWDist",  (int) SLBWDist);
            snpt.put ("SLBWDistParam1",  SLBWDistParam1);
            snpt.put ("SLBWDistParam2",  SLBWDistParam2);
            snpt.put ("SLBWDistParam3",  SLBWDistParam3);
            
            snpt.put ("SLDelayDist",  (int) SLDelayDist);
            snpt.put ("SLDelayDistParam1",  SLDelayDistParam1);
            snpt.put ("SLDelayDistParam2",  SLDelayDistParam2);
            snpt.put ("SLDelayDistParam3",  SLDelayDistParam3);
            
            vnrpt.put ("TotalTime", totalTime);
            vnrpt.put ("VNTopologyType", get_Topology_Type_Str(vn_topo_type));
            vnrpt.put ("VNRLinkSplittingRate", VNRLinkSplittingRate);
            
            vnrpt.put ("VNRNumNodesDist", (int) VNRNumNodesDist);
            vnrpt.put ("VNRNumNodesDistParam1", VNRNumNodesDistParam1);
            vnrpt.put ("VNRNumNodesDistParam2", VNRNumNodesDistParam2);
            vnrpt.put ("VNRNumNodesDistParam3", VNRNumNodesDistParam3);
            
            vnrpt.put ("VNRDurationDist",  (int) VNRDurationDist);
            vnrpt.put ("VNRDurationDistParam1",  VNRDurationDistParam1);
            vnrpt.put ("VNRDurationDistParam2",  VNRDurationDistParam2);
            vnrpt.put ("VNRDurationDistParam3",  VNRDurationDistParam3);
            
            vnrpt.put ("VNRArrivalDist",  (int) VNRArrivalDist);
            vnrpt.put ("VNRArrivalDistParam1",  VNRArrivalDistParam1);
            vnrpt.put ("VNRArrivalDistParam2",  VNRArrivalDistParam2);
            vnrpt.put ("VNRArrivalDistParam3",  VNRArrivalDistParam3);
            
            vnrpt.put ("VNRMaxDistanceDist",  (int) VNRMaxDistanceDist);
            vnrpt.put ("VNRMaxDistanceDistParam1",  VNRMaxDistanceDistParam1);
            vnrpt.put ("VNRMaxDistanceDistParam2",  VNRMaxDistanceDistParam2);
            vnrpt.put ("VNRMaxDistanceDistParam3",  VNRMaxDistanceDistParam3);
            
            vnrpt.put ("VNCPUDist",  (int) VNCPUDist);
            vnrpt.put ("VNCPUDistParam1",  VNCPUDistParam1);
            vnrpt.put ("VNCPUDistParam2",  VNCPUDistParam2);
            vnrpt.put ("VNCPUDistParam3",  VNCPUDistParam3);
            
            vnrpt.put ("VLBWDist",  (int) VLBWDist);
            vnrpt.put ("VLBWDistParam1",  VLBWDistParam1);
            vnrpt.put ("VLBWDistParam2",  VLBWDistParam2);
            vnrpt.put ("VLBWDistParam3",  VLBWDistParam3);
            
            vnrpt.put ("VLDelayDist",  (int) VLDelayDist);
            vnrpt.put ("VLDelayDistParam1",  VLDelayDistParam1);
            vnrpt.put ("VLDelayDistParam2",  VLDelayDistParam2);
            vnrpt.put ("VLDelayDistParam3",  VLDelayDistParam3);
        }
        
        NetworkFileGenerator::NetworkFileGenerator () :
        params (Parameters())
        {
        }
        
        NetworkFileGenerator::~NetworkFileGenerator()
        {
        }
        
        std::shared_ptr<Network<VYSubstrateNode<>,VYSubstrateLink<>>>
        NetworkFileGenerator::VYSubstrateNetFileGenerator (bool writeToFile)
        {
            std::shared_ptr<ExternalLibHandler<VYSubstrateNode<>, VYSubstrateLink<>>> handler;
            #ifdef WITH_FNSS_SUPPORT
            if (params.Handler.compare("FNSS") == 0)
                handler.reset(new FNSSHandler<VYSubstrateNode<>, VYSubstrateLink<>> ());
            else
                handler.reset(new BriteHandler<VYSubstrateNode<>, VYSubstrateLink<>>);
            #else
                handler.reset(new BriteHandler<VYSubstrateNode<>, VYSubstrateLink<>>); 
            #endif
           std::shared_ptr<Network<VYSubstrateNode<>,VYSubstrateLink<>>> substrate_net
            = handler->getNetwork(params.sn_topo_type, params.SubstrateNodeNum, params.SNCPUDist, params.SNCPUDistParam1, params.SNCPUDistParam2, params.SNCPUDistParam3, params.SLBWDist, params.SLBWDistParam1, params.SLBWDistParam2, params.SLBWDistParam3, params.SLDelayDist, params.SLDelayDistParam1, params.SLDelayDistParam2, params.SLDelayDistParam3);
            
            if (writeToFile)
            {
                std::ofstream ofstrm;
                std::stringstream strstrm;
                strstrm << params.DirToSaveFiles << "/substrate_nets";
                boost::filesystem::path p (strstrm.str());
                boost::filesystem::create_directory(p);
                
                try
                {
                    if ( exists(p) && is_directory(p))
                    {
                        std::stringstream snConfigFiles;
                        snConfigFiles << strstrm.str();
                        snConfigFiles << "/substrate_net_params.xml";
                        auto setting = boost::property_tree::xml_writer_make_settings<std::string>(' ',4);
                        boost::property_tree::write_xml(snConfigFiles.str(), params.snpt, std::locale(), setting);
                        
                        snConfigFiles.str(std::string());
                        snConfigFiles << strstrm.str();
                        snConfigFiles << "/substrate_net_generation_algo_params.xml";
                        boost::property_tree::write_xml(snConfigFiles.str(), handler->getProperties(), std::locale(), setting);
                        
                        strstrm << "/substrate_net_" << get_Topology_Type_Str(params.sn_topo_type) << "_" << params.Handler << "_nodes_" <<
                            substrate_net->getNumNodes() << "_links_" << substrate_net->getNumLinks() << handler->getPreferredFileName() << ".txt";
                        
                        ofstrm.open(strstrm.str().c_str());
                        if (ofstrm.is_open())
                        {
                            substrate_net->writeNetworkToFile(ofstrm, true);
                        }
                        else
                        {
                            std::cerr << ">>>> Exception in writing substrate network file.<<< \n" << "Could not open File: " << strstrm.str();
                            throw;
                        }
                    }
                    else
                    {
                        std::cerr << ">>>> Exception in writing substrate network file.<<< \n" <<
                        "Directory: " << params.DirToSaveFiles
                        << " does not exist.";
                        throw;
                    }
                }
                catch (...)
                {
                }
            }
            return substrate_net;
        }
        std::shared_ptr<std::list<std::shared_ptr<VYVirtualNetRequest<>>>>
            NetworkFileGenerator::VYVirtualNetRequestGenerator (bool writeToFile)
        {
            
            //set the handler
            std::shared_ptr<ExternalLibHandler<VYVirtualNode<>,VYVirtualLink<>>> handler;
            #ifdef WITH_FNSS_SUPPORT
            if (params.Handler.compare("FNSS") == 0)
                handler.reset(new FNSSHandler<VYVirtualNode<>,VYVirtualLink<>> ());
            else
                handler.reset(new BriteHandler<VYVirtualNode<>,VYVirtualLink<>>);
            #else
                handler.reset(new BriteHandler<VYVirtualNode<>,VYVirtualLink<>>); 
            #endif   
            std::shared_ptr<std::list<std::shared_ptr<VYVirtualNetRequest<>>>> vnrlist
            (new std::list<std::shared_ptr<VYVirtualNetRequest<>>>());
            int numRequests = (int) (params.totalTime / params.VNRArrivalDistParam1);
            unsigned int time = 0;
            
            std::string vnrDirectoryPath;
            
            if (writeToFile)
            {
                try
                {
                    std::stringstream strstrm;
                    strstrm << params.DirToSaveFiles << "/" << "reqs-" << (int) params.VNRArrivalDistParam1
                        << "-" << (int) params.VNRDurationDistParam1 <<
                        "-nodesMin-" << (int) params.VNRNumNodesDistParam1 <<
                        "-nodesMax-" << (int) params.VNRNumNodesDistParam2 <<
                        "-grid-" << ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.BriteHandler.outerGridSize");
                    boost::filesystem::path p (strstrm.str());
                    vnrDirectoryPath = strstrm.str();
                    
                    if ( exists(p) || !boost::filesystem::create_directory(p))
                    {
                        cerr << "Cannot create a directory to save virtual netwrok request files." << std::endl;
                        cerr << "Directory: " << vnrDirectoryPath << std::endl;
                        throw;
                    }
                    std::stringstream configFile;
                    configFile << vnrDirectoryPath << "/vnr_params.xml";
                    auto setting = boost::property_tree::xml_writer_make_settings<std::string>(' ',4);
                    boost::property_tree::write_xml(configFile.str(), params.vnrpt, std::locale(), setting);
                    
                    configFile.str(std::string());
                    configFile << vnrDirectoryPath << "/vnr_brite_params.xml";
                    boost::property_tree::write_xml(configFile.str(), handler->getProperties(), std::locale(), setting);
                }
                catch (...)
                {
                }
            }
            
            for (int i=0; i< numRequests; i++)
            {
                int numNodes = RNG::Instance()->sampleDistribution<int>(params.VNRNumNodesDist, std::tuple<double,double,double> (params.VNRNumNodesDistParam1, params.VNRNumNodesDistParam2, params.VNRNumNodesDistParam3));
                
                    std::shared_ptr<Network<VYVirtualNode<>,VYVirtualLink<>>> vn =
                        handler->getNetwork (params.vn_topo_type, numNodes, params.VNCPUDist, params.VNCPUDistParam1, params.VNCPUDistParam2, params.VNCPUDistParam3, params.VLBWDist, params.VLBWDistParam1, params.VLBWDistParam2, params.VLBWDistParam3, params.VLDelayDist, params.VLDelayDistParam1, params.VLDelayDistParam2, params.VLDelayDistParam3);
                double rnd = gsl_rng_uniform (RNG::Instance()->getGeneralRNG());
                
                int linksplitable = (rnd < params.VNRLinkSplittingRate) ? (int)Link_Embedding_Algo_Types::WITH_PATH_SPLITTING : (int)Link_Embedding_Algo_Types::NO_PATH_SPLITTING;
                
                unsigned int interArrivalTime;
                do {
                     interArrivalTime = (unsigned int) RNG::Instance()->sampleDistribution<double>(params.VNRArrivalDist, std::tuple<double,double,double>(params.VNRArrivalDistParam1,params.VNRArrivalDistParam2,params.VNRArrivalDistParam3));
                    //BOOST_LOG_TRIVIAL(debug) << time+ interArrivalTime;
                }
                while (time + interArrivalTime > params.totalTime);
                time += interArrivalTime;
                
                unsigned int duration = (unsigned int) RNG::Instance()->sampleDistribution<double>(params.VNRDurationDist, std::tuple<double,double,double>(params.VNRDurationDistParam1,params.VNRDurationDistParam2, params.VNRDurationDistParam3));
                
                unsigned int maxD = (unsigned int) RNG::Instance()->sampleDistribution<double>(params.VNRMaxDistanceDist, std::tuple<double,double,double>(params.VNRMaxDistanceDistParam1,params.VNRMaxDistanceDistParam2, params.VNRMaxDistanceDistParam3));
                
                int topo = 0;
                std::shared_ptr<VYVirtualNetRequest<>> vnr (new VYVirtualNetRequest<> (vn, (double) time, (double) duration, (int) linksplitable, topo, (int)maxD,nullptr,nullptr));
                vnrlist->push_back(vnr);
                
                if (writeToFile)
                {
                    std::ofstream ofstrm;
                    std::stringstream strstrm;
                    BOOST_LOG_TRIVIAL(debug) << "Directory Path: " << vnrDirectoryPath << std::endl;
                    strstrm << vnrDirectoryPath;
                    boost::filesystem::path p (vnrDirectoryPath);
                    
                    try
                    {
                        if ( exists(p) && is_directory(p))
                        {
                            
                            
                            strstrm << "/req" << i << ".txt";
                            
                            ofstrm.open(strstrm.str().c_str());
                            if (ofstrm.is_open())
                            {
                                vnr->writeVNRToFile (ofstrm);
                            }
                            else
                            {
                                std::cerr << ">>>> Exception in writing virtual network file.<<< \n" << "Could not open File: " << strstrm.str();
                                throw;
                            }
                        }
                        else
                        {
                            std::cerr << ">>>> Exception in writing virtual network file.<<< \n" <<
                            "Directory: " << params.DirToSaveFiles
                            << " does not exist.";
                            throw;
                        }
                    }
                    catch (...)
                    {
                    }
                }
                
            }
            return vnrlist;
        }
    }
}

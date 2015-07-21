/**
 * @file mcvne-experiments.cc
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

#include "mcvne-experiments.h"
#include "mcvne/mcvne-node-embedding-algo.h"
#include "mcvne/mcvne-bfs-link-embedding-algo.h"

#include "Vineyard/vy-vine-link-embedding-algo.h"
#include "Vineyard/vy-vine-link-embedding-algo-no-disk-io.h"

#include <cstring>

namespace vne {
    namespace experiments {
        
        template<>
        MCVNENodeMCFNoDiskIOLinkExp<>::MCVNENodeMCFNoDiskIOLinkExp ()
        {
            sb  = std::make_shared<VYSubstrateNetworkBuilder<>>(VYSubstrateNetworkBuilder<>());
            
            std::shared_ptr<VYVineTwoStageEmbeddingAlgo<>> embeddingAlgo (new VYVineTwoStageEmbeddingAlgo<>
                                                                          (*sb,
                                                                           std::shared_ptr<MCVNENodeEmbeddingAlgo<>> (new MCVNENodeEmbeddingAlgo<> ()),
                                                                           std::shared_ptr<VYVineLinkEmbeddingAlgoNoDiskIO<>> (new VYVineLinkEmbeddingAlgoNoDiskIO<> ())
                                                                           )
                                                                          );
            
            std::shared_ptr<ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>> releaseAlgo (new ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>(*sb));
            
            VYVNREmbeddingProc<>* embeddingProc (new VYVNREmbeddingProc<>(embeddingAlgo));
            VYVNRReleaseProc<>* releaseProc (new VYVNRReleaseProc<>(releaseAlgo));
            VYVNRProcObserver<>* observer (new VYVNRProcObserver<> (sb->getNetwork()));
            VYVNRGenerator<>* gen (new VYVNRGenerator<>());
            observer->registerSubscriber(this);
            
            graph = new VYVNRProcDigraph<> (embeddingProc, releaseProc, gen, observer);
            
            std::stringstream vnrDir;
            vnrDir << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.path") << "/"
            << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.dir");
            
            std::stringstream snDir (ConfigManager::Instance()->getConfig<std::string>("vineyard.SubstrateNetwork.path"));
            
            std::stringstream ConfigFile;
            ConfigFile << vnrDir.str() << "/vnr_params.xml";
            
            boost::property_tree::ptree VNParams;
            boost::property_tree::read_xml(ConfigFile.str(), VNParams);
            
            ConfigFile.str(std::string());
            ConfigFile << vnrDir.str() <<"/vnr_brite_params.xml";
            boost::property_tree::ptree VNBriteParams;
            boost::property_tree::read_xml(ConfigFile.str(), VNBriteParams);
            
            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_params.xml";
            boost::property_tree::ptree SNParams;
            boost::property_tree::read_xml(ConfigFile.str(), SNParams);
            
            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_brite_params.xml";
            boost::property_tree::ptree SNBriteParams;
            boost::property_tree::read_xml(ConfigFile.str(), SNBriteParams);
            
            params.setAllParams(SNParams, SNBriteParams, VNParams, VNBriteParams);
            
            
            std::string link_algo ("MCF Link Mapping");
            std::string node_algo ("MCVNE Node Mapping");
            
            Embedding_Algorithm_Types algo_type = Embedding_Algorithm_Types::TWO_STAGE;
            this->initialize (graph, algo_type, node_algo, link_algo);
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setBeta");
            mcts_max_depth = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.MaxDepth");
            mcts_num_simulations = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.NumSimulations");
            mcts_expand_count = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.ExpandCount");
            mcts_exploration_constant = ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.ExplorationConstant");
            mcts_use_rave = (int) ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseRave");
            mcts_rave_discount = (mcts_use_rave > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveDiscount") : -1.0;
            mcts_rave_constant = (mcts_use_rave > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveConstant") : -1.0;
            mcts_use_sp_mcts  = (int) ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseSinglePlayerMCTS");
            mcts_sp_constant =  (mcts_use_sp_mcts > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.SPMCTSConstant") : -1.0;
            mcts_discount = ConfigManager::Instance()->getConfig<double>("MCTS.Simulator.discount");
            
        }
        
        template<>
        MCVNENodeMCFNoDiskIOLinkExp<>::~MCVNENodeMCFNoDiskIOLinkExp()
        {};
        
        template<>
        void MCVNENodeMCFNoDiskIOLinkExp<>::statisticsGenerated (Statistics& stat)
        {
            statistics.push_back(*static_cast<VYStatistics*> (&stat));
        }
        
        template<>
        MCVNENodeMCFLinkExp<>::MCVNENodeMCFLinkExp ()
        {
            sb  = std::make_shared<VYSubstrateNetworkBuilder<>>(VYSubstrateNetworkBuilder<>());
            
            std::shared_ptr<VYVineTwoStageEmbeddingAlgo<>> embeddingAlgo (new VYVineTwoStageEmbeddingAlgo<>
                                                                          (*sb,
                                                                           std::shared_ptr<MCVNENodeEmbeddingAlgo<>> (new MCVNENodeEmbeddingAlgo<> ()),
                                                                           std::shared_ptr<VYVineLinkEmbeddingAlgo<>> (new VYVineLinkEmbeddingAlgo<> ())
                                                                           )
                                                                          );
            
            std::shared_ptr<ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>> releaseAlgo (new ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>(*sb));
            
            VYVNREmbeddingProc<>* embeddingProc (new VYVNREmbeddingProc<>(embeddingAlgo));
            VYVNRReleaseProc<>* releaseProc (new VYVNRReleaseProc<>(releaseAlgo));
            VYVNRProcObserver<>* observer (new VYVNRProcObserver<> (sb->getNetwork()));
            VYVNRGenerator<>* gen (new VYVNRGenerator<>());
            observer->registerSubscriber(this);
            
            graph = new VYVNRProcDigraph<> (embeddingProc, releaseProc, gen, observer);
            
            std::stringstream vnrDir;
            vnrDir << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.path") << "/"
            << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.dir");
            
            std::stringstream snDir (ConfigManager::Instance()->getConfig<std::string>("vineyard.SubstrateNetwork.path"));
            
            std::stringstream ConfigFile;
            ConfigFile << vnrDir.str() << "/vnr_params.xml";
            
            boost::property_tree::ptree VNParams;
            boost::property_tree::read_xml(ConfigFile.str(), VNParams);
            
            ConfigFile.str(std::string());
            ConfigFile << vnrDir.str() <<"/vnr_brite_params.xml";
            boost::property_tree::ptree VNBriteParams;
            boost::property_tree::read_xml(ConfigFile.str(), VNBriteParams);
            
            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_params.xml";
            boost::property_tree::ptree SNParams;
            boost::property_tree::read_xml(ConfigFile.str(), SNParams);
            
            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_brite_params.xml";
            boost::property_tree::ptree SNBriteParams;
            boost::property_tree::read_xml(ConfigFile.str(), SNBriteParams);
            
            params.setAllParams(SNParams, SNBriteParams, VNParams, VNBriteParams);
            
            
            std::string link_algo ("MCF Link Mapping");
            std::string node_algo ("MCVNE Node Mapping");
            
            Embedding_Algorithm_Types algo_type = Embedding_Algorithm_Types::TWO_STAGE;
            this->initialize (graph, algo_type, node_algo, link_algo);
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("MCVNE.VNEMCTSSimulator.setBeta");
            mcts_max_depth = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.MaxDepth");
            mcts_num_simulations = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.NumSimulations");
            mcts_expand_count = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.ExpandCount");
            mcts_exploration_constant = ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.ExplorationConstant");
            mcts_use_rave = (int) ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseRave");
            mcts_rave_discount = (mcts_use_rave > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveDiscount") : -1.0;
            mcts_rave_constant = (mcts_use_rave > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveConstant") : -1.0;
            mcts_use_sp_mcts  = (int) ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseSinglePlayerMCTS");
            mcts_sp_constant =  (mcts_use_sp_mcts > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.SPMCTSConstant") : -1.0;
            mcts_discount = ConfigManager::Instance()->getConfig<double>("MCTS.Simulator.discount");
            
        }
        
        template<>
        MCVNENodeMCFLinkExp<>::~MCVNENodeMCFLinkExp()
        {};
        
        template<>
        void MCVNENodeMCFLinkExp<>::statisticsGenerated (Statistics& stat)
        {
            statistics.push_back(*static_cast<VYStatistics*> (&stat));
        }
        

        template<>
        MCVNENodeBFSLinkExp<>::MCVNENodeBFSLinkExp ()
        {
            sb  = std::make_shared<VYSubstrateNetworkBuilder<>>(VYSubstrateNetworkBuilder<>());
            
            std::shared_ptr<VYVineTwoStageEmbeddingAlgo<>> embeddingAlgo (new VYVineTwoStageEmbeddingAlgo<>
                                                                          (*sb,
                                                                           std::shared_ptr<MCVNENodeEmbeddingAlgo<>>(new MCVNENodeEmbeddingAlgo<>()),
                                                                           std::shared_ptr<MCVNEBFSLinkEmbeddingAlgo<>>(new MCVNEBFSLinkEmbeddingAlgo<>())
                                                                           )
                                                                          );
            
            std::shared_ptr<ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>> releaseAlgo (new ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>(*sb));
            
            VYVNREmbeddingProc<>* embeddingProc (new VYVNREmbeddingProc<>(embeddingAlgo));
            VYVNRReleaseProc<>* releaseProc (new VYVNRReleaseProc<>(releaseAlgo));
            VYVNRProcObserver<>* observer (new VYVNRProcObserver<> (sb->getNetwork()));
            VYVNRGenerator<>* gen (new VYVNRGenerator<>());
            observer->registerSubscriber(this);
            
            graph = new VYVNRProcDigraph<> (embeddingProc, releaseProc, gen, observer);
            
            std::stringstream vnrDir;
            vnrDir << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.path") << "/"
            << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.dir");
            
            std::stringstream snDir (ConfigManager::Instance()->getConfig<std::string>("vineyard.SubstrateNetwork.path"));
            
            std::stringstream ConfigFile;
            ConfigFile << vnrDir.str() << "/vnr_params.xml";
            
            boost::property_tree::ptree VNParams;
            boost::property_tree::read_xml(ConfigFile.str(), VNParams);
            
            ConfigFile.str(std::string());
            ConfigFile << vnrDir.str() << "/vnr_brite_params.xml";
            boost::property_tree::ptree VNBriteParams;
            boost::property_tree::read_xml(ConfigFile.str(), VNBriteParams);
            
            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_params.xml";
            boost::property_tree::ptree SNParams;
            boost::property_tree::read_xml(ConfigFile.str(), SNParams);
            
            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_brite_params.xml";
            boost::property_tree::ptree SNBriteParams;
            boost::property_tree::read_xml(ConfigFile.str(), SNBriteParams);
            
            params.setAllParams(SNParams, SNBriteParams, VNParams, VNBriteParams);
            
            std::string link_algo ("BFS-SP Link Mapping");
            std::string node_algo ("MCVNE Node Mapping");
            
            Embedding_Algorithm_Types algo_type = Embedding_Algorithm_Types::TWO_STAGE;
            this->initialize (graph, algo_type, node_algo, link_algo);
            
            mcts_max_depth = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.MaxDepth");
            mcts_num_simulations = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.NumSimulations");
            mcts_expand_count = ConfigManager::Instance()->getConfig<int>("MCTS.MCTSParameters.ExpandCount");
            mcts_exploration_constant = ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.ExplorationConstant");
            mcts_use_rave = (int) ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseRave");
            mcts_rave_discount = (mcts_use_rave > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveDiscount") : -1.0;
            mcts_rave_constant = (mcts_use_rave > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.RaveConstant") : -1.0;
            mcts_use_sp_mcts  = (int) ConfigManager::Instance()->getConfig<bool>("MCTS.MCTSParameters.UseSinglePlayerMCTS");
            mcts_sp_constant =  (mcts_use_sp_mcts > 0) ? ConfigManager::Instance()->getConfig<double>("MCTS.MCTSParameters.SPMCTSConstant") : -1.0;
            mcts_discount = ConfigManager::Instance()->getConfig<double>("MCTS.Simulator.discount");
        }
        
        template<>
        MCVNENodeBFSLinkExp<>::~MCVNENodeBFSLinkExp()
        {};
        
        template<>
        void MCVNENodeBFSLinkExp<>::statisticsGenerated (Statistics& stat)
        {
            statistics.push_back(*static_cast<VYStatistics*> (&stat));
        }
        
    }
}
using namespace vne::experiments;
using namespace vne;
HIBERLITE_EXPORT_CLASS_WITH_NAME(MCVNENodeMCFNoDiskIOLinkExp<>, MCVNENodeMCFLinkNoDiskIO)
HIBERLITE_EXPORT_CLASS_WITH_NAME(MCVNENodeMCFLinkExp<>, MCVNENodeMCFLink)
HIBERLITE_EXPORT_CLASS_WITH_NAME(MCVNENodeBFSLinkExp<>, MCVNENodeBFSLink)


/**
 * @file vineyard-experiments.cc
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

#include "vineyard-experiments.h"
#include "Vineyard/vy-vine-node-embedding-algo.h"
#include "Vineyard/vy-vine-link-embedding-algo.h"

#include <cstring>

namespace vne {
    namespace experiments {
        template<>
        VineNodeMCFLinkExp<>::VineNodeMCFLinkExp ()
        {
            sb  = std::make_shared<VYSubstrateNetworkBuilder<>>(VYSubstrateNetworkBuilder<>());

            std::shared_ptr<VYVineTwoStageEmbeddingAlgo<>> embeddingAlgo (new VYVineTwoStageEmbeddingAlgo<>
             (*sb,
              std::make_shared<VYVineNodeEmbeddingAlgo<>>(VYVineNodeEmbeddingAlgo<>()),
              std::make_shared<VYVineLinkEmbeddingAlgo<>>(VYVineLinkEmbeddingAlgo<>())
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
            vnrDir << ConfigManager::Instance()->getConfig<std::string>("vineyard", "VirtualNetRequest", "path") << "/"
            << ConfigManager::Instance()->getConfig<std::string>("vineyard", "VirtualNetRequest", "dir");

            std::stringstream snDir (ConfigManager::Instance()->getConfig<std::string>("vineyard", "SubstrateNetwork", "path"));

            std::stringstream ConfigFile;
            ConfigFile << vnrDir.str() << "/vnr_generation_params.toml";

            auto VNRParams = toml::parse(ConfigFile.str());

            ConfigFile.str (std::string());
            ConfigFile << snDir.str() << "/substrate_net_generation_params.toml";
            auto SNParams  = toml::parse(ConfigFile.str());

            params.setAllParams(SNParams, VNRParams);

            std::string link_algo ("Vine MCF Link Mapping");
            std::string node_algo ("Vine Node Mapping");

            Embedding_Algorithm_Types algo_type = Embedding_Algorithm_Types::TWO_STAGE;
            this->initialize (graph, algo_type, node_algo, link_algo);

            setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setBeta");
            node_embedding_type  = ConfigManager::Instance()->getConfig<std::string>("vineyard", "Configs", "nodeMappingType");
        }

        template<>
        VineNodeMCFLinkExp<>::~VineNodeMCFLinkExp()
        {};

        template<>
        void VineNodeMCFLinkExp<>::statisticsGenerated (Statistics& stat)
        {
            statistics.push_back(*static_cast<VYStatistics*> (&stat));
        }

    }
}
using namespace vne::experiments;
using namespace vne;
HIBERLITE_EXPORT_CLASS_WITH_NAME(VineNodeMCFLinkExp<>, VineNodeMCFLink)

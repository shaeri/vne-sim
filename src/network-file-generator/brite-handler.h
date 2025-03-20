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

#ifndef NFG_BRITE_HANDLER_
#define NFG_BRITE_HANDLER_

#include "external-lib-handler.h"

#include "BRITE/C++/Brite.h"
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

namespace vne
{
namespace nfg
{
    template <typename A, typename B>
    class BriteHandler : public ExternalLibHandler<A, B>
    {
       public:
        BriteHandler();
        virtual ~BriteHandler() {};
        struct Parameters {
            Parameters();
            std::string BriteSeedFile;
            int nodePlacement;
            int numNeighbors;
            int innerGridSize;
            int outerGridSize;

            struct RTWaxman {
                RTWaxman();
                int growthType;
                double alpha;
                double beta;
            } rtWaxman;
        };

        const Parameters &getParams() const;

        virtual std::shared_ptr<Network<A, B>> getNetwork(
            Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2,
            double cpu_param3, Distribution bw_dist, double bw_param1, double bw_param2,
            double bw_param3, Distribution delay_dist, double delay_param1, double delay_param2,
            double delay_param3) override;

        virtual std::string getPreferredFileName() override;

       private:
        Parameters params;

        //bw and cpu types: Constant = 1, Uniform =2, HeavyTailed = 3, Exponential =4
        std::shared_ptr<Network<A, B>> getNetwork_RTWaxman(
            int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
            Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
            Distribution delay_dist, double delay_param1, double delay_param2,
            double delay_param3);
        std::shared_ptr<Network<A, B>> createNetFromBRITETopo(
            std::shared_ptr<Topology> t, Distribution cpu_dist, double cpu_param1,
            double cpu_param2, double cpu_param3, Distribution bw_dist, double bw_param1,
            double bw_param2, double bw_param3, Distribution delay_dist, double delay_param1,
            double delay_param2, double delay_param3);
        inline void InitSeeds();
        //void ParseModel ();
    };

    template <typename A, typename B>
    std::string BriteHandler<A, B>::getPreferredFileName()
    {
        std::stringstream strstrm;
        strstrm << "brite_outergrid_" << params.outerGridSize << "_inner_grid_"
                << params.innerGridSize;
        return strstrm.str();
    }
    template <typename A, typename B>
    std::shared_ptr<Network<A, B>> BriteHandler<A, B>::getNetwork(
        Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2,
        double cpu_param3, Distribution bw_dist, double bw_param1, double bw_param2,
        double bw_param3, Distribution delay_dist, double delay_param1, double delay_param2,
        double delay_param3)
    {
        //if (tt == Topology_Type::Waxman)
        return getNetwork_RTWaxman(n, cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist,
                                   bw_param1, bw_param2, bw_param3, delay_dist, delay_param1,
                                   delay_param2, delay_param3);
    }

    template <typename A, typename B>
    std::shared_ptr<Network<A, B>> BriteHandler<A, B>::getNetwork_RTWaxman(
        int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
        Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
        Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3)
    {
        std::shared_ptr<RouterWaxPar> par = std::shared_ptr<RouterWaxPar>(
            new RouterWaxPar(n, params.outerGridSize, params.innerGridSize, params.nodePlacement,
                             params.rtWaxman.growthType, min(params.numNeighbors, n),
                             params.rtWaxman.alpha, params.rtWaxman.beta, 1, 10, 1024));
        std::shared_ptr<RouterWaxman> rt_wax_model = std::shared_ptr<RouterWaxman>(
            new RouterWaxman(par.get()));
        std::shared_ptr<Topology> t = std::shared_ptr<Topology>(new Topology(rt_wax_model.get()));
        return createNetFromBRITETopo(t, cpu_dist, cpu_param1, cpu_param2, cpu_param3, bw_dist,
                                      bw_param1, bw_param2, bw_param3, delay_dist, delay_param1,
                                      delay_param2, delay_param3);
    }

    template <typename A, typename B>
    std::shared_ptr<Network<A, B>> BriteHandler<A, B>::createNetFromBRITETopo(
        std::shared_ptr<Topology> t, Distribution cpu_dist, double cpu_param1, double cpu_param2,
        double cpu_param3, Distribution bw_dist, double bw_param1, double bw_param2,
        double bw_param3, Distribution delay_dist, double delay_param1, double delay_param2,
        double delay_param3)
    {
        Graph *g = t->GetGraph();
        std::shared_ptr<Network<A, B>> net(new Network<A, B>());

        //we need to have the offset of the node ids because the VirtualNode Ids will be generated
        //in increasing order irrespective of the number of request.
        int nodeID_offset = IdGenerator::Instance()->peekId<A>();

        for (int i = 0; i < g->GetNumNodes(); i++) {
            double node_cpu = RNG::Instance()->sampleDistribution<double, double, double, double>(
                cpu_dist, std::tuple<double, double, double>(cpu_param1, cpu_param2, cpu_param3));
            std::shared_ptr<A> n(new A(node_cpu, (int)g->GetNodePtr(i)->GetNodeInfo()->GetCoordX(),
                                       g->GetNodePtr(i)->GetNodeInfo()->GetCoordY()));
            net->addNode(n);
            BOOST_LOG_TRIVIAL(debug)
                << g->GetNodePtr(i)->GetId() << " "
                << (int)g->GetNodePtr(i)->GetNodeInfo()->GetCoordX() << " "
                << (int)g->GetNodePtr(i)->GetNodeInfo()->GetCoordY() << std::endl;
        }
        for (auto el = g->GetEdges()->begin(); el != g->GetEdges()->end(); el++) {
            double link_bw = RNG::Instance()->sampleDistribution<double, double, double, double>(
                bw_dist, std::tuple<double, double, double>(bw_param1, bw_param2, bw_param3));

            double link_delay = RNG::Instance()
                                    ->sampleDistribution<double, double, double, double>(
                                        delay_dist, std::tuple<double, double, double>(
                                                        delay_param1, delay_param2, delay_param3));

            std::shared_ptr<B> l(new B(link_bw, link_delay,
                                       (*el)->GetSrc()->GetId() + nodeID_offset,
                                       (*el)->GetDst()->GetId() + nodeID_offset));
            net->addLink(l);
            BOOST_LOG_TRIVIAL(debug)
                << " " << (*el)->GetSrc()->GetId() << " " << (*el)->GetDst()->GetId() << link_bw
                << link_delay << std::endl;
        }
        return net;
    }

    template <typename A, typename B>
    void BriteHandler<A, B>::InitSeeds()
    {
        ofstream last_seed_file;

        /* Create Parse object */
        Parse p((char *)params.BriteSeedFile.c_str());

        /* Parse configuration file */
        p.ParseSeed((char *)"PLACES", Model::s_places);
        p.ParseSeed((char *)"CONNECT", Model::s_connect);
        p.ParseSeed((char *)"EDGE_CONN", Model::s_edgeconn);
        p.ParseSeed((char *)"GROUPING", Model::s_grouping);
        p.ParseSeed((char *)"ASSIGNMENT", Model::s_assignment);
        p.ParseSeed((char *)"BANDWIDTH", Model::s_bandwidth);

        BOOST_LOG_TRIVIAL(info) << "Place seed used: " << Model::s_places[0] << " "
                                << Model::s_places[1] << " " << Model::s_places[2] << "\n";
        BOOST_LOG_TRIVIAL(info) << "Connect seed used: " << Model::s_connect[0] << " "
                                << Model::s_connect[1] << " " << Model::s_connect[2] << "\n";
        BOOST_LOG_TRIVIAL(info) << "Edge conn seed used: " << Model::s_edgeconn[0] << " "
                                << Model::s_edgeconn[1] << " " << Model::s_edgeconn[2] << "\n";
        BOOST_LOG_TRIVIAL(info) << "Grouping seed used: " << Model::s_grouping[0] << " "
                                << Model::s_grouping[1] << " " << Model::s_grouping[2] << "\n";
        BOOST_LOG_TRIVIAL(info) << "Assigment seed used: " << Model::s_assignment[0] << " "
                                << Model::s_assignment[1] << " " << Model::s_assignment[2] << "\n";
        BOOST_LOG_TRIVIAL(info) << "Bandwidth seed used: " << Model::s_bandwidth[0] << " "
                                << Model::s_bandwidth[1] << " " << Model::s_bandwidth[2] << "\n"
                                << std::endl;

        last_seed_file.open("last_seed_file", ios::out);

        if (last_seed_file.fail()) {
            cerr << "Cannot open seed files for input/output...\n";
            exit(0);
        }

        last_seed_file << "PLACES"
                       << " " << Model::s_places[0] << " " << Model::s_places[1] << " "
                       << Model::s_places[2] << "\n";

        last_seed_file << "CONNECT"
                       << " " << Model::s_connect[0] << " " << Model::s_connect[1] << " "
                       << Model::s_connect[2] << "\n";

        last_seed_file << "EDGE_CONN"
                       << " " << Model::s_edgeconn[0] << " " << Model::s_edgeconn[1] << " "
                       << Model::s_edgeconn[2] << "\n";

        last_seed_file << "GROUPING"
                       << " " << Model::s_grouping[0] << " " << Model::s_grouping[1] << " "
                       << Model::s_grouping[2] << "\n";

        last_seed_file << "ASSIGNMENT"
                       << " " << Model::s_assignment[0] << " " << Model::s_assignment[1] << " "
                       << Model::s_assignment[2] << "\n";

        last_seed_file << "BANDWIDTH"
                       << " " << Model::s_bandwidth[0] << " " << Model::s_bandwidth[1] << " "
                       << Model::s_bandwidth[2] << "\n";

        last_seed_file.close();
    }

    template <typename A, typename B>
    BriteHandler<A, B>::Parameters::Parameters()
        : BriteSeedFile(ConfigManager::Instance()->getConfig<std::string>(
              "NetworkFileGenerator", "BriteHandler", "BriteSeedFile")),
          nodePlacement(ConfigManager::Instance()->getConfig<int>(
              "NetworkFileGenerator", "BriteHandler", "nodePlacement")),
          numNeighbors(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                 "BriteHandler", "numNeighbors")),
          innerGridSize(ConfigManager::Instance()->getConfig<int>(
              "NetworkFileGenerator", "BriteHandler", "innerGridSize")),
          outerGridSize(ConfigManager::Instance()->getConfig<int>(
              "NetworkFileGenerator", "BriteHandler", "outerGridSize")),
          rtWaxman(RTWaxman())
    {
    }

    template <typename A, typename B>
    BriteHandler<A, B>::Parameters::RTWaxman::RTWaxman()
        : growthType(ConfigManager::Instance()->getConfig<int>(
              "NetworkFileGenerator", "BriteHandler", "RTWaxman", "growthType")),
          alpha(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator",
                                                             "BriteHandler", "RTWaxman", "alpha")),
          beta(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator", "BriteHandler",
                                                            "RTWaxman", "beta"))
    {
    }

    template <typename A, typename B>
    BriteHandler<A, B>::BriteHandler() : ExternalLibHandler<A, B>(), params(Parameters())
    {
        InitSeeds();
        this->pt["NetworkFileGenerator"]["BriteHandler"]["nodePlacement"] = params.nodePlacement;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["numNeighbors"] = params.numNeighbors;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["innerGridSize"] = params.innerGridSize;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["outerGridSize"] = params.outerGridSize;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["nodePlacement"] = params.nodePlacement;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["RTWaxman"]["growthType"] =
            params.rtWaxman.growthType;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["RTWaxman"]["alpha"] = params.rtWaxman
                                                                                    .alpha;
        this->pt["NetworkFileGenerator"]["BriteHandler"]["RTWaxman"]["beta"] = params.rtWaxman
                                                                                   .beta;
    }

    template <typename A, typename B>
    const typename BriteHandler<A, B>::Parameters &BriteHandler<A, B>::getParams() const
    {
        return params;
    }
}  // namespace nfg
}  // namespace vne
#endif /* defined(__vne_mcts__brite_handler__) */

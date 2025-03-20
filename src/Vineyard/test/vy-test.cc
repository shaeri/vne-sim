/**
 * @file   vy-test.cc
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date July 23, 2014
 *
 * @copyright Copyright (c) Jun 3, 2014                      SOROUSH HAERI
 *      All Rights Reserved
 *
 *      Permission to use, copy, modify, and distribute this software and its
 *      documentation for any purpose and without fee is hereby granted, provided
 *      that the above copyright notice appear in all copies and that both that
 *      copyright notice and this permission notice appear in supporting
 *      documentation, and that the name of the author not be used in advertising or
 *      publicity pertaining to distribution of the software without specific,
 *      written prior permission.
 *
 *      THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *      ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *      AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *      DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *      AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *      OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <regex>
#include <math.h>
#include <set>

#include "core/network.h"
#include "core/config-manager.h"
#include "core/two-stage-embedding-algo.h"
#include "core/db-manager.h"

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-node.h"
#include "Vineyard/vy-substrate-link.h"
#include "Vineyard/vy-virtual-link.h"
#include "Vineyard/vy-virtual-net-request.h"
#include "Vineyard/vy-substrate-network-builder.h"
#include "Vineyard/vy-vnr-proc-digraph.h"
#include "Vineyard/vy-vine-node-embedding-algo.h"
#include "Vineyard/vy-vine-link-embedding-algo.h"
#include "Vineyard/vy-vine-two-stage-embedding-algo.h"
#include "Vineyard/vy-statistics.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace vne;
using namespace vne::vineyard;

/*void LoggerInit()
{
    boost::log::core::get()->set_filter
    (
     boost::log::trivial::severity >= boost::log::trivial::error
    );
}
*/
struct ReachabilityCondition {
    bool operator()(
        const std::shared_ptr<VYVirtualNode<double>> lhs,
        const std::shared_ptr<const std::vector<std::shared_ptr<VYVirtualLink<>>>> lhsLinks,
        const std::shared_ptr<VYVirtualNode<double>> rhs, double maxD) const
    {
        return (lhs->getCoordinates().distanceFrom(rhs->getCoordinates()) <= maxD &&
                lhs->getCPU() >= rhs->getCPU());
    }
};
BOOST_AUTO_TEST_SUITE(VYTest)
BOOST_AUTO_TEST_CASE(VYCoordTest)
{
    std::cout << "test" << std::endl;
    VYCoordinate c1 = VYCoordinate(10, 10);
    VYCoordinate c2 = VYCoordinate(0, 0);
    BOOST_CHECK(c1.distanceFrom(c2) ==
                sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2)));
}
BOOST_AUTO_TEST_CASE(VYSubstrateTest)
{
    std::shared_ptr<VYSubstrateNode<>> sn1 = std::make_shared<VYSubstrateNode<>>(
        VYSubstrateNode<>(25, 10, 10));
    std::shared_ptr<VYSubstrateNode<>> sn2 = std::make_shared<VYSubstrateNode<>>(
        VYSubstrateNode<>(15, 0, 0));
    std::shared_ptr<VYSubstrateNode<>> sn3 = std::make_shared<VYSubstrateNode<>>(
        VYSubstrateNode<>(10, -10, -10));
    std::shared_ptr<VYSubstrateLink<>> l1 = std::make_shared<VYSubstrateLink<>>(
        VYSubstrateLink<>(5, 2, sn1->getId(), sn2->getId()));
    std::shared_ptr<VYSubstrateLink<>> l2 = std::make_shared<VYSubstrateLink<>>(
        VYSubstrateLink<>(1, 3, sn1->getId(), sn3->getId()));
    Network<VYSubstrateNode<>, VYSubstrateLink<>> substrateNetwork =
        Network<VYSubstrateNode<>, VYSubstrateLink<>>();

    substrateNetwork.addNode(sn1);
    substrateNetwork.addNode(sn2);
    substrateNetwork.addNode(sn3);
    substrateNetwork.addLink(l1);
    substrateNetwork.addLink(l2);

    VYVirtualNode<> vn1(VYVirtualNode<>(10, 0, 0));

    BOOST_CHECK(substrateNetwork.getLinksForNodeId(0)->size() == 2);
    BOOST_CHECK(!sn1->hasResources(32));
}
BOOST_AUTO_TEST_CASE(VYVirtualTest)
{
    std::function<std::shared_ptr<std::pair<double, double>>(VYVirtualNetRequest<> * vnr)>
        getRevenue = [](VYVirtualNetRequest<> *vnr) -> std::shared_ptr<std::pair<double, double>> {
        std::shared_ptr<std::pair<double, double>> rev(new std::pair<double, double>(0, 0));
        const std::shared_ptr<std::vector<std::shared_ptr<VYVirtualNode<>>>> n =
            vnr->getVN()->getAllNodes();
        for (int i = 0; i < n->size(); ++i) {
            rev->first += n->at(i)->getCPU();
        }
        const std::shared_ptr<std::vector<std::shared_ptr<VYVirtualLink<>>>> l =
            vnr->getVN()->getAllLinks();
        for (int j = 0; j < l->size(); ++j) {
            rev->second += l->at(j)->getBandwidth();
        }
        return rev;
    };

    std::function<std::shared_ptr<std::pair<double, double>>(VYVirtualNetRequest<> * vnr)>
        getCost = [](VYVirtualNetRequest<> *vnr) -> std::shared_ptr<std::pair<double, double>> {
        std::shared_ptr<std::pair<double, double>> cost(new std::pair<double, double>(0, 0));
        const std::shared_ptr<std::vector<std::shared_ptr<VYVirtualNode<>>>> n =
            vnr->getVN()->getAllNodes();
        for (int i = 0; i < n->size(); ++i) {
            cost->first += n->at(i)->getCPU();
        }
        const std::shared_ptr<std::vector<std::shared_ptr<VYVirtualLink<>>>> l =
            vnr->getVN()->getAllLinks();
        for (int j = 0; j < l->size(); ++j) {
            cost->second += l->at(j)->getPathLength() * l->at(j)->getBandwidth();
        }
        return cost;
    };

    std::shared_ptr<Network<VYVirtualNode<>, VYVirtualLink<>>> Vnet1(
        new Network<VYVirtualNode<>, VYVirtualLink<>>());
    std::shared_ptr<VYVirtualNode<>> vn11(new VYVirtualNode<>(10, 10, 10));
    std::shared_ptr<VYVirtualNode<>> vn12(new VYVirtualNode<>(2, 0, 0));
    std::shared_ptr<VYVirtualNode<>> vn13(new VYVirtualNode<>(5, -10, -10));
    std::shared_ptr<VYVirtualLink<>> l11(new VYVirtualLink<>(5, 5, vn11->getId(), vn13->getId()));
    std::shared_ptr<VYVirtualLink<>> l12(new VYVirtualLink<>(2, 1, vn12->getId(), vn13->getId()));
    Vnet1->addNode(vn11);
    Vnet1->addNode(vn12);
    Vnet1->addNode(vn13);
    Vnet1->addLink(l11);
    Vnet1->addLink(l12);

    //const std::shared_ptr<std::set<int>> setT =
    double d = 3.0;
    ReachabilityCondition cond;
    cond(vn11, Vnet1->getLinksForNodeId(vn11->getId()), vn12, d);
    Vnet1->getNodesIDsWithConditions<ReachabilityCondition>(vn11, d);

    std::cout << Vnet1->getNodesIDsWithConditions<ReachabilityCondition>(vn11, d)->size()
              << std::endl;

    std::cout << "Revenue:  " << &getRevenue << std::endl;
    std::shared_ptr<VYVirtualNetRequest<>> vnr1(
        new VYVirtualNetRequest<>(Vnet1, 0, 100, 1, 5, 20, 0, 0));
    std::cout << "getting revenues" << std::endl;
    std::cout << vnr1->getNodeRevenue() << std::endl;
    std::cout << vnr1->getLinkRevenue() << std::endl;
    std::cout << vnr1->getNodeCost() << std::endl;
    std::cout << vnr1->getLinkCost() << std::endl;
}

BOOST_AUTO_TEST_CASE(VYNetworkBuilderTest)
{
    VYSubstrateNode<>(10, 10, 10);
    std::cout << IdGenerator::Instance()->peekId<VYSubstrateNode<>>() << std::endl;
    VYSubstrateNetworkBuilder<> b = VYSubstrateNetworkBuilder<>();
    std::shared_ptr<Network<VYSubstrateNode<>, VYSubstrateLink<>>> net = b.getNetwork();
    std::cout << net->getId() << std::endl;
    std::cout << "Pointer count : " << net.use_count() << std::endl;
    net.reset();
    net = b.getNetwork();
    std::cout << net->getId() << std::endl;
}
#if 0
BOOST_AUTO_TEST_CASE(VYVNRGenProcTest)
{
    adevs::Digraph<VYVNRGenerator<>::PTR_TYPE> vnr_embedding;
    VYVNRGenerator<>* gen = new VYVNRGenerator<> ();
    
    VYVNREmbeddingProc<>* proc = new VYVNREmbeddingProc<> ();
    
    vnr_embedding.add(gen);
    vnr_embedding.add(proc);
    
    vnr_embedding.couple(gen, gen->arrive, proc, proc->arrive);
    
    adevs::Simulator<VYVNRGenerator<>::ADEVS_IO_TYPE> sim(&vnr_embedding);
    
    while (sim.nextEventTime() < DBL_MAX)
    {
        sim.execNextEvent();
    }
}
#endif
BOOST_AUTO_TEST_CASE(VYVNRProcDigraphTest)
{
    typedef ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>
        RELEASE_ALGO_TYPE;
    //LoggerInit();
    VYSubstrateNetworkBuilder<> sb = VYSubstrateNetworkBuilder<>();
    std::shared_ptr<VYVineTwoStageEmbeddingAlgo<>> embeddingAlgo(new VYVineTwoStageEmbeddingAlgo<>(
        sb, std::make_shared<VYVineNodeEmbeddingAlgo<>>(VYVineNodeEmbeddingAlgo<>()),
        std::make_shared<VYVineLinkEmbeddingAlgo<>>(VYVineLinkEmbeddingAlgo<>())));
    std::shared_ptr<RELEASE_ALGO_TYPE> releaseAlgo(
        new ReleaseAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>(
            sb));
    VYVNREmbeddingProc<> *embeddingProc(new VYVNREmbeddingProc<>(embeddingAlgo));
    VYVNRReleaseProc<> *releaseProc(new VYVNRReleaseProc<>(releaseAlgo));
    VYVNRProcObserver<> *observer(new VYVNRProcObserver<>(sb.getNetwork()));
    VYVNRProcDigraph<> graph(embeddingProc, releaseProc, new VYVNRGenerator<>(), observer);
    std::cout << "created the graph" << std::endl;
    adevs::Simulator<VYVNRGenerator<>::ADEVS_IO_TYPE> sim(&graph);
    std::cout << "created the simulator" << std::endl;
    std::cout << std::endl;
    while (sim.nextEventTime() < DBL_MAX) {
        sim.execNextEvent();
    }
}
BOOST_AUTO_TEST_CASE(VYEmbeddingAlgoTest)
{
    //substrate network
    std::shared_ptr<Network<VYSubstrateNode<>, VYSubstrateLink<>>> sn(
        new Network<VYSubstrateNode<>, VYSubstrateLink<>>());

    std::shared_ptr<VYSubstrateNode<>> n1(new VYSubstrateNode<>(3.0, 0, 0));
    std::shared_ptr<VYSubstrateNode<>> n2(new VYSubstrateNode<>(5.0, 0, 0));
    std::shared_ptr<VYSubstrateNode<>> n3(new VYSubstrateNode<>(2.0, 0, 0));

    std::shared_ptr<VYSubstrateLink<>> l1(new VYSubstrateLink<>(6.0, 0, n1->getId(), n2->getId()));
    std::shared_ptr<VYSubstrateLink<>> l2(new VYSubstrateLink<>(5.0, 0, n1->getId(), n3->getId()));
    std::shared_ptr<VYSubstrateLink<>> l3(new VYSubstrateLink<>(3.0, 0, n2->getId(), n3->getId()));

    sn->addNode(n1);
    sn->addNode(n2);
    sn->addNode(n3);
    sn->addLink(l1);
    sn->addLink(l2);
    sn->addLink(l3);

    //Virtual net
    std::shared_ptr<Network<VYVirtualNode<>, VYVirtualLink<>>> vn(
        new Network<VYVirtualNode<>, VYVirtualLink<>>());

    std::shared_ptr<VYVirtualNode<>> m1(new VYVirtualNode<>(2.0, 0, 0));
    std::shared_ptr<VYVirtualNode<>> m2(new VYVirtualNode<>(2.0, 0, 0));

    std::shared_ptr<VYVirtualLink<>> k1(new VYVirtualLink<>(4.0, 0, m1->getId(), m2->getId()));
    vn->addNode(m1);
    vn->addNode(m2);
    vn->addLink(k1);

    std::shared_ptr<VYVirtualNetRequest<>> vnr(
        new VYVirtualNetRequest<>(vn, 15.2, 16.2, 0, 0, 2, nullptr, nullptr));
    VYVineNodeEmbeddingAlgo<> nodeAlgo;
    //VYVineEmbeddingAlgo<> algo (sn);
    nodeAlgo.embeddVNRNodes(sn, vnr);
}

/*
This test has been moved to experiments/tests/experiments-test
BOOST_AUTO_TEST_CASE(VYDBTes)
{
    //VYStatistics st;
    //st.processing_time = 0.01;
    //st.std_dev_link_stress = 1.75;
    //LoggerInit();
    vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();
    
    //exp.statisticsGenerated(st);
    exp.run();
    
    //std::shared_ptr<hiberlite::Database>
    std::string dbName ("/Users/Soroosh/Desktop/test.db");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
    //db->registerBeanClass<VYStatistics>();
    typedef vne::experiments::VineNodeMCFLinkExp<> experimentBean;
    db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
 */
BOOST_AUTO_TEST_CASE(VYGLPKTest)
{
    glp_prob *lp1 = glp_create_prob();
    std::cout << lp1 << std::endl;
    glp_prob *lp2 = glp_create_prob();
    std::cout << lp2 << std::endl;

    glp_erase_prob(lp1);
    glp_erase_prob(lp2);
}
BOOST_AUTO_TEST_SUITE_END()
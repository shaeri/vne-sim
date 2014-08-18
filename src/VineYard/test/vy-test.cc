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

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-virtual-node.h"
#include "Vineyard/vy-substrate-link.h"
#include "Vineyard/vy-virtual-link.h"
#include "Vineyard/vy-virtual-net-request.h"
#include "Vineyard/vy-substrate-network-builder.h"
#include "Vineyard/vy-vnr-proc-digraph.h"


#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace vne;
using namespace vne::vineyard;

void LoggerInit()
{
    boost::log::core::get()->set_filter
    (
     boost::log::trivial::severity >= boost::log::trivial::info
    );
}

BOOST_AUTO_TEST_SUITE (VYTest)
BOOST_AUTO_TEST_CASE(VYCoordTest)
{
    std::cout << "test" << std::endl;
    VYCoordinate c1 = VYCoordinate (10,10);
    VYCoordinate c2 = VYCoordinate (0,0);
    BOOST_CHECK(c1.distanceFrom(c2) == sqrt (pow(c1.first-c2.first, 2)+pow(c1.second-c2.second,2)));
}
BOOST_AUTO_TEST_CASE(VYSubstrateTest)
{
    std::shared_ptr<VYSubstrateNode<>> sn1 = std::make_shared<VYSubstrateNode<>>(VYSubstrateNode<> (25, 10, 10));
    std::shared_ptr<VYSubstrateNode<>> sn2 = std::make_shared<VYSubstrateNode<>>(VYSubstrateNode<> (10,0,0));
    std::shared_ptr<VYSubstrateNode<>> sn3 = std::make_shared<VYSubstrateNode<>>(VYSubstrateNode<> (10,-10,-10));
    std::shared_ptr<VYSubstrateLink<>> l1 = std::make_shared<VYSubstrateLink<>>(VYSubstrateLink<> (5,2,sn1->getId(),sn2->getId()));
    std::shared_ptr<VYSubstrateLink<>> l2 = std::make_shared<VYSubstrateLink<>>(VYSubstrateLink<> (1,3,sn1->getId(),sn3->getId()));
    Network<VYSubstrateNode<>, VYSubstrateLink<>> substrateNetwork = Network<VYSubstrateNode<>, VYSubstrateLink<>> ();
    
    substrateNetwork.addNode(sn1);
    substrateNetwork.addNode(sn2);
    substrateNetwork.addNode(sn3);
    substrateNetwork.addLink(l1);
    substrateNetwork.addLink(l2);
    
    std::shared_ptr<VYSubstrateLink<>> l3 = std::make_shared<VYSubstrateLink<>>(VYSubstrateLink<> (3,6,5,7));
    std::cout<<substrateNetwork.getLinksForNodeId(0)->at(0)->getCount() << std::endl;
    substrateNetwork.getLinksForNodeId(0)->at(0)->operator++();
    std::cout<<substrateNetwork.getLinksForNodeId(0)->at(0)->getCount() << std::endl;
    //substrateNetwork.getLinksForNodeId(0)->push_back(l3);
    BOOST_CHECK (substrateNetwork.getLinksForNodeId(0)->size()==2);
    BOOST_CHECK(!sn1->hasResources(32));
    /*
    std::cout << sn1->getCoordinates().distanceFrom (sn2->getCoordinates()) << std::endl;
    std::cout<< ++(*sn1) << std::endl;
    std::cout<< sn1->getCount() << std::endl;
    std::cout<< ++(*sn1) << std::endl;
    std::cout<< sn1->getCount() << std::endl;
    */
}
BOOST_AUTO_TEST_CASE(VYVirtualTest)
{
    std::function<std::shared_ptr<std::pair<double,double>>(VYVirtualNetRequest<>* vnr)> getRevenue  = [] (VYVirtualNetRequest<>* vnr) -> std::shared_ptr<std::pair<double,double>>
    {
        std::shared_ptr<std::pair<double,double>> rev (new std::pair<double,double>(0,0));
        const std::shared_ptr<std::vector<std::shared_ptr<const VYVirtualNode<>>>> n = vnr->getVN()->getAllNodes();
        for (int i=0;i<n->size();++i)
        {
            rev->first += n->at(i)->getCPU ();
            
        }
       const std::shared_ptr<std::vector<std::shared_ptr<const VYVirtualLink<>>>>  l = vnr->getVN ()->getAllLinks();
        for (int j=0;j<l->size();++j)
        {
            rev->second += l->at(j)->getBandwidth();
        }
        return rev;
    };
    
    std::function<std::shared_ptr<std::pair<double,double>>(VYVirtualNetRequest<>* vnr)> getCost  = [] (VYVirtualNetRequest<>* vnr) -> std::shared_ptr<std::pair<double,double>>
    {
        std::shared_ptr<std::pair<double,double>>  cost (new std::pair<double,double>(0,0));
        const std::shared_ptr<std::vector<std::shared_ptr<const VYVirtualNode<>>>> n = vnr->getVN()->getAllNodes();
        for (int i=0;i<n->size();++i)
        {
            cost->first += n->at(i)->getCPU ();
            
        }
        const std::shared_ptr<std::vector<std::shared_ptr<const VYVirtualLink<>>>>  l = vnr->getVN()->getAllLinks();
        for (int j=0;j<l->size();++j)
        {
            cost->second += l->at(j)->getPathLength() * l->at(j)->getBandwidth();
        }
        return cost;
    };
     
    std::shared_ptr<Network<VYVirtualNode<>, VYVirtualLink<>>> Vnet1(new Network<VYVirtualNode<>, VYVirtualLink<>>());
    std::shared_ptr<VYVirtualNode<>> vn11 (new VYVirtualNode<> (10,10,10));
    std::shared_ptr<VYVirtualNode<>> vn12 (new VYVirtualNode<> (2,0,0));
    std::shared_ptr<VYVirtualNode<>> vn13 (new VYVirtualNode<> (5,-10,-10));
    std::shared_ptr<VYVirtualLink<>> l11 (new VYVirtualLink<> (5,5,vn11->getId(),vn13->getId()));
    std::shared_ptr<VYVirtualLink<>> l12 (new VYVirtualLink<> (2,1,vn12->getId(),vn13->getId()));
    Vnet1->addNode(vn11);
    Vnet1->addNode(vn12);
    Vnet1->addNode(vn13);
    Vnet1->addLink(l11);
    Vnet1->addLink(l12);

    std::cout << "Revenue:  " << &getRevenue << std::endl;
    std::shared_ptr<VYVirtualNetRequest<>> vnr1  (new VYVirtualNetRequest<>(Vnet1, 0,100,1,5,20,0,0));
    std::cout << "getting revenues" << std::endl;
    std::cout << vnr1->getNodeRevenue() << std::endl;
    std::cout << vnr1->getLinkRevenue() << std::endl;
    std::cout << vnr1->getNodeCost() << std::endl;
    std::cout << vnr1->getLinkCost() << std::endl;
}
    

BOOST_AUTO_TEST_CASE(VYNetworkBuilderTest)
{
    VYSubstrateNode<>(10, 10,10);
    std::cout << IdGenerator::Instance()->peekId<VYSubstrateNode<>>() << std::endl;
    VYSubstrateNetworkBuilder<> b = VYSubstrateNetworkBuilder<> ();
    std::shared_ptr<Network<VYSubstrateNode<>, VYSubstrateLink<>>> net = b.getNetwork ();
    std::cout<< net->getId() << std::endl;
    std::cout << "Pointer count : " << net.use_count() << std::endl;
    net.reset();
    net = b.getNetwork ();
    std::cout<< net->getId() << std::endl;
    
}
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
BOOST_AUTO_TEST_CASE(VYVNRProcDigraphTest)
{
    LoggerInit();
    VYVNRProcDigraph<> graph = VYVNRProcDigraph<>();
    std::cout<< "created the graph" << std::endl;
    adevs::Simulator<VYVNRGenerator<>::ADEVS_IO_TYPE> sim(&graph);
    std::cout<< "created the simulator" << std::endl;
    std::cout << std::endl;
    while (sim.nextEventTime() < DBL_MAX)
    {
        sim.execNextEvent();
    }
}
BOOST_AUTO_TEST_SUITE_END ()
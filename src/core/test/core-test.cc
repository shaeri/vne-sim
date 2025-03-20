/**
 * @file network.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 3, 2014
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
#include <tuple>
#include <string>
#include <typeinfo>

#include "core/core-types.h"
#include "core/link.h"
#include "core/network.h"
#include "core/virtual-network-request.h"
#include "core/substrate-node.h"
#include "core/substrate-link.h"
#include "adevs/adevs.h"
#include "core/substrate-network.h"
#include "core/virtual-network.h"
#include "core/coordinate.h"

#include "core/config-manager.h"
#include "core/rng.h"

using namespace vne;
//boost::logging::core::get()->add_global_attribute("Scope", boost::make_shared< attrs::named_scope >());
/*
 * Node test suites
 */
typedef vne::Node<int, double, std::string> node_fixture;

typedef vne::Link<int, double, std::string> t1;
typedef vne::Node<int, double, std::string> t2;
//extern class IdGenerator;
//extern std::map<const char*, int> vne::IdGenerator::m_map;

BOOST_AUTO_TEST_SUITE(NodeTest)
/*
BOOST_AUTO_TEST_CASE(Resources)
{
	std::tuple<int, double, std::string> t = std::make_tuple(22, 4.2, "test");
	setResources(t);
	BOOST_CHECK(std::get<0>(getResources()) == 22);
	BOOST_CHECK(std::get<1>(getResources()) == 4.2);
	BOOST_CHECK(std::get<2>(getResources()).compare("test") == 0);
}
*/
BOOST_AUTO_TEST_CASE(TypeId)
{
    vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(
        Resources<int, double, std::string>(5, 3.5, "yoyo"), vne::Entity_t::virt);
    vne::Node<int, double> n1 = vne::Node<int, double>(Resources<int, double>(5, 3.5),
                                                       vne::Entity_t::virt);
    //BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
    //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
    BOOST_CHECK(n.getType() == n1.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
    int nCount = vne::IdGenerator::Instance()->peekId<t2>();
    for (int i = 0; i < 5; i++) {
        vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(
            Resources<int, double, std::string>(3, 2.5, "test"), vne::Entity_t::virt);
        //BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
        //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
        BOOST_REQUIRE(n.getId() == nCount + i);
    }
}
BOOST_AUTO_TEST_SUITE_END()

/*
 * Link test suites
 */
//typedef vne::Link<int, double, std::string> link_fixture;
BOOST_AUTO_TEST_SUITE(LinkTest)

/*
BOOST_AUTO_TEST_CASE(Resources)
{
	std::tuple<int, double, std::string> t = std::make_tuple(22, 4.2, "test");
	vne::Link<int, double, std::string> link;
	link.setResources(t);
	BOOST_CHECK(std::get<0>(link.getResources()) == 22);
	BOOST_CHECK(std::get<1>(link.getResources()) == 4.2);
	BOOST_CHECK(std::get<2>(link.getResources()).compare("test") == 0);
}
*/
BOOST_AUTO_TEST_CASE(TypeId)
{
    vne::Link<int, double, std::string> l = vne::Link<int, double, std::string>(
        Resources<int, double, std::string>(), vne::Entity_t::virt, 0, 0);
    vne::Link<int, double, std::string> l2 = vne::Link<int, double, std::string>(
        1, 2.0, "shaeri", vne::Entity_t::virt, 0, 0);
    //BOOST_TEST_MESSAGE ("New node object is: " << int(l.getType()));
    //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
    BOOST_CHECK(l2.getType() == l.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
    vne::Link<int, double, std::string> l1 = vne::Link<int, double, std::string>(
        5, 7.9, "test", vne::Entity_t::virt, 0, 0);
    int lCount = vne::IdGenerator::Instance()->peekId<t1>();
    for (int i = 0; i < 5; i++) {
        vne::Link<int, double, std::string> l = vne::Link<int, double, std::string>(
            5, 7.9, "test", vne::Entity_t::virt, 0, 0);
        //BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
        //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
        BOOST_REQUIRE(l.getId() == lCount + i);
    }
}
BOOST_AUTO_TEST_SUITE_END()

/*
 * Generator tests
 */
BOOST_AUTO_TEST_SUITE(NetworkTest)
/*BOOST_AUTO_TEST_CASE(AddSimpleNodeTest)
 {
 //typedef vne::Node<int> t1;
 //int nCount = vne::IdGenerator::peekId<t1>();
 vne::SimpleNetwork net = vne::SimpleNetwork();
 for (int i = 0; i < 5; i++)
 {
 std::shared_ptr<vne::Node<int>> n = std::make_shared<vne::Node<int>>(
 vne::Entity_t::substrate);
 net.addNode(n);
 }
 }*/
BOOST_AUTO_TEST_CASE(createNetwork)
{
    vne::Network<vne::Node<int>, vne::Link<int>> n =
        vne::Network<vne::Node<int>, vne::Link<int>>();

    std::shared_ptr<vne::Node<int>> node1Ptr = std::make_shared<vne::Node<int>>(
        Resources<int>(5), vne::Entity_t::substrate);
    n.addNode(node1Ptr);
    std::shared_ptr<const vne::Node<int>> anotherPtr = n.getNode(node1Ptr->getId());
    BOOST_REQUIRE(anotherPtr->getId() == node1Ptr->getId());
    anotherPtr.reset();
    std::shared_ptr<vne::Node<int>> node2Ptr = std::make_shared<vne::Node<int>>(
        Resources<int>(6), vne::Entity_t::substrate);
    BOOST_REQUIRE(node1Ptr->getId() != node2Ptr->getId());
    n.addNode(node2Ptr);
    std::shared_ptr<vne::Node<int>> node3Ptr = std::make_shared<vne::Node<int>>(
        Resources<int>(5), vne::Entity_t::substrate);
    n.addNode(node3Ptr);

    std::shared_ptr<vne::Link<int>> l1Ptr = std::make_shared<vne::Link<int>>(
        5, vne::Entity_t::substrate, node1Ptr->getId(), node2Ptr->getId());
    BOOST_REQUIRE(l1Ptr->getNodeFromId() != l1Ptr->getNodeToId());
    n.addLink(l1Ptr);

    std::shared_ptr<vne::Link<int>> l2Ptr = std::make_shared<vne::Link<int>>(
        5, vne::Entity_t::substrate, node1Ptr->getId(), node3Ptr->getId());
    BOOST_REQUIRE(l2Ptr->getNodeFromId() != l1Ptr->getNodeToId());
    n.addLink(l2Ptr);

    std::shared_ptr<const vne::Link<int>> anotherLptr = n.getLinkBetweenNodes(node2Ptr->getId(),
                                                                              node3Ptr->getId());

    BOOST_CHECK(n.getLinksForNodeId(node1Ptr->getId())->size() == 2);
    BOOST_CHECK(n.getLinksForNodeId(node2Ptr->getId())->size() == 1);

    BOOST_REQUIRE(n.getLinkBetweenNodes(node1Ptr->getId(), node1Ptr->getId()) == nullptr);
    BOOST_REQUIRE(n.getLinkBetweenNodes(node2Ptr->getId(), node2Ptr->getId()) == nullptr);
    BOOST_REQUIRE(n.getLinkBetweenNodes(node3Ptr->getId(), node3Ptr->getId()) == nullptr);

    BOOST_REQUIRE(n.getLinkBetweenNodes(node2Ptr->getId(), node3Ptr->getId()) == nullptr);
    BOOST_REQUIRE(n.getLinkBetweenNodes(node3Ptr->getId(), node2Ptr->getId()) == nullptr);

    BOOST_REQUIRE(n.getLinkBetweenNodes(node1Ptr->getId(), node3Ptr->getId())->getId() ==
                  l2Ptr->getId());
    BOOST_REQUIRE(n.getLinkBetweenNodes(node3Ptr->getId(), node1Ptr->getId())->getId() ==
                  l2Ptr->getId());
    BOOST_REQUIRE(n.getLinkBetweenNodes(node1Ptr->getId(), node2Ptr->getId())->getId() ==
                  l1Ptr->getId());
    BOOST_REQUIRE(n.getLinkBetweenNodes(node2Ptr->getId(), node1Ptr->getId())->getId() ==
                  l1Ptr->getId());

    /*std::shared_ptr<vne::Link<int>> linkPtr = std::make_shared<vne::Link<int>>(
	 vne::Entity_t::substrate, 0, 0);
	 net.addLink(linkPtr);
	 net.addNode(nodePtr);

	adevs::crand *rand = new adevs::crand();
	rand->set_seed(1);
	adevs::rv randvar =  adevs::rv(rand);
	for (int i=0; i<10; i++)
	{
		std::cout<< randvar.exponential(1) << std::endl;
	}
	*/
}
BOOST_AUTO_TEST_SUITE_END()
/*
 * Network tests
 */
BOOST_AUTO_TEST_SUITE(GeneratorTest)
BOOST_AUTO_TEST_CASE(Resource)
{
    int lCount = vne::IdGenerator::Instance()->peekId<t1>();
    int nCount = vne::IdGenerator::Instance()->peekId<t2>();
    for (int i = 0; i < 100; i++) {
        vne::Link<int, double, std::string> l = vne::Link<int, double, std::string>(
            1, 2, "test", vne::Entity_t::virt, 0, 0);
        vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(
            Resources<int, double, std::string>(7, 6.5, "TEST"), vne::Entity_t::virt);
        BOOST_CHECK(l.getId() == lCount + i);
        BOOST_CHECK(n.getId() == nCount + i);
    }
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(VNRequestTest)
BOOST_AUTO_TEST_CASE(BaseVNRequestTest)
{
    std::shared_ptr<Network<VirtualNode<int>, VirtualLink<int>>> vn =
        std::make_shared<Network<VirtualNode<int>, VirtualLink<int>>>();
    vne::VirtualNetworkRequest<Network<VirtualNode<int>, VirtualLink<int>>> req =
        vne::VirtualNetworkRequest<Network<VirtualNode<int>, VirtualLink<int>>>(vn, 1, 10);
    BOOST_CHECK(vn->getId() == req.getVN()->getId());
    std::cout << vn.use_count() << endl;
}
#if 0
BOOST_AUTO_TEST_CASE(SimpleRequestProcessorTest)
{
	vne::SimpleRequestProcessor simpleProc =  vne::SimpleRequestProcessor ();
	vne::SimpleRequestProcessor::ADEVS_IO_TYPE a;
	std::cout << typeid(vne::SimpleRequestProcessor::ADEVS_IO_TYPE).name();
	simpleProc.delta_int ();
}
#endif
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SubstrateNetworkTest)
BOOST_AUTO_TEST_CASE(SubstrateVirtualNodeTest)
{
    std::shared_ptr<SubstrateNode<double, int>> sn1(
        new SubstrateNode<double, int>(Resources<double, int>(10.0, 10)));
    std::shared_ptr<VirtualNode<double, int>> vn1(
        new VirtualNode<double, int>(Resources<double, int>(5.0, 4)));
    std::shared_ptr<VirtualNode<double, int>> vn2(
        new VirtualNode<double, int>(Resources<double, int>(3.0, 5)));
    std::shared_ptr<VirtualNode<double, int>> vn3(
        new VirtualNode<double, int>(Resources<double, int>(1.0, 1)));

    BOOST_CHECK(sn1->hasResources(vn1->getResources()));
    BOOST_CHECK(sn1->hasResources(vn2->getResources()));
    sn1->embedNode(vn1);
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", "
                             << std::get<1>(sn1->getResources()) << endl;
    sn1->embedNode(vn2);
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", "
                             << std::get<1>(sn1->getResources()) << endl;
    sn1->embedNode(vn3);
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", "
                             << std::get<1>(sn1->getResources()) << endl;
    BOOST_CHECK(std::get<0>(sn1->getResources()) == 1.0);
    BOOST_CHECK(std::get<1>(sn1->getResources()) == 0);
    BOOST_LOG_TRIVIAL(debug) << "VN2 pointer count Before free: " << vn2.use_count() << endl;
    sn1->freeResources(vn2->getId());
    BOOST_LOG_TRIVIAL(debug) << "VN2 pointer count After free: " << vn2.use_count() << endl;
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", "
                             << std::get<1>(sn1->getResources()) << endl;
    //BOOST_LOG_TRIVIAL(debug) << "Sn1 pointer count " << sn1.use_count () << endl;
    /*while (true)
    {
        SubstrateLink<int>* sl1 = new SubstrateLink<int> (std::make_tuple(10), Entity_t::virt, 1, 2);
        delete sl1;
    }
    */
}
BOOST_AUTO_TEST_CASE(SubstrateVirtualLinkTest)
{
    std::shared_ptr<SubstrateLink<double, int>> sl1(
        new SubstrateLink<double, int>(Resources<double, int>(10.0, 10), 1, 2));
    std::shared_ptr<VirtualLink<double, int>> vl1(
        new VirtualLink<double, int>(Resources<double, int>(5.0, 4), 1, 2));
    std::shared_ptr<VirtualLink<double, int>> vl2(
        new VirtualLink<double, int>(Resources<double, int>(3.0, 5), 1, 2));
    std::shared_ptr<VirtualLink<double, int>> vl3(
        new VirtualLink<double, int>(Resources<double, int>(1.0, 1), 1, 2));

    BOOST_CHECK(sl1->hasResources(vl1->getResources()));
    BOOST_CHECK(sl1->hasResources(vl2->getResources()));
    sl1->embedLink(vl1);
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sl1->getResources()) << ", "
                             << std::get<1>(sl1->getResources()) << endl;
    sl1->embedLink(vl2);
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sl1->getResources()) << ", "
                             << std::get<1>(sl1->getResources()) << endl;
    sl1->embedLink(vl3);
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sl1->getResources()) << ", "
                             << std::get<1>(sl1->getResources()) << endl;
    BOOST_CHECK(std::get<0>(sl1->getResources()) == 1.0);
    BOOST_CHECK(std::get<1>(sl1->getResources()) == 0);
    BOOST_LOG_TRIVIAL(debug) << "VL2 pointer count Before free: " << vl2.use_count() << endl;
    sl1->freeResources(vl2->getId());
    BOOST_LOG_TRIVIAL(debug) << "VL2 pointer count After free: " << vl2.use_count() << endl;
    BOOST_LOG_TRIVIAL(debug) << std::get<0>(sl1->getResources()) << ", "
                             << std::get<1>(sl1->getResources()) << endl;
    //BOOST_LOG_TRIVIAL(debug) << "Sn1 pointer count " << sn1.use_count () << endl;
    /*while (true)
    {
        SubstrateLink<int>* sl1 = new SubstrateLink<int> (std::make_tuple(10), Entity_t::virt, 1, 2);
        delete sl1;
    }
    */
}
#if 0
BOOST_AUTO_TEST_CASE(SubstrateNetworkTestCase)
{

   SubstrateNetwork<SubstrateNode<int>, SubstrateLink<int>> n =
            vne::SubstrateNetwork<SubstrateNode<int>, SubstrateLink<int>>();

	std::shared_ptr<vne::SubstrateNode<int>> node1Ptr = std::make_shared<vne::SubstrateNode<int>>(Resources<int>(5));
	n.addNode(node1Ptr);
	std::shared_ptr<vne::SubstrateNode<int>> anotherPtr = n.getNode(node1Ptr->getId());
	BOOST_REQUIRE(anotherPtr->getId() == node1Ptr->getId());
	anotherPtr.reset();
	std::shared_ptr<vne::SubstrateNode<int>> node2Ptr = std::make_shared<vne::SubstrateNode<int>>(
			Resources<int>(6));
	BOOST_REQUIRE(node1Ptr->getId() != node2Ptr->getId());
	n.addNode(node2Ptr);
	std::shared_ptr<vne::SubstrateLink<int>> lPtr = std::make_shared<vne::SubstrateLink<int>>(
			12, node1Ptr->getId(), node2Ptr->getId());
	BOOST_REQUIRE(lPtr->getNodeFromId() != lPtr->getNodeToId());
	n.addLink(lPtr);
	BOOST_CHECK(n.getLinksForNodeId(node1Ptr->getId())->size() == 1);
	BOOST_CHECK(n.getLinksForNodeId(node2Ptr->getId())->size() == 1);
}
BOOST_AUTO_TEST_CASE(VirtualNetworkTestCase)
{
   VirtualNetwork<VirtualNode<int>, VirtualLink<int>> n =
            vne::VirtualNetwork<VirtualNode<int>, VirtualLink<int>>();

	std::shared_ptr<vne::VirtualNode<int>> node1Ptr = std::make_shared<vne::VirtualNode<int>>(Resources<int>(5));
	n.addNode(node1Ptr);
	std::shared_ptr<vne::VirtualNode<int>> anotherPtr = n.getNode(node1Ptr->getId());
	BOOST_REQUIRE(anotherPtr->getId() == node1Ptr->getId());
	anotherPtr.reset();
	std::shared_ptr<vne::VirtualNode<int>> node2Ptr = std::make_shared<vne::VirtualNode<int>>(
			Resources<int>(6));
	BOOST_REQUIRE(node1Ptr->getId() != node2Ptr->getId());
	n.addNode(node2Ptr);
	std::shared_ptr<vne::VirtualLink<int>> lPtr = std::make_shared<vne::VirtualLink<int>>(
			12, node1Ptr->getId(), node2Ptr->getId());
	BOOST_REQUIRE(lPtr->getNodeFromId() != lPtr->getNodeToId());
	n.addLink(lPtr);
	BOOST_CHECK(n.getLinksForNodeId(node1Ptr->getId())->size() == 1);
	BOOST_CHECK(n.getLinksForNodeId(node2Ptr->getId())->size() == 1);
}
#endif
BOOST_AUTO_TEST_CASE(SubstrateNetworkTestCase)
{
    Network<SubstrateNode<int>, SubstrateLink<int>> n =
        vne::Network<SubstrateNode<int>, SubstrateLink<int>>();

    std::shared_ptr<vne::SubstrateNode<int>> node1Ptr = std::make_shared<vne::SubstrateNode<int>>(
        Resources<int>(5));
    n.addNode(node1Ptr);

    std::shared_ptr<const vne::SubstrateNode<int>> anotherPtr = n.getNode(node1Ptr->getId());
    BOOST_REQUIRE(anotherPtr->getId() == node1Ptr->getId());
    anotherPtr.reset();
    std::shared_ptr<vne::SubstrateNode<int>> node2Ptr = std::make_shared<vne::SubstrateNode<int>>(
        Resources<int>(6));
    BOOST_REQUIRE(node1Ptr->getId() != node2Ptr->getId());
    n.addNode(node2Ptr);
    std::shared_ptr<vne::SubstrateLink<int>> lPtr = std::make_shared<vne::SubstrateLink<int>>(
        12, node1Ptr->getId(), node2Ptr->getId());

    BOOST_REQUIRE(lPtr->getNodeFromId() != lPtr->getNodeToId());
    n.addLink(lPtr);
    BOOST_CHECK(n.getLinksForNodeId(node1Ptr->getId())->size() == 1);
    BOOST_CHECK(n.getLinksForNodeId(node2Ptr->getId())->size() == 1);
    BOOST_CHECK(n.getLinksForNodeId(27) == nullptr);
}
BOOST_AUTO_TEST_CASE(VirtualNetworkTestCase)
{
    Network<VirtualNode<int>, VirtualLink<int>> n =
        vne::Network<VirtualNode<int>, VirtualLink<int>>();

    std::shared_ptr<vne::VirtualNode<int>> node1Ptr = std::make_shared<vne::VirtualNode<int>>(
        Resources<int>(5));
    n.addNode(node1Ptr);
    std::shared_ptr<const vne::VirtualNode<int>> anotherPtr = n.getNode(node1Ptr->getId());
    BOOST_REQUIRE(anotherPtr->getId() == node1Ptr->getId());
    anotherPtr.reset();
    std::shared_ptr<vne::VirtualNode<int>> node2Ptr = std::make_shared<vne::VirtualNode<int>>(
        Resources<int>(6));
    BOOST_REQUIRE(node1Ptr->getId() != node2Ptr->getId());
    n.addNode(node2Ptr);
    std::shared_ptr<vne::VirtualLink<int>> lPtr = std::make_shared<vne::VirtualLink<int>>(
        12, node1Ptr->getId(), node2Ptr->getId());
    BOOST_REQUIRE(lPtr->getNodeFromId() != lPtr->getNodeToId());
    n.addLink(lPtr);
    BOOST_CHECK(n.getLinksForNodeId(node1Ptr->getId())->size() == 1);
    BOOST_CHECK(n.getLinksForNodeId(node2Ptr->getId())->size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CoordinateTest)
BOOST_AUTO_TEST_CASE(Cartesian2DTest)
{
    CartesianCoord2D<int, int> _1 = CartesianCoord2D<int, int>(0, 0);
    CartesianCoord2D<int, int> _2 = CartesianCoord2D<int, int>(8, 8);
    cout << _2.distanceFrom(_1) << endl;
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Config)
BOOST_AUTO_TEST_CASE(ConfigTest)
{
    cout << ConfigManager::Instance()->getConfig<string>("vineyard", "SubstrateNetwork.path")
         << endl;
    //std::cout << boost::filesystem::initial_path() << std::endl;
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RNG)
BOOST_AUTO_TEST_CASE(RNGTest)
{
    class TestObj : public RNGSubscriber
    {
    };
    TestObj t1;
    const gsl_rng *r = vne::RNG::Instance()->getMyRNG(&t1);
    std::cout << gsl_rng_min(r) << std::endl;
    std::cout << gsl_rng_max(r) << std::endl;
    std::cout << gsl_rng_uniform(r) << std::endl;
    std::cout << gsl_rng_name(r) << std::endl;
    vne::RNG::Instance()->unsubscribe(&t1);
}
BOOST_AUTO_TEST_SUITE_END()

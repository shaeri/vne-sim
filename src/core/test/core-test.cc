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

//#include "simple-impl/simple-network.h"
#include "core/core-types.h"
#include "core/link.h"
#include "core/network.h"
#include "core/request.h"
#include "simple-impl/simple-request-proc.h"
#include "core/substrate-node.h"
#include "core/substrate-link.h"
#include "adevs.h"

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

BOOST_AUTO_TEST_SUITE (NodeTest)
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
	vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>
        (Resources<int, double, std::string>(5, 3.5, "yoyo"), vne::Entity_t::virt);
	vne::Node<int, double> n1 = vne::Node<int, double>
        (Resources<int, double>(5, 3.5), vne::Entity_t::virt);
	//BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
	//BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
	BOOST_CHECK(n.getType() == n1.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
	int nCount = vne::IdGenerator::peekId<t2>();
	for (int i = 0; i < 5; i++)
	{
		vne::Node<int, double, std::string> n = vne::Node<int, double,
        std::string>( Resources<int, double, std::string>(3, 2.5, "test"), vne::Entity_t::virt);
		//BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
		//BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
		BOOST_REQUIRE(n.getId() == nCount + i);
	}
}
BOOST_AUTO_TEST_SUITE_END()

/*
 * Link test suites
 */
typedef vne::Link<int, double, std::string> link_fixture;
BOOST_FIXTURE_TEST_SUITE (LinkTest, link_fixture)

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
			vne::Entity_t::virt, 0, 0);
	//BOOST_TEST_MESSAGE ("New node object is: " << int(l.getType()));
	//BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
	BOOST_CHECK(getType() != l.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
	int lCount = vne::IdGenerator::peekId<t1>();
	for (int i = 0; i < 5; i++)
	{
		vne::Link<int, double, std::string> l = vne::Link<int, double,
				std::string>(vne::Entity_t::virt, 0, 0);
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
	//typedef vne::Node<int> t1;
	//int nCount = vne::IdGenerator::peekId<t1>();
	//vne::Network<vne::Node<int>, int, vne::Link<int>, int> net = vne::Network<vne::Node<int>, int, vne::Link<int>, int> ();
	vne::Network<vne::Node<int>, vne::Link<int>> n = vne::Network<
			vne::Node<int>, vne::Link<int>>();

	std::shared_ptr<vne::Node<int>> node1Ptr = std::make_shared<vne::Node<int>>(
			Resources<int>(5), vne::Entity_t::substrate);
	n.addNode(node1Ptr);
	std::shared_ptr<vne::Node<int>> anotherPtr = n.getNode(node1Ptr->getId());
	BOOST_REQUIRE(anotherPtr->getId() == node1Ptr->getId());
	anotherPtr.reset();
	std::shared_ptr<vne::Node<int>> node2Ptr = std::make_shared<vne::Node<int>>(
			Resources<int>(6), vne::Entity_t::substrate);
	BOOST_REQUIRE(node1Ptr->getId() != node2Ptr->getId());
	n.addNode(node2Ptr);
	std::shared_ptr<vne::Link<int>> lPtr = std::make_shared<vne::Link<int>>(
			vne::Entity_t::substrate, node1Ptr->getId(), node2Ptr->getId());
	BOOST_REQUIRE(lPtr->getNodeFromId() != lPtr->getNodeToId());
	n.addLink(lPtr);
	BOOST_CHECK(n.getLinksForNodeId(node1Ptr->getId())->size() == 1);
	BOOST_CHECK(n.getLinksForNodeId(node2Ptr->getId())->size() == 1);
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
	int lCount = vne::IdGenerator::peekId<t1>();
	int nCount = vne::IdGenerator::peekId<t2>();
	for (int i = 0; i < 100; i++)
	{
		vne::Link<int, double, std::string> l = vne::Link<int, double,
				std::string>(vne::Entity_t::virt, 0, 0);
		vne::Node<int, double, std::string> n = vne::Node<int, double,
        std::string>(Resources<int, double, std::string>(7, 6.5, "TEST"), vne::Entity_t::virt);
		BOOST_CHECK(l.getId() == lCount + i);
		BOOST_CHECK(n.getId() == nCount + i);
	}
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RequestTest)
BOOST_AUTO_TEST_CASE(RequestTest)
{
	vne::Request<vne::Node<int>, vne::Link<int>> req =
			vne::Request<vne::Node<int>, vne::Link<int>> ();
	BOOST_CHECK(std::get<0>(req.getNodeResources(0))!=1);
	BOOST_CHECK(std::get<0>(req.getLinkResources(0))!=10);
}

BOOST_AUTO_TEST_CASE(SimpleRequestProcessorTest)
{
	vne::SimpleRequestProcessor simpleProc =  vne::SimpleRequestProcessor ();
	vne::SimpleRequestProcessor::ADEVS_IO_TYPE a;
	std::cout << typeid(vne::SimpleRequestProcessor::ADEVS_IO_TYPE).name();
	simpleProc.delta_int ();
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SubstrateNetworkTest)
BOOST_AUTO_TEST_CASE(SubstrateNodeTest)
{
	std::shared_ptr<SubstrateNode<double,int>>  sn1   (new SubstrateNode<double,int> (Resources<double,int> (10.0, 10)));
	std::shared_ptr<VirtualNode<double,int>> vn1 (new VirtualNode<double,int> (Resources<double,int> (5.0, 4)));
	std::shared_ptr<VirtualNode<double, int>> vn2 (new VirtualNode<double,int> (Resources<double,int> (3.0, 5)));
	std::shared_ptr<VirtualNode<double, int>> vn3 (new VirtualNode<double, int> (Resources<double,int> (1.0, 1)));

	BOOST_CHECK (sn1->hasResources(vn1->getResources()));
	BOOST_CHECK (sn1->hasResources(vn2->getResources()));
	sn1->embedNode(vn1);
	BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", " << std::get<1>(sn1->getResources()) << endl;
	sn1->embedNode(vn2);
	BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", " << std::get<1>(sn1->getResources()) << endl;
	sn1->embedNode(vn3);
	BOOST_LOG_TRIVIAL(debug)  << std::get<0>(sn1->getResources()) << ", " << std::get<1>(sn1->getResources()) << endl;
	BOOST_CHECK(std::get<0>(sn1->getResources()) == 1.0);
	BOOST_CHECK(std::get<1>(sn1->getResources()) == 0);
	BOOST_LOG_TRIVIAL(debug) << "VN2 pointer count Before free: " << vn2.use_count () << endl;
	sn1->freeResources(vn2->getId ());
	BOOST_LOG_TRIVIAL(debug) << "VN2 pointer count After free: " << vn2.use_count () << endl;
	BOOST_LOG_TRIVIAL(debug) << std::get<0>(sn1->getResources()) << ", " << std::get<1>(sn1->getResources()) << endl;
	//BOOST_LOG_TRIVIAL(debug) << "Sn1 pointer count " << sn1.use_count () << endl;
    /*while (true)
    {
        SubstrateLink<int>* sl1 = new SubstrateLink<int> (std::make_tuple(10), Entity_t::virt, 1, 2);
        delete sl1;
    }
    */
}
BOOST_AUTO_TEST_SUITE_END()

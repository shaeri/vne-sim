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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CoreTest 

#include <boost/test/unit_test.hpp>
#include <tuple>
#include <string>

#include "../simple-network.h"
#include "../core-types.h"
#include "../link.h"
#include "../network.h"
//#include "../node.h"

/*
 * Node test suites
 */
typedef vne::Node<int, double, std::string> node_fixture;

typedef vne::Link<int, double, std::string> t1;
typedef vne::Node<int, double, std::string> t2;
//extern class IdGenerator;
//extern std::map<const char*, int> vne::IdGenerator::m_map;

BOOST_FIXTURE_TEST_SUITE (NodeTest, node_fixture)

BOOST_AUTO_TEST_CASE(Resources)
{
	std::tuple<int, double, std::string> t = std::make_tuple(22, 4.2, "test");
	setResources(t);
	BOOST_CHECK(std::get<0>(getResources()) == 22);
	BOOST_CHECK(std::get<1>(getResources()) == 4.2);
	BOOST_CHECK(std::get<2>(getResources()).compare("test") == 0);
}
BOOST_AUTO_TEST_CASE(TypeId)
{
	vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(
			vne::Entity_t::virt);
	//BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
	//BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
	BOOST_CHECK(getType() != n.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
	int nCount = vne::IdGenerator::peekId<t2>();
	for (int i = 0; i < 5; i++)
	{
		vne::Node<int, double, std::string> n = vne::Node<int, double,
				std::string>(vne::Entity_t::virt);
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

BOOST_AUTO_TEST_CASE(Resources)
{
	std::tuple<int, double, std::string> t = std::make_tuple(22, 4.2, "test");
	vne::Link<int, double, std::string> link;
	link.setResources(t);
	BOOST_CHECK(std::get<0>(link.getResources()) == 22);
	BOOST_CHECK(std::get<1>(link.getResources()) == 4.2);
	BOOST_CHECK(std::get<2>(link.getResources()).compare("test") == 0);
}
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
BOOST_AUTO_TEST_CASE(AddSimpleNodeTest)
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
}
BOOST_AUTO_TEST_CASE(createNetwork)
{
	//typedef vne::Node<int> t1;
	//int nCount = vne::IdGenerator::peekId<t1>();
	//vne::Network<vne::Node<int>, int, vne::Link<int>, int> net = vne::Network<vne::Node<int>, int, vne::Link<int>, int> ();
	vne::Network<vne::Node<int,int>,vne::Link<std::string>> n = vne::Network<vne::Node<int,int>,vne::Link<std::string>> ();
	/*
	std::shared_ptr<vne::Node<int>> nodePtr = std::make_shared<vne::Node<int>>(
			vne::Entity_t::substrate);
	std::shared_ptr<vne::Link<int>> linkPtr = std::make_shared<vne::Link<int>>(
			vne::Entity_t::substrate, 0, 0);
	net.addLink(linkPtr);
	net.addNode(nodePtr);
	*/
}
BOOST_AUTO_TEST_SUITE_END()
	/*
	 * Network tests
	 */
BOOST_AUTO_TEST_SUITE	(GeneratorTest)
	BOOST_AUTO_TEST_CASE(Resources)
	{
		int lCount = vne::IdGenerator::peekId<t1>();
		int nCount = vne::IdGenerator::peekId<t2>();
		for (int i = 0; i < 100; i++)
		{
			vne::Link<int, double, std::string> l = vne::Link<int, double,
					std::string>(vne::Entity_t::virt, 0, 0);
			vne::Node<int, double, std::string> n = vne::Node<int, double,
					std::string>(vne::Entity_t::virt);
			BOOST_CHECK(l.getId() == lCount + i);
			BOOST_CHECK(n.getId() == nCount + i);
		}
	}
	BOOST_AUTO_TEST_SUITE_END()

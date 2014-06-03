#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CoreTest 

#include <boost/test/unit_test.hpp>
#include <tuple>
#include <string>

#include "../node.h"
#include "../link.h"
#include "../core-types.h"
#include "../id-generator.h"

/*
 * Node test suites
 */
typedef vne::Node<int, double, std::string> node_fixture;

typedef vne::Link<int, double, std::string> t1;
typedef vne::Node<int, double, std::string> t2;

BOOST_FIXTURE_TEST_SUITE (NodeTest, node_fixture)

BOOST_AUTO_TEST_CASE(Resources)
{
        std::tuple<int, double, std::string> t = std::make_tuple(22, 4.2, "test");
	setResources (t);
	BOOST_CHECK(std::get<0>(getResources()) == 22);
	BOOST_CHECK(std::get<1>(getResources()) == 4.2);
	BOOST_CHECK(std::get<2>(getResources()).compare("test")==0);
}
BOOST_AUTO_TEST_CASE(TypeId)
{
        vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(vne::Entity_t::virt);
        //BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
        //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
        BOOST_CHECK(getType () != n.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
        int nCount = vne::IdGenerator::peekId<t2>();
        for (int i=0; i<5; i++){
            vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(vne::Entity_t::virt);
            //BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
            //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
            BOOST_REQUIRE( n.getId() == nCount + i);
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
	link.setResources (t);
	BOOST_CHECK(std::get<0>(link.getResources()) == 22);
	BOOST_CHECK(std::get<1>(link.getResources()) == 4.2);
	BOOST_CHECK(std::get<2>(link.getResources()).compare("test")==0);
}
BOOST_AUTO_TEST_CASE(TypeId)
{
        vne::Link<int, double, std::string> l = vne::Link<int, double, std::string>(vne::Entity_t::virt);
        //BOOST_TEST_MESSAGE ("New node object is: " << int(l.getType()));
        //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
        BOOST_CHECK(getType () != l.getType());
}
BOOST_AUTO_TEST_CASE(Id)
{
        int lCount = vne::IdGenerator::peekId<t1>();
        for (int i=0; i<5; i++){
            vne::Link<int, double, std::string> l = vne::Link<int, double, std::string>(vne::Entity_t::virt);
            //BOOST_TEST_MESSAGE ("New node object is: " << int(n.getType()));
            //BOOST_TEST_MESSAGE ("Old node object is: " << int(getType()));
            BOOST_REQUIRE(l.getId() == lCount + i);
        }
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE (GeneratorTest)
BOOST_AUTO_TEST_CASE(Resources)
{
        int lCount = vne::IdGenerator::peekId<t1>();
        int nCount = vne::IdGenerator::peekId<t2>();
        for (int i=0; i<100; i++)
          {
            vne::Link<int, double, std::string> l = vne::Link<int, double, std::string>(vne::Entity_t::virt);
            vne::Node<int, double, std::string> n = vne::Node<int, double, std::string>(vne::Entity_t::virt);
            BOOST_CHECK (l.getId() == lCount + i);
            BOOST_CHECK (n.getId() == nCount + i);
          }
}
BOOST_AUTO_TEST_SUITE_END()

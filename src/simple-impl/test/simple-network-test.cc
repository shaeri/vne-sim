/**
 * @file simple-network-test.cc
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 13, 2014
 * 
 * @copyright Copyright (c) Jun 13, 2014                      SOROUSH HAERI
 *            All Rights Reserved 
 *
 *            Permission to use, copy, modify, and distribute this software and its
 *            documentation for any purpose and without fee is hereby granted, provided
 *            that the above copyright notice appear in all copies and that both that
 *            copyright notice and this permission notice appear in supporting
 *            documentation, and that the name of the author not be used in advertising or
 *            publicity pertaining to distribution of the software without specific,
 *            written prior permission.
 *
 *            THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *            ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *            AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *            DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *            AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *            OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <boost/test/unit_test.hpp>
#include "simple-impl/simple-link.h"
#include "simple-impl/simple-node.h"
#include "simple-impl/simple-network.h"

using namespace vne;

BOOST_AUTO_TEST_SUITE (SimpleNetworkTest)
BOOST_AUTO_TEST_CASE(IDTEST)
{
	std::shared_ptr<SimpleNode> n0 = std::make_shared<SimpleNode>(SimpleNode(Entity_t::substrate, 10));
	std::shared_ptr<SimpleNode> n1 = std::make_shared<SimpleNode>(SimpleNode(Entity_t::substrate, 10));
	std::shared_ptr<SimpleNode> n2 = std::make_shared<SimpleNode>(SimpleNode(Entity_t::substrate, 10));
	std::shared_ptr<SimpleLink> l0 = std::make_shared<SimpleLink>(SimpleLink(Entity_t::substrate, n0->getId(), n1->getId(), 7.5));
	std::shared_ptr<SimpleLink> l1 = std::make_shared<SimpleLink>(SimpleLink(Entity_t::substrate, n1->getId(), n2->getId(), 7.5));
	std::shared_ptr<SimpleLink> l2 = std::make_shared<SimpleLink>(SimpleLink(Entity_t::substrate, n1->getId(), n2->getId(), 7.5));
	SimpleNetwork net = SimpleNetwork ();
	net.addNode(n0);
	net.addNode(n1);
	net.addNode(n2);
	net.addLink(l0);
	net.addLink(l1);
	net.addLink(l2);
}

BOOST_AUTO_TEST_SUITE_END()



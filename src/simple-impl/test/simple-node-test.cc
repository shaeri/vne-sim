/**
 * @file simple-node-test.cc
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
#include "simple-impl/simple-node.h"
#include <boost/test/unit_test.hpp>


using namespace vne;

BOOST_AUTO_TEST_SUITE (SimpleNodeTest)
BOOST_AUTO_TEST_CASE(IDTEST)
{
	SimpleNode n1 = SimpleNode(12.5, Entity_t::substrate);
	BOOST_CHECK(n1.getCPU() == 12.5);
	SimpleNode n2 = SimpleNode(6.5, Entity_t::substrate);
	BOOST_CHECK(n2.getType() != Entity_t::virt);
	BOOST_CHECK(n2.getCPU() == 6.5);
	//Node<double> n3 = Node<double>(Resources<double>(11.5), Entity_t::substrate);
	//BOOST_TEST_MESSAGE("l3.id = " << n3.getId());
	//BOOST_CHECK(n3.getId() != n1.getId());
	//BOOST_CHECK(n3.getId() != n2.getId());

}
BOOST_AUTO_TEST_SUITE_END()
/* namespace vne */

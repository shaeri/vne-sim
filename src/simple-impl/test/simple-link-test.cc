/**
 * @file simple-link-test.cc
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
#include "../simple-link.h"

using namespace vne;

BOOST_AUTO_TEST_SUITE (SimpleLinkTest)
BOOST_AUTO_TEST_CASE(IDTEST)
{
	SimpleLink l1 = SimpleLink(Entity_t::substrate, 0, 1, 3.5);
	BOOST_CHECK(l1.getBandwidth() == 3.5);
	BOOST_CHECK(l1.getNodeFromId() == 0);
	SimpleLink l2 = SimpleLink(Entity_t::virt, 2, 4, 7.5);
	BOOST_CHECK(l2.getType() == Entity_t::virt);
	BOOST_CHECK(l2.getNodeToId() == 4);
	Link<double> l3 = Link<double>(Entity_t::substrate, 0, 1);
	BOOST_TEST_MESSAGE("l3.id = " << l3.getId());
	BOOST_CHECK(l3.getId() != l1.getId());
	BOOST_CHECK(l3.getId() != l2.getId());

}

BOOST_AUTO_TEST_SUITE_END()

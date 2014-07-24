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

#include <math.h>

#include "VineYard/vy-substrate-node.h"

using namespace vne::vineyard;

BOOST_AUTO_TEST_SUITE (VYTest)
BOOST_AUTO_TEST_CASE(VYCoordTest)
{
    VYCoordinate c1 = VYCoordinate (10,10);
    VYCoordinate c2 = VYCoordinate (0,0);
    BOOST_CHECK(c1.distanceFrom(c2) == sqrt (pow(c1.first-c2.first, 2)+pow(c1.second-c2.second,2)));
}
BOOST_AUTO_TEST_CASE(VYSubstrateTest)
{
    VYSubstrateNode sn1 = VYSubstrateNode(25, 10, 10);
    VYSubstrateNode sn2 = VYSubstrateNode (10,0,0);
    BOOST_CHECK(!sn1.hasResources(32));
    std::cout << sn1.getCoordinates().distanceFrom (sn2.getCoordinates()) << std::endl;
    std::cout<< ++sn1 << std::endl;
    std::cout<< sn1.getCount() << std::endl;
    std::cout<< ++sn1 << std::endl;
    std::cout<< sn1.getCount() << std::endl;
}
BOOST_AUTO_TEST_SUITE_END ()
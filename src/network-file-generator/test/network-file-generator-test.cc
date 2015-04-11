/**
 * @file   network-file-generator-test.cc
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

#include "network-file-generator/network-file-generator.h"

#include "utilities/logger.h"


using namespace vne;
using namespace vne::utilities;
using namespace vne::nfg;

BOOST_AUTO_TEST_SUITE (FileGenerator)

BOOST_AUTO_TEST_CASE(SubstrateNetworkGenerator)
{
    NetworkFileGenerator nfg;
    nfg.VYSubstrateNetFileGenerator(true);
    nfg.VYVirtualNetRequestGenerator(true);
}
BOOST_AUTO_TEST_CASE(VNRGenerator)
{
    NetworkFileGenerator nfg;
    nfg.VYVirtualNetRequestGenerator(true);
}
BOOST_AUTO_TEST_SUITE_END ()

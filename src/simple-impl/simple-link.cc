/**
 * @file simple-link.cc
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
#include "simple-link.h"

namespace vne
{

SimpleLink::SimpleLink() :
		Link<double>()
{
}

SimpleLink::SimpleLink(Entity_t t, int node_from, int node_to,
		double _bandwidth)
	: Link<double>(t, node_from, node_to)
{
	  std::get<0>(resources) = _bandwidth;
}
SimpleLink::~SimpleLink()
{
}

double SimpleLink::getBandwidth()
{
	return std::get<0>(resources);
}

} /* namespace vne */

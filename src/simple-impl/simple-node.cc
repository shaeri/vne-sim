/**
 * @file simple-node.cc
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
#include <simple-impl/simple-node.h>

namespace vne
{

SimpleNode::SimpleNode(double _cpu)
	: Node<double>()
{
	std::get<0> (resources) = _cpu;
}
SimpleNode::SimpleNode(Entity_t t, double _cpu)
	: Node<double>(t)
{
	std::get<0> (resources) = _cpu;
}
double SimpleNode::getCPU()
{
	return std::get<0> (resources);
}
SimpleNode::~SimpleNode()
{
}

} /* namespace vne */

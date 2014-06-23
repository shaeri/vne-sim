/**
 * @file virtual-node.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 17, 2014
 * 
 * @copyright Copyright (c) Jun 17, 2014                      SOROUSH HAERI
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
#ifndef VIRTUAL_NODE_H_
#define VIRTUAL_NODE_H_

#include "substrate-node.h"

namespace vne{

template<typename...> class SubstrateNode;

template<typename... NODERES>
class VirtualNode: public Node<NODERES...>
{
public:
	//VirtualNode ();
	VirtualNode (std::tuple<NODERES...> _res);
	void setHostNode (SubstrateNode<NODERES...>* _n);
	virtual ~VirtualNode();
private:
	typedef VirtualNode<NODERES...> this_t;
	//the substrate node that is hosting the virtual node
	SubstrateNode<NODERES...>* substrate_host_node;
};
/*
template<typename ... NODERES>
VirtualNode<NODERES...>::VirtualNode ()
	: Node<NODERES...> (Entity_t::virt)
{
}
*/
template<typename ... NODERES>
VirtualNode<NODERES...>::VirtualNode (std::tuple<NODERES...> _res)
	: 	Node<NODERES...> (_res, Entity_t::virt)
{
	this->id  = vne::IdGenerator::getId<this_t>(this);
}

template<typename ... NODERES>
void VirtualNode<NODERES...>::setHostNode (SubstrateNode<NODERES...>* _n)
{
	BOOST_LOG_NAMED_SCOPE("VirtualNode::setHostNode")
	substrate_host_node = _n;
	//BOOST_LOG_TRIVIAL(debug) << "substrate node pointer count: " << _n.use_count () << endl;
	//<<
}
template<typename ... NODERES>
VirtualNode<NODERES...>::~VirtualNode ()
{
	BOOST_LOG_TRIVIAL(debug) << "Destructing VirtualNode id : " << this->id <<endl;
}
}

#endif /* VIRTUAL_NODE_H_ */

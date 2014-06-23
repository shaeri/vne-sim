/**
 * @file substrate-node.h
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
#ifndef SUBSTRATE_NODE_H_
#define SUBSTRATE_NODE_H_

#include "core/node.h"
#include "core/virtual-node.h"

namespace vne
{
template<typename ... NODERES>
class SubstrateNode: public Node<NODERES...>
{
public:
	//SubstrateNode();
	SubstrateNode(std::tuple<NODERES...> _res);
	virtual ~SubstrateNode();
	bool hasResources(std::tuple<NODERES...> _res);
	Embedding_Result embedNode(std::shared_ptr<VirtualNode<NODERES...> > _n);
	void freeResources(int _id);
private:
	typedef SubstrateNode<NODERES...> this_t;
	//std::shared_ptr<SubstrateNode<NODERES...>> selfPtr;
	std::map<int, std::shared_ptr<VirtualNode<NODERES...> >> embedded_nodes;
	template<std::size_t> struct int_
	{
	};

	template<size_t Pos>
	bool hasResources(std::tuple<NODERES...> t, int_<Pos>, bool previousResult)
	{
		bool retVal;
		if (std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(t)
				> std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(
						this->resources))
		{
			retVal = previousResult && false;
		}
		else
			retVal = previousResult && true;
		return hasResources(t, int_<Pos - 1>(), retVal);
	}

	bool hasResources(std::tuple<NODERES...> t, int_<1>, bool previousResult =
			true)
	{
		bool retVal;
		if (std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1>(t)
				> std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1>(
						this->resources))
		{
			retVal = false;
		}
		else
			retVal = true;
		return retVal;
	}

	template<size_t Pos>
	void embedNode(std::tuple<NODERES...> t, int_<Pos>)
	{
		auto val =
				std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(
						this->resources);
		std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(this->resources)
				= val - std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(t);
		embedNode(t, int_<Pos - 1>());
	}

	void embedNode(std::tuple<NODERES...> t, int_<1>)
	{
		auto val = std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1> (this->resources);
		std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1> (this->resources)
				= val - std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1>(t);
	}
	template<size_t Pos>
	void freeResources(std::tuple<NODERES...> t, int_<Pos>)
	{
		auto val = std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos> (this->resources);
		std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(this->resources)
				= val + std::get<std::tuple_size<std::tuple<NODERES...>>::value - Pos>(t);
		freeResources(t, int_<Pos - 1>());
	}

	void freeResources(std::tuple<NODERES...> t, int_<1>)
	{
		auto val = std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1> (this->resources);
		std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1>(this->resources) = val
				+ std::get<std::tuple_size<std::tuple<NODERES...>>::value - 1>(t);
	}
};
/*
template<typename ... NODERES>
SubstrateNode<NODERES...>::SubstrateNode() :
		Node<NODERES...>(Entity_t::substrate)
{
}
*/
template<typename ... NODERES>
SubstrateNode<NODERES...>::SubstrateNode(std::tuple<NODERES...> _res) :
		Node<NODERES...>(_res, Entity_t::substrate, true)
{
	this->id  = vne::IdGenerator::getId<this_t>(this);
}

template<typename ... NODERES>
SubstrateNode<NODERES...>::~SubstrateNode()
{
	BOOST_LOG_TRIVIAL(debug) << "Destructing SubstrateNode id :" << this->id <<
			 endl;
	//selfPtr.reset();
}
template<typename ... NODERES>
bool SubstrateNode<NODERES...>::hasResources(std::tuple<NODERES...> _res)
{
	return hasResources(_res, int_<sizeof...(NODERES)>(), true);
}
template<typename ... NODERES>
Embedding_Result SubstrateNode<NODERES...>::embedNode(
		std::shared_ptr<VirtualNode<NODERES...> > _n)
{
	BOOST_LOG_NAMED_SCOPE("SubstrateNode::embedNode");
	auto it = embedded_nodes.find(_n->getId());
	assert(it == embedded_nodes.end());
	if(!hasResources(_n->getResources()))
	{
		return Embedding_Result::NOT_ENOUGH_SUBSTRATE_NODE_RESOURCES;
	}
	embedNode(_n->getResources(), int_<sizeof...(NODERES)>());
	_n->setHostNode(this);
	embedded_nodes[_n->getId()] = std::move(_n);
	return Embedding_Result::SUCCESSFUL_NODE_EMBEDDING;
}
template<typename ... NODERES>
void SubstrateNode<NODERES...>::freeResources(int _id)
{
	auto it = embedded_nodes.find(_id);
	assert(it != embedded_nodes.end());
	freeResources(it->second->getResources(), int_<sizeof...(NODERES)>());
	embedded_nodes[_id].reset();
	embedded_nodes.erase(it);
}
}
#endif /* SUBSTRATE_NODE_H_ */

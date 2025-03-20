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
#ifndef BOOST_LOG_DYN_LINK
#define BOOST_LOG_DYN_LINK
#endif

#include "core/node.h"
#include "core/virtual-node.h"

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

namespace vne
{
template <typename...>
class VirtualNode;

template <typename... NODERES>
class SubstrateNode : public Node<NODERES...>
{
   public:
    //SubstrateNode();
    SubstrateNode(const Resources<NODERES...> &_res);
    SubstrateNode(const NODERES &..._res);
    virtual ~SubstrateNode();
    bool hasResources(const Resources<NODERES...> &_res);
    bool hasResources(const NODERES &..._res);
    Embedding_Result embedNode(std::shared_ptr<VirtualNode<NODERES...>> _n);
    void freeResources(int _id);
    const Resources<NODERES...> &getMaxResources() const;

   private:
    typedef SubstrateNode<NODERES...> this_t;
    Resources<NODERES...> max_resources;
    std::map<int, std::shared_ptr<VirtualNode<NODERES...>>> embedded_nodes;
};
/*
template<typename ... NODERES>
SubstrateNode<NODERES...>::SubstrateNode() :
		Node<NODERES...>(Entity_t::substrate)
{
}
*/
template <typename... NODERES>
SubstrateNode<NODERES...>::SubstrateNode(const Resources<NODERES...> &_res)
    : Node<NODERES...>(_res, Entity_t::substrate, true)
{
    max_resources = _res;
    this->id = vne::IdGenerator::Instance()->getId<this_t>(this);
}
template <typename... NODERES>
SubstrateNode<NODERES...>::SubstrateNode(const NODERES &..._res)
    : Node<NODERES...>(_res..., Entity_t::substrate, true)
{
    max_resources = Resources<NODERES...>(_res...);
    this->id = vne::IdGenerator::Instance()->getId<this_t>(this);
}
template <typename... NODERES>
SubstrateNode<NODERES...>::~SubstrateNode()
{
    //BOOST_LOG_TRIVIAL(debug) << "Destructing SubstrateNode id :" << this->id <<
    //		 std::endl;
}
template <typename... NODERES>
bool SubstrateNode<NODERES...>::hasResources(const Resources<NODERES...> &_res)
{
    return this->resources.hasResources(_res);
}
template <typename... NODERES>
bool SubstrateNode<NODERES...>::hasResources(const NODERES &..._res)
{
    return this->resources.hasResources(_res...);
}
template <typename... NODERES>
Embedding_Result SubstrateNode<NODERES...>::embedNode(std::shared_ptr<VirtualNode<NODERES...>> _n)
{
    BOOST_LOG_NAMED_SCOPE("SubstrateNode::embedNode");
    auto it = embedded_nodes.find(_n->getId());
    assert(it == embedded_nodes.end());
    Embedding_Result result = this->resources.embedResources(_n->getResources());
    if (result == Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES) {
        return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
    } else {
        _n->setHostNode(this);
        embedded_nodes[_n->getId()] = std::move(_n);
        return Embedding_Result::SUCCESSFUL_EMBEDDING;
    }
}
template <typename... NODERES>
void SubstrateNode<NODERES...>::freeResources(int _id)
{
    auto it = embedded_nodes.find(_id);
    assert(it != embedded_nodes.end());
    Resources<NODERES...> _res = it->second->getResources();
    this->resources.freeResources(_res);
    embedded_nodes[_id].reset();
    embedded_nodes.erase(it);
}
template <typename... NODERES>
const Resources<NODERES...> &SubstrateNode<NODERES...>::getMaxResources() const
{
    return max_resources;
}
}  // namespace vne
#endif /* SUBSTRATE_NODE_H_ */
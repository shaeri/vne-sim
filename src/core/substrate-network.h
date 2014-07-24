/**
 * @file substrate-network.h
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
#ifndef SUBSTRATE_NETWORK_H_
#define SUBSTRATE_NETWORK_H_
#if 0
#include "core/network.h"
#include "core/substrate-network.h"

namespace vne{
template<typename ...> class SubstrateNetwork;
//template<typename ...> class Network;
template<typename ... NodeT, typename ... LinkT>
    class SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>> :
                                                            public Network<SubstrateNode<NodeT...>, Link<LinkT...>>
{
public:
    SubstrateNetwork ();
    virtual ~SubstrateNetwork();
    void addNode (std::shared_ptr<SubstrateNode<NodeT...>> node);
    std::shared_ptr<SubstrateNode<NodeT...>> getNode(int id);
	//void addLink(std::shared_ptr<SubstrateLink<LinkT...>> link);
	//std::shared_ptr<std::vector<std::shared_ptr<SubstrateLink<LinkT...>>> >getLinksForNodeId (int id);
private:
    typedef SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>> this_t;
};
template<typename ... NodeT, typename ... LinkT>
SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>>::SubstrateNetwork ()
    : Network<Node<NodeT...>, Link<LinkT...>> (true)
{
    this->id  = vne::IdGenerator::Instance()->getId<this_t>(this);
}
template<typename ... NodeT, typename ... LinkT>
SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>>::~SubstrateNetwork ()
{
}

template<typename ... NodeT, typename ... LinkT>
void SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>>::addNode (std::shared_ptr<SubstrateNode<NodeT...>> node)
{
    Network<Node<NodeT...>, Link<LinkT...>>::addNode (node);
}

template<typename ... NodeT, typename ... LinkT>
std::shared_ptr<SubstrateNode<NodeT...>> SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>>::getNode (int id)
{
    return std::static_pointer_cast<SubstrateNode<NodeT...>> (Network<Node<NodeT...>, Link<LinkT...>>::getNode (id));
}
/*
template<typename ... NodeT, typename ... LinkT>
std::shared_ptr<std::vector<std::shared_ptr<SubstrateLink<LinkT...>>>> SubstrateNetwork<SubstrateNode<NodeT...>, SubstrateLink<LinkT...>>::getLinksForNodeId (int id)
{
    return std::static_pointer_cast<std::shared_ptr<std::vector<std::shared_ptr<SubstrateLink<LinkT...>>>>> (Network<Node<NodeT...>, Link<LinkT...>>::getLinksForNodeId (id));
}
*/
}
#endif
#endif /* SUBSTRATE_NETWORK_H_ */
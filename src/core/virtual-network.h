/**
 * @file virtual-network.h
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
#ifndef VIRTUAL_NETWORK_H_
#define VIRTUAL_NETWORK_H_
#if 0
#include "core/network.h"
#include "core/virtual-network.h"

namespace vne{
    template<typename ...> class VirtualNetwork;
    //template<typename ...> class Network;
    template<typename ... NodeT, typename ... LinkT>
    class VirtualNetwork<VirtualNode<NodeT...>, VirtualLink<LinkT...>> : public Network<Node<NodeT...>, Link<LinkT...>>
    {
    public:
        VirtualNetwork ();
        virtual ~VirtualNetwork();
        void addNode (std::shared_ptr<VirtualNode<NodeT...>> node);
        std::shared_ptr<VirtualNode<NodeT...>> getNode(int id);
        //void addLink(std::shared_ptr<VirtualLink<LinkT...>> link);
        //std::shared_ptr<std::vector<std::shared_ptr<VirtualLink<LinkT...>>> >getLinksForNodeId (int id);
    private:
        typedef VirtualNetwork<VirtualNode<NodeT...>, VirtualLink<LinkT...>> this_t;
    };
    template<typename ... NodeT, typename ... LinkT>
    VirtualNetwork<VirtualNode<NodeT...>, VirtualLink<LinkT...>>::VirtualNetwork ()
    : Network<Node<NodeT...>, Link<LinkT...>> (true)
    {
        this->id  = vne::IdGenerator::Instance()->getId<this_t>(this);
    }
    template<typename ... NodeT, typename ... LinkT>
    VirtualNetwork<VirtualNode<NodeT...>, VirtualLink<LinkT...>>::~VirtualNetwork ()
    {
    }
    
    template<typename ... NodeT, typename ... LinkT>
    void VirtualNetwork<VirtualNode<NodeT...>, VirtualLink<LinkT...>>::addNode (std::shared_ptr<VirtualNode<NodeT...>> node)
    {
        Network<Node<NodeT...>, Link<LinkT...>>::addNode (node);
    }
    
    template<typename ... NodeT, typename ... LinkT>
    std::shared_ptr<VirtualNode<NodeT...>> VirtualNetwork<VirtualNode<NodeT...>, VirtualLink<LinkT...>>::getNode (int id)
    {
        return std::static_pointer_cast<VirtualNode<NodeT...>> (Network<Node<NodeT...>, Link<LinkT...>>::getNode (id));
    }
    
}
#endif
#endif /* VIRTUAL_NETWORK_H_ */

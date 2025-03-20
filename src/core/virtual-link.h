/**
   * @file virtual-link.h
   * @author Soroush Haeri <soroosh.haeri@me.com>
   * @date 7/2/14
   * 
   * @copyright Copyright (c) 7/2/14                      SOROUSH HAERI
   *     All Rights Reserved 
   *    
   *     Permission to use, copy, modify, and distribute this software and its
   *     documentation for any purpose and without fee is hereby granted, provided
   *     that the above copyright notice appear in all copies and that both that
   *     copyright notice and this permission notice appear in supporting
   *     documentation, and that the name of the author not be used in advertising or
   *     publicity pertaining to distribution of the software without specific,
   *     written prior permission.
   *
   *     THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
   *     ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
   *     AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
   *     DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
   *     AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   *     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
  **/

#ifndef VIRTUAL_LINK_H_
#define VIRTUAL_LINK_H_

#include "core/link.h"
#include "core/substrate-link.h"

namespace vne
{
template <typename...>
class SubstrateLink;

template <typename... LINKRES>
class VirtualLink : public Link<LINKRES...>
{
   public:
    //VirtualNode ();
    VirtualLink(const Resources<LINKRES...> &_res, int node_from, int node_to);
    VirtualLink(const LINKRES &..._res, int node_from, int node_to);
    virtual void addHostLink(SubstrateLink<LINKRES...> *_l);
    virtual void addHostLink(SubstrateLink<LINKRES...> *_l,
                             std::shared_ptr<Resources<LINKRES...>> _res);
    virtual void removeHostLink(SubstrateLink<LINKRES...> *_l);
    virtual ~VirtualLink();

   protected:
    std::map<int, SubstrateLink<LINKRES...> *> substrate_host_links;
    //holds the fraction of resources that are mapped on the subsrate LinkID
    //this map is used for flow splitting
    std::map<int, std::shared_ptr<Resources<LINKRES...>>> substrate_host_embedded_resources;

   private:
    typedef VirtualLink<LINKRES...> this_t;
    //the substrate links that are hosting the virtual link
    //std::vector<SubstrateLink<LINKRES...>*> ;
};
template <typename... LINKRES>
VirtualLink<LINKRES...>::VirtualLink(const Resources<LINKRES...> &_res, int node_from, int node_to)
    : Link<LINKRES...>(_res, Entity_t::virt, node_from, node_to)
{
    this->id = vne::IdGenerator::Instance()->getId<this_t>(this);
}
template <typename... LINKRES>
VirtualLink<LINKRES...>::VirtualLink(const LINKRES &..._res, int node_from, int node_to)
    : Link<LINKRES...>(_res..., Entity_t::virt, node_from, node_to)
{
    this->id = vne::IdGenerator::Instance()->getId<this_t>(this);
}
template <typename... LINKRES>
void VirtualLink<LINKRES...>::addHostLink(SubstrateLink<LINKRES...> *_l)
{
    BOOST_LOG_NAMED_SCOPE("VirtualLink::addHostLink")
    substrate_host_links.insert(std::make_pair(_l->getId(), _l));
}
template <typename... LINKRES>
void VirtualLink<LINKRES...>::addHostLink(SubstrateLink<LINKRES...> *_l,
                                          std::shared_ptr<Resources<LINKRES...>> _res)
{
    BOOST_LOG_NAMED_SCOPE("VirtualLink::addHostLink")
    substrate_host_links.insert(std::make_pair(_l->getId(), _l));
    substrate_host_embedded_resources.insert(std::make_pair(_l->getId(), _res));
}
template <typename... LINKRES>
void VirtualLink<LINKRES...>::removeHostLink(SubstrateLink<LINKRES...> *_l)
{
    BOOST_LOG_NAMED_SCOPE("VirtualLink::removeHostLink")
    auto it1 = substrate_host_links.find(_l->getId());
    substrate_host_links.erase(it1);
    auto it2 = substrate_host_embedded_resources.find(_l->getId());
    substrate_host_embedded_resources.erase(it2);
}
template <typename... LINKRES>
VirtualLink<LINKRES...>::~VirtualLink()
{
    BOOST_LOG_TRIVIAL(debug) << "Destructing VirtualLink id : " << this->id << std::endl;
}
}  // namespace vne
#endif
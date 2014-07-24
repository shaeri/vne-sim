  /**
   * @file substrate-link.h
   * @author Soroush Haeri <soroosh.haeri@me.com>
   * @date 6/23/14
   * 
   * @copyright Copyright (c) 6/23/14                      SOROUSH HAERI
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

#ifndef SUBSTRATE_LINK_H_
#define SUBSTRATE_LINK_H_

#include "core/link.h"
#include "core/virtual-link.h"

namespace vne
{
template<typename...> class VirtualLink;
    
template<typename... LINKRES>
class SubstrateLink: public Link<LINKRES...>
{
public:
    SubstrateLink(const Resources<LINKRES...>& _res, int node_from, int node_to);
    SubstrateLink(const LINKRES &... _res, int node_from, int node_to);
    virtual ~SubstrateLink();
	bool hasResources(const Resources<LINKRES...>& _res);
	Embedding_Result embedLink(std::shared_ptr<VirtualLink<LINKRES...> > _l);
	void freeResources(int _id);
protected:
    std::map<int, std::shared_ptr<VirtualLink<LINKRES...>>> embedded_links;
private:
    typedef SubstrateLink<LINKRES...> this_t;
};
template<typename... LINKRES>
SubstrateLink<LINKRES...>::SubstrateLink(const Resources<LINKRES...>& _res, int node_from, int node_to)
    : Link<LINKRES...>(_res, Entity_t::substrate, node_from, node_to, true)
{
    this -> id = IdGenerator::Instance()->getId<this_t>(this);
}
template<typename... LINKRES>
SubstrateLink<LINKRES...>::SubstrateLink(const LINKRES &... _res, int node_from, int node_to)
    : Link<LINKRES...>(_res..., Entity_t::substrate, node_from, node_to, true)
{
    this -> id = IdGenerator::Instance()->getId<this_t>(this);
}
template<typename... LINKRES>
SubstrateLink<LINKRES...>::~SubstrateLink()
{
}
template<typename... LINKRES>
bool SubstrateLink<LINKRES...>::hasResources(const Resources<LINKRES...> &_res)
{
   return this->resources.hasResources(_res);
}
template<typename... LINKRES>
Embedding_Result SubstrateLink<LINKRES...>::embedLink (std::shared_ptr<VirtualLink<LINKRES...> > _l)
{
    BOOST_LOG_NAMED_SCOPE("SubstrateLink::embedLink");
	auto it = embedded_links.find(_l->getId());
	assert(it == embedded_links.end());
    Resources<LINKRES...> _res = _l->getResources ();
    Embedding_Result result = this->resources.embedResources(_res);
	if(result == Embedding_Result::NOT_ENOUGH_SUBSTRATE_NODE_RESOURCES)
	{
		return Embedding_Result::NOT_ENOUGH_SUBSTRATE_NODE_RESOURCES;
	}
	else
    {
        _l-> addHostLink (this);
        embedded_links[_l->getId()] = std::move(_l);
        return Embedding_Result::SUCCESSFUL_NODE_EMBEDDING;
    }
}
template<typename... LINKRES>
void SubstrateLink<LINKRES...>::freeResources(int _id)
{
    auto it = embedded_links.find(_id);
	assert(it != embedded_links.end());
    Resources<LINKRES...> _res = it->second->getResources();
	this->resources.freeResources(_res);
	embedded_links[_id].reset();
	embedded_links.erase(it);
}
}
#endif
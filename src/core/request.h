/**
 * @file request.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 12, 2014
 * 
 * @copyright Copyright (c) Jun 12, 2014                      SOROUSH HAERI
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
#ifndef REQUEST_H_
#define REQUEST_H_

#include "id-generator.h"
#include "node.h"
#include "link.h"

#include <tuple>

namespace vne
{
template<typename ...> class Request;


template<typename ... NODERES, typename ... LINKRES>
class Request<Node<NODERES...>, Link<LINKRES...>>
{
public:
	Request();
	int getId ();
	std::tuple<NODERES...> getNodeResources(int node_id);
	void setNodeResources (int node_id, std::tuple<NODERES...> res);
	std::tuple<LINKRES...> getLinkResources(int link_id);
	void setLinkResources (int link_id, std::tuple<LINKRES...> res);

private:
	typedef Request<Node<NODERES...>, Link<LINKRES...>> this_t;
protected:
	int id;
	//For every link and node holds a set of resources requests
	std::map<int, std::tuple<LINKRES...>> linkResources;
	std::map<int, std::tuple<NODERES...>> nodeResources;
};
template<typename ... NODERES, typename ... LINKRES>
Request<Node<NODERES...>, Link<LINKRES...>>::Request ()
	: id (IdGenerator::getId<this_t>(this))
{
}

template<typename ... NODERES, typename ... LINKRES>
int Request<Node<NODERES...>, Link<LINKRES...>>::getId()
{
	return id;
}
template<typename ... NODERES, typename ... LINKRES>
std::tuple<NODERES...> Request<Node<NODERES...>, Link<LINKRES...>>::getNodeResources(int node_id)
{
	return nodeResources[node_id];
}
template<typename ... NODERES, typename ... LINKRES>
std::tuple<LINKRES...> Request<Node<NODERES...>, Link<LINKRES...>>::getLinkResources(int link_id)
{
	return linkResources[link_id];
}

template<typename ... NODERES, typename ... LINKRES>
void Request<Node<NODERES...>, Link<LINKRES...>>::setNodeResources
		(int node_id, std::tuple<NODERES...> res)
{
	nodeResources[node_id] = res;
}
template<typename ... NODERES, typename ... LINKRES>
void Request<Node<NODERES...>, Link<LINKRES...>>::setLinkResources
		(int link_id, std::tuple<LINKRES...> res)
{
	linkResources[link_id] = res;
}
}
#endif /* REQUEST_H_ */
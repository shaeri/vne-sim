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
#include <tuple>

namespace vne
{
template<typename ...> class Request;


template<typename ... NODERES, typename ... LINKRES>
class Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>
{
public:
	Request(std::tuple<NODERES...> noderes, std::tuple<LINKRES...> linkres);
	int getId ();
	std::tuple<NODERES...> getNodeResources();
	std::tuple<LINKRES...> getLinkResources();
	//void setNodeResources(std::tuple<NODERES...> t);
	//void setLinkResources(std::tuple<LINKRES...> t);
private:
	typedef Request<std::tuple<NODERES...>, std::tuple<LINKRES...>> this_t;
	int id;
	std::tuple<LINKRES...> nodeResources;
	std::tuple<NODERES...> linkResources;
};
template<typename ... NODERES, typename ... LINKRES>
Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>::Request
		(std::tuple<NODERES...> nodeRes, std::tuple<LINKRES...> linkRes)
				: id (vne::IdGenerator::getId<this_t>(this)),
				  nodeResources (nodeRes),
				  linkResources (linkRes)
{
}

template<typename ... NODERES, typename ... LINKRES>
int Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>::getId()
{
	return id;
}
template<typename ... NODERES, typename ... LINKRES>
std::tuple<NODERES...> Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>::getNodeResources()
{
	return nodeResources;
}
template<typename ... NODERES, typename ... LINKRES>
std::tuple<LINKRES...> Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>::getLinkResources()
{
	return linkResources;
}
/*
template<typename ... NODERES, typename ... LINKRES>
void Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>::setNodeResources
		(std::tuple<NODERES...> t)
{
	nodeResources = t;
}
template<typename ... NODERES, typename ... LINKRES>
void Request<std::tuple<NODERES...>, std::tuple<LINKRES...>>::setLinkResources
		(std::tuple<LINKRES...> t)
{
	linkResources = t;
}
*/
}
#endif /* REQUEST_H_ */

/**
 * @file network.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 4, 2014
 * 
 * @copyright Copyright (c) Jun 4, 2014                      SOROUSH HAERI
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
#ifndef NETWORK_H_
#define NETWORK_H_

#include "node.h"
#include "link.h"
#include <memory>

namespace vne
{
template <typename...> class Network;
template<template<typename...> class Node, typename... NodeT,
			template<typename...> class Link, typename... LinkT>
class Network<Node<NodeT...>, Link<LinkT...>>
{
public:
	Network ();
	~Network();
	void addNode (std::shared_ptr<Node<NodeT...>> node) ;
	//void addLink (std::shared_ptr<Link<LinkT...>> link) ;
private:
	std::map<int, std::shared_ptr<Node<NodeT...>>> nodesMap;
	//std::map<int, std::vector<std::shared_ptr<Link<LinkT...>>>> linksMap;
};
template<template<typename...> class Node, typename... NodeT,
			template<typename...> class Link, typename... LinkT>
Network<Node<NodeT...>, Link<LinkT...>>::Network ()
{
}
/*
template<vne::Node, typename... args1, vne::Link, typename... args2>
Network<NodeT, LinkT>::~Network ()
{
}
template<vne::Node, typename... args1, vne::Link, typename... args2>
void Network<NodeT, LinkT>::addNode(std::shared_ptr<NodeT> node)
{
}
template<vne::Node, typename... args1, vne::Link, typename... args2>
void Network<NodeT, LinkT>::addLink(std::shared_ptr<LinkT> link)
{
}
*/
}
#endif /* NETWORK_H_ */

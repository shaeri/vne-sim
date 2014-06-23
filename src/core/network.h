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
#define BOOST_LOG_DYN_LINK

#include "node.h"
#include "link.h"
#include <vector>
#include <memory>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

namespace vne
{
template<typename ...> class Network;
template<typename ... NodeT, typename ... LinkT>
class Network<Node<NodeT...>, Link<LinkT...>>
{
public:
	Network();
	~Network();
	void addNode(std::shared_ptr<Node<NodeT...>> node);
	std::shared_ptr<Node<NodeT...>> getNode(int id);
	void addLink(std::shared_ptr<Link<LinkT...>> link);
	std::shared_ptr<std::vector<std::shared_ptr<Link<LinkT...>>> >getLinksForNodeId (int id);
private:
	typedef Network<Node<NodeT...>, Link<LinkT...>> this_t;
protected:
	int id;
	std::unordered_map<int, std::shared_ptr<Node<NodeT...>>>nodesMap;
	std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<Link<LinkT...>>>>> linksMap;
};
template<typename ... NodeT, typename ... LinkT>
Network<Node<NodeT...>, Link<LinkT...>>::Network()
	: id(IdGenerator::getId<this_t>(this))
{
}
template<typename ... NodeT, typename ... LinkT>
Network<Node<NodeT...>, Link<LinkT...>>::~Network()
{
}
template<typename ... NodeT, typename ... LinkT>
void Network<Node<NodeT...>, Link<LinkT...>>::addNode(
		std::shared_ptr<Node<NodeT...>> node)
{
	auto it = nodesMap.find(node->getId());
	assert(
			it == nodesMap.end()
					&& "A node with the same ID exists in the network.");
	nodesMap[node->getId()] = node;
}
template<typename ... NodeT, typename ... LinkT>
std::shared_ptr<Node<NodeT...>>
Network<Node<NodeT...>, Link<LinkT...>>::getNode(int id)
{
	if (nodesMap.find(id) == nodesMap.end())
		return 0;
	else
		return nodesMap[id];
}

template<typename ... NodeT, typename ... LinkT>
void Network<Node<NodeT...>, Link<LinkT...>>::addLink(
		std::shared_ptr<Link<LinkT...>> link)
{
	assert(
			nodesMap.find(link->getNodeFromId()) != nodesMap.end()
					&& "Trying to add a link with invalid nodefrom ID");
	assert(
			nodesMap.find(link->getNodeToId()) != nodesMap.end()
					&& "Trying to add a link with invalid nodeTo ID");
	//add an element for node From
	BOOST_LOG_TRIVIAL(debug)<< "Link ID: " << link->getId() << " from node: "
			<< link->getNodeFromId() << " to: " << link->getNodeToId();
	auto it = linksMap.find(link->getNodeFromId());
	if (it == linksMap.end())
	{
		std::vector<std::shared_ptr<Link<LinkT...>>>v;
		v.push_back(link);
		linksMap.insert(std::make_pair(link->getNodeFromId (),
						std::make_shared<std::vector<std::shared_ptr<Link<LinkT...>>>>(v)));
	}
	else
	linksMap[link->getNodeFromId ()]->push_back(link);
	//add an element for node to
	it = linksMap.find(link->getNodeToId());
	if (it == linksMap.end())
	{
		std::vector<std::shared_ptr<Link<LinkT...>>>m;
		m.push_back(link);
		linksMap.insert(std::make_pair(link->getNodeToId (),
						std::make_shared<std::vector<std::shared_ptr<Link<LinkT...>>>>(m)));
	}
	else
	linksMap[link->getNodeToId ()]->push_back(link);
}
template<typename ... NodeT, typename ... LinkT>
std::shared_ptr<std::vector<std::shared_ptr<Link<LinkT...>>> >
	Network<Node<NodeT...>, Link<LinkT...>>::getLinksForNodeId(int id)
{
	if (linksMap.find(id) == linksMap.end())
	return 0;
	else
	return linksMap[id];
}
}
#endif /* NETWORK_H_ */

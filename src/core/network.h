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
//class NetworkBase {};
template<typename, typename> class Network;
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
class Network<NodeC<NodeT...>, LinkC<LinkT...>>
{
static_assert (std::is_base_of<Node<NodeT...>, NodeC<NodeT...>>::value,
               "First template argument of class Network must be a Node derivative.");
static_assert (std::is_base_of<Link<LinkT...>, LinkC<LinkT...>>::value,
               "Second template argument of class Network must be a Link derivative.");
public:
	Network();
	virtual ~Network();
	void addNode(std::shared_ptr<NodeC<NodeT...>> node);
	const std::shared_ptr<NodeC<NodeT...>> getNode(int id);
	void addLink(std::shared_ptr<LinkC<LinkT...>> link);
	const std::shared_ptr<const std::vector<std::shared_ptr<LinkC<LinkT...>>> >getLinksForNodeId (int id) const;
    const std::shared_ptr<std::vector<std::shared_ptr<const NodeC<NodeT...>>>>getAllNodes () const;
    const std::shared_ptr<std::vector<std::shared_ptr<const LinkC<LinkT...>>>>getAllLinks () const;
    int getId() {return this->id;};
    int getNumNodes () {return this->nodesMap.size();};
    int getNumLinks () {return this->linksMap.size();};
private:
	typedef Network<NodeC<NodeT...>, LinkC<LinkT...>> this_t;
protected:
	int id;
    Network (bool noid);
	std::map<int, std::shared_ptr<NodeC<NodeT...>>> nodesMap;
    std::map<int, std::shared_ptr<LinkC<LinkT...>>> linksMap;
    //key stores the nodeId and the vector stores the set of Id of all links attached to the given nodeId
    std::map<int, std::shared_ptr<std::vector<int>>> connectionMap;
    
};
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::Network()
	: id(IdGenerator::Instance()->getId<this_t>(this))
{
}
//this is the protected constructor that does not assign an id to the object
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::Network(bool noid)
{
}

template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::~Network()
{
}

template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
void Network<NodeC<NodeT...>, LinkC<LinkT...>>::addNode(
		std::shared_ptr<NodeC<NodeT...>> node)
{
	auto it = nodesMap.find(node->getId());
	assert(
			it == nodesMap.end()
					&& "A node with the same ID exists in the network.");
	nodesMap[node->getId()] = node;
}

template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
const std::shared_ptr<NodeC<NodeT...>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getNode(int id)
{
	if (nodesMap.find(id) == nodesMap.end())
		return 0;
	else
		return nodesMap[id];
}

template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
void Network<NodeC<NodeT...>, LinkC<LinkT...>>::addLink(
		std::shared_ptr<LinkC<LinkT...>> link)
{
	assert(
			nodesMap.find(link->getNodeFromId()) != nodesMap.end()
					&& "Trying to add a link with invalid nodefrom ID");
	assert(
			nodesMap.find(link->getNodeToId()) != nodesMap.end()
					&& "Trying to add a link with invalid nodeTo ID");
    assert(
           linksMap.find(link->getId()) == linksMap.end()
           && "link id already exists in the network.");
	//add an element for node From
	BOOST_LOG_TRIVIAL(debug)<< "Link ID: " << link->getId() << " from node: "
			<< link->getNodeFromId() << " to: " << link->getNodeToId();
	auto it = connectionMap.find(link->getNodeFromId());
	if (it == connectionMap.end())
	{
		std::vector<int> v;
		v.push_back(link->getId());
		connectionMap.insert(std::make_pair(link->getNodeFromId (),
						std::make_shared<std::vector<int>>(v)));
	}
	else
    {
        connectionMap[link->getNodeFromId ()]->push_back(link->getId());
    }
    
	//add an element for nodeTo
	it = connectionMap.find(link->getNodeToId());
	if (it == connectionMap.end())
	{
		std::vector<int> v;
		v.push_back(link->getId());
        connectionMap.insert(std::make_pair(link->getNodeToId (),
						std::make_shared<std::vector<int>>(v)));
	}
	else
       connectionMap[link->getNodeToId ()]->push_back(link->getId());
    //add the link to the linkMap
    linksMap[link->getId()]=link;
}
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
const std::shared_ptr<const std::vector<std::shared_ptr<LinkC<LinkT...> >> >
	Network<NodeC<NodeT...>, LinkC<LinkT...>>::getLinksForNodeId(int id) const
{
    const std::shared_ptr<std::vector<std::shared_ptr<LinkC<LinkT...> >> > v (new std::vector<std::shared_ptr<LinkC<LinkT...> >>());
    auto it = connectionMap.find(id);
    if (it != connectionMap.end())
    {
        for (auto linkId : *(it->second))
        {
            v->push_back (linksMap.find(linkId)->second);
        }
        return v;
    }
    else
        return nullptr;
}
template<typename... NodeT, template<typename...> class NodeC,
typename... LinkT, template <typename...> class LinkC>
const std::shared_ptr<std::vector<std::shared_ptr<const NodeC<NodeT...>>>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getAllNodes () const
{
    const std::shared_ptr<std::vector<std::shared_ptr<const NodeC<NodeT...>>>> vec (new std::vector<std::shared_ptr<const NodeC<NodeT...>>>());
    for (auto it = nodesMap.begin(); it != nodesMap.end(); ++it)
        vec->push_back(it->second);
    return vec;
}
template<typename... NodeT, template<typename...> class NodeC,
typename... LinkT, template <typename...> class LinkC>
const std::shared_ptr<std::vector<std::shared_ptr<const LinkC<LinkT...>>>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getAllLinks () const
{
    const std::shared_ptr<std::vector<std::shared_ptr<const LinkC<LinkT...>>>> vec (new std::vector<std::shared_ptr<const LinkC<LinkT...>>>());
    for (auto it = linksMap.begin(); it != linksMap.end(); ++it)
        vec->push_back(it->second);
    return vec;
}
}
#endif /* NETWORK_H_ */
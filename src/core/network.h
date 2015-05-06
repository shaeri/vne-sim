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
#include <set>
#include <memory>
#include <fstream>

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
	void addLink(std::shared_ptr<LinkC<LinkT...>> link);
    
    const std::shared_ptr<NodeC<NodeT...>> getNode(int id) const;
    const std::shared_ptr<std::set<int>> getNodeIdSet () const;
	
    const std::shared_ptr<LinkC<LinkT...>> getLink (int id) const;
    const std::shared_ptr<const std::vector<std::shared_ptr<LinkC<LinkT...>>> > getLinksForNodeId (int id) const;
    const std::shared_ptr<LinkC<LinkT...>> getLinkBetweenNodes (int nodeIdFrom, int nodeIdTo) const;
    const std::shared_ptr<std::set<int>> getLinkIdSet () const;
    
    const std::shared_ptr<std::vector<std::shared_ptr<NodeC<NodeT...>>>> getAllNodes () const;
    const std::shared_ptr<std::vector<std::shared_ptr<LinkC<LinkT...>>>> getAllLinks () const;
    
    void writeNetworkToFile (std::ofstream& ofstrm, bool writeNetworkSize);
    
    template<typename CONDITION, typename ... T>
    const std::shared_ptr<std::vector<std::shared_ptr<NodeC<NodeT...>>>> getNodesWithConditions (T &... args);
    //template<typename CONDITION, typename ... T>
    //inline const std::shared_ptr<std::vector<int>> getNodesIDsWithConditions (T &... args);
    template<typename CONDITION, typename ... T>
    const std::shared_ptr<std::set<int>> getNodesIDsWithConditions (T &... args);
    //runs the BFS algorithm from a given nodeId
    //This functinos stops visiting nodes when the condition is not met.
    //template<typename CONDITION, typename ... T>
    //inline const std::shared_ptr<std::vector<int>> BFSwithCondition (int nodeId, T &... args);
    
    int getId() {return this->id;};
    int getNumNodes () {return (int) this->nodesMap.size();};
    int getNumLinks () {return (int) this->linksMap.size();};
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
    node->local_id  = (int) nodesMap.size()-1;
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
    
    //set the link's nodes to and from local ids
    link->node_from_local_id = nodesMap[link->getNodeFromId()]->getLocalId ();
    link->node_to_local_id = nodesMap[link->getNodeToId()]->getLocalId ();
	
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
inline const std::shared_ptr<NodeC<NodeT...>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getNode(int id) const
{
    auto it = nodesMap.find(id);
    if (it == nodesMap.end())
        return 0;
    return it->second;
}
    
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
inline const std::shared_ptr<std::set<int>>
    Network<NodeC<NodeT...>, LinkC<LinkT...>>::getNodeIdSet() const
{
    const std::shared_ptr<std::set<int>> keySet (new std::set<int>());
    for (auto it = nodesMap.begin(); it!=  nodesMap.end(); it++)
    {
        keySet->insert(it->first);
    }
    return keySet;
}
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
inline const std::shared_ptr<const std::vector<std::shared_ptr<LinkC<LinkT...> >> >
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
inline const std::shared_ptr<LinkC<LinkT...>>
    Network<NodeC<NodeT...>, LinkC<LinkT...>>::getLinkBetweenNodes (int nodeIdFrom, int nodeIdTo) const
{
    auto nodeFromItr = connectionMap.find (nodeIdFrom);
    auto nodeToItr = connectionMap.find (nodeIdTo);
    if (connectionMap.size() > 0 && (nodeFromItr == connectionMap.end() || nodeToItr == connectionMap.end ()))
        std::cerr << "Network::getLinkBetweenNodes : Invalid nodeIds" << std::endl;
    // The connection map is symmetric, however, the links are not.
    // Therefore, we need to do the loop twice once for nodeIdFrom
    // and once for nodeIdTo.
    for (int i : *(nodeFromItr->second))
    {
        auto it = linksMap.find (i);
        if ((it->second->getNodeFromId () == nodeIdFrom && it->second->getNodeToId () == nodeIdTo) ||
            (it->second->getNodeFromId () == nodeIdTo && it->second->getNodeToId () == nodeIdFrom))
            return it->second;
    }
    return nullptr;
}
    
template<typename... NodeT, template<typename...> class NodeC,
typename... LinkT, template <typename...> class LinkC>
const std::shared_ptr<LinkC<LinkT...>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getLink(int id) const
{
    auto it = linksMap.find(id);
    if (it == linksMap.end())
        return 0;
    return it->second;
}
    
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
inline const std::shared_ptr<std::set<int>>
    Network<NodeC<NodeT...>, LinkC<LinkT...>>::getLinkIdSet () const
{
    const std::shared_ptr<std::set<int>> keySet (new std::set<int>());
    for (auto it = linksMap.begin(); it!=  linksMap.end(); it++)
    {
        keySet->insert(it->first);
    }
    return keySet;
}

template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
inline const std::shared_ptr<std::vector<std::shared_ptr<NodeC<NodeT...>>>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getAllNodes () const
{
    const std::shared_ptr<std::vector<std::shared_ptr<NodeC<NodeT...>>>> vec (new std::vector<std::shared_ptr<NodeC<NodeT...>>>());
    for (auto it = nodesMap.begin(); it != nodesMap.end(); ++it)
        vec->push_back(it->second);
    return vec;
}
    
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
inline const std::shared_ptr<std::vector<std::shared_ptr<LinkC<LinkT...>>>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getAllLinks () const
{
    const std::shared_ptr<std::vector<std::shared_ptr<LinkC<LinkT...>>>> vec (new std::vector<std::shared_ptr<LinkC<LinkT...>>>());
    for (auto it = linksMap.begin(); it != linksMap.end(); ++it)
        vec->push_back(it->second);
    return vec;
}
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
inline void Network<NodeC<NodeT...>, LinkC<LinkT...>>::writeNetworkToFile (std::ofstream& ofstrm, bool writeNetworkSize)
{
    if (ofstrm.is_open())
    {
        if (writeNetworkSize)
            ofstrm << getNumNodes() << " " << getNumLinks() << std::endl;
    
        for (auto it = nodesMap.begin(); it != nodesMap.end(); ++it)
            it->second-> writeNodeToFile (ofstrm);
        for (auto it = linksMap.begin(); it != linksMap.end(); ++it)
            it->second-> writeLinkToFile (ofstrm);
    }
    else
        BOOST_LOG_TRIVIAL(error) << "Network::writeVNRToFile: VYThe file is not open for writing. " << std::endl;
}
    
template<typename... NodeT, template<typename...> class NodeC,
    typename... LinkT, template <typename...> class LinkC>
        template<typename CONDITION, typename... T>
inline const std::shared_ptr<std::vector<std::shared_ptr<NodeC<NodeT...>>>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getNodesWithConditions (T &... args)
{
    const std::shared_ptr<std::vector<std::shared_ptr<const NodeC<NodeT...>>>> vec (new std::vector<std::shared_ptr<const NodeC<NodeT...>>>());
    CONDITION cond;
    for (auto it = nodesMap.begin(); it != nodesMap.end(); ++it)
    {
        if (cond(*(it->second), getLinksForNodeId(it->first),args...))
            vec->push_back(it->second);
    }
    return vec;
}
    
/*template<typename... NodeT, template<typename...> class NodeC,
typename... LinkT, template <typename...> class LinkC>
template<typename CONDITION, typename... T>
inline const std::shared_ptr<std::vector<int>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getNodesIDsWithConditions (T &... args)
{
    const std::shared_ptr<std::vector<int>> vec (new std::vector<int>());
    CONDITION cond;
    for (auto it = nodesMap.begin(); it != nodesMap.end(); ++it)
    {
        if (cond(*(it->second), args...))
            vec->push_back(it->second->getId());
    }
    return vec;
}
*/
template<typename... NodeT, template<typename...> class NodeC,
typename... LinkT, template <typename...> class LinkC>
template<typename CONDITION, typename... T>
inline const std::shared_ptr<std::set<int>>
Network<NodeC<NodeT...>, LinkC<LinkT...>>::getNodesIDsWithConditions (T &... args)
{
    const std::shared_ptr<std::set<int>> idSet (new std::set<int>());
    CONDITION cond;
    for (auto it = nodesMap.begin(); it != nodesMap.end(); ++it)
    {
        if (cond(it->second, getLinksForNodeId(it->first), args...))
            idSet->insert(it->second->getId());
    }
    return idSet;
}
}
#endif /* NETWORK_H_ */
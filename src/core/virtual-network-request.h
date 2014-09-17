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
#ifndef VIRTUAL_NETWORK_REQUEST_H_
#define VIRTUAL_NETWORK_REQUEST_H_

#include "id-generator.h"
#include "network.h"
#include "virtual-node.h"
#include "virtual-link.h"

#include <list>

namespace vne
{
template<typename> class VirtualNetworkRequest;

template<typename ... NODERES, template <typename ...> class NODECLASS,
            typename... LINKRES, template <typename...> class LINKCLASS>
class VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>
{
static_assert (std::is_base_of<VirtualNode<NODERES...>, NODECLASS<NODERES...>>::value,
                   "First template argument must be a VirtualNode derivative.");
static_assert (std::is_base_of<VirtualLink<LINKRES...>, LINKCLASS<LINKRES...>>::value,
                   "Second template argument must be a VirtualLink derivative.");
//template<typename, typename> friend class NodeEmbeddingAlgorithm;
//template<typename, typename> friend class LinkEmbeddingAlgorithm;
    
public:
	VirtualNetworkRequest(std::shared_ptr<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> _vn,
                          double time, double duration);
	int getId () const {return id;};
    double getDuration () const {return duration;};
    double getArrivalTime() const {return arrivalTime;};
    // Proc time is usually not known in advance so after the embedding
    // algorithm is completed this value is set for simulation purposes
    double getProccessingTime() const {return proc_time;};
    void setProccessingTime (double t) {proc_time = t;}
    double getDepartureTime() const {return arrivalTime + duration;};
    //bool successfulEmbedding () const {return successful_embedding;};
    //void setEmbeddingResult (bool arg) {successful_embedding = arg;};
    bool operator< (VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>& rhs) const {return this->time<rhs.getTime();};
    std::shared_ptr<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> getVN () const {return vn;};
    
    const std::map<int,std::list<int>>* getLinkMap () const {return &linkMap;};
    const std::map<int,int>* getNodeMap () const {return &nodeMap;};
    void addNodeMapping (int sNodeId, int vNodeId);
    void addLinkMapping (int sLinkId, int vLinkId);
    
protected:
	int id;
    double arrivalTime;
    double duration;
    double proc_time;
    //bool successful_embedding;
    std::shared_ptr<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> vn;
    VirtualNetworkRequest(std::shared_ptr<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> _vn,
                          double time, double duration, bool noid);
    ///These two maps hold the temporary mappings based on node and link IDs
    //nodeMap<VirtualnodeId, SubstrateNodeId>;
    std::map<int, int> nodeMap;
    //linkMap<virtualLinkId, list<substrateNodeIds>>
    std::map<int,std::list<int>> linkMap;
    
private:
	typedef VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> this_t;
    
};
template<typename ... NODERES, template <typename ...> class NODECLASS,
    typename... LINKRES, template <typename...> class LINKCLASS>
    VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>::
    VirtualNetworkRequest(std::shared_ptr<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> _vn,
                          double _time, double _duration)
    : id (vne::IdGenerator::Instance()->getId<this_t>(this)),
      arrivalTime (_time),
      duration(_duration),
      proc_time(0),
      vn(std::move(_vn)),
      //successful_embedding(false),
      linkMap (std::map<int,std::list<int>>()),
      nodeMap (std::map<int,int>())
    {}
template<typename ... NODERES, template <typename ...> class NODECLASS,
    typename... LINKRES, template <typename...> class LINKCLASS>
    void VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>::
            addNodeMapping (int sNodeId, int vNodeId)
    {
       nodeMap.insert(std::make_pair(vNodeId, sNodeId));
    }

template<typename ... NODERES, template <typename ...> class NODECLASS,
    typename... LINKRES, template <typename...> class LINKCLASS>
    void VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>::
            addLinkMapping (int sLinkId, int vLinkId)
    {
        linkMap[vLinkId].push_back(sLinkId);
    }
}

#endif /* REQUEST_H_ */
/**
 * @file link-embedding-algorithm.h
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

#ifndef LINK_EMBEDDING_ALGORITHM_H_
#define LINK_EMBEDDING_ALGORITHM_H_

#include <list>

#include "network.h"
#include "substrate-link.h"
#include "substrate-node.h"
#include "virtual-network-request.h"
#include "network-builder.h"

namespace vne
{
template <typename, typename>
class LinkEmbeddingAlgorithm;

template <typename... SNODERES, template <typename...> class SNODECLASS, typename... SLINKRES,
          template <typename...> class SLINKCLASS, typename... VNODERES,
          template <typename...> class VNODECLASS, typename... VLINKRES,
          template <typename...> class VLINKCLASS, template <typename> class VNRCLASS>
class LinkEmbeddingAlgorithm<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
                             VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
{
    static_assert(std::is_base_of<SubstrateNode<SNODERES...>, SNODECLASS<SNODERES...>>::value,
                  "Template arguments are not correctly set.");
    static_assert(std::is_base_of<VirtualNode<VNODERES...>, VNODECLASS<VNODERES...>>::value,
                  "Template arguments are not correctly set.");
    static_assert(std::is_base_of<SubstrateLink<SLINKRES...>, SLINKCLASS<SLINKRES...>>::value,
                  "Template arguments are not correctly set.");
    static_assert(std::is_base_of<VirtualLink<VLINKRES...>, VLINKCLASS<VLINKRES...>>::value,
                  "Template arguments are not correctly set.");
    static_assert(
        std::is_base_of<
            VirtualNetworkRequest<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>,
            VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>::value,
        "Template arguments are not correctly set.");
    //TwoStageEmbeddingAlgo is a friend
    template <typename SUBNET, typename VNR>
    friend class TwoStageEmbeddingAlgo;

   public:
    typedef VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>> VNR_TYPE;
    typedef Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>> SUBSTRATE_TYPE;

    virtual Embedding_Result embeddVNRLinks(std::shared_ptr<SUBSTRATE_TYPE> substrate_net,
                                            std::shared_ptr<VNR_TYPE> vnr) = 0;
    //implementing this funciton is optional. However, for using MCTS this function must be implemented.
    //This function works on the given id sets insetad of the vnr default id sets.
    virtual Embedding_Result embeddVNRLinksForIdSets(
        std::shared_ptr<SUBSTRATE_TYPE> substrate_net, std::shared_ptr<VNR_TYPE> vnr,
        const std::map<int, int> *nodeIdMap,
        std::map<int, std::list<std::pair<int, std::shared_ptr<Resources<SLINKRES...>>>>> *linkMap)
    {
        return embeddVNRLinks(substrate_net, vnr);
    };

    Link_Embedding_Algo_Types getType() const { return type; };

   protected:
    LinkEmbeddingAlgorithm(Link_Embedding_Algo_Types t) : type(t) {};
    Link_Embedding_Algo_Types type;
};
}  // namespace vne

#endif

/**
 * @file release-algorithm.h
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

#ifndef RELEASE_ALGORITHM_H_
#define RELEASE_ALGORITHM_H_

#include "core/network-builder.h"

namespace vne {
    template<typename,typename> class ReleaseAlgorithm;
    
    template<
    typename ... SNODERES, template <typename ...> class SNODECLASS,
    typename... SLINKRES, template <typename...> class SLINKCLASS,
    typename ... VNODERES, template <typename ...> class VNODECLASS,
    typename... VLINKRES, template <typename...> class VLINKCLASS,
    template<typename> class VNRCLASS>
    class ReleaseAlgorithm<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
    VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>
    {
        static_assert (std::is_base_of<SubstrateNode<SNODERES...>, SNODECLASS<SNODERES...>>::value,
                       "Template arguments are not correctly set.");
        static_assert (std::is_base_of<VirtualNode<VNODERES...>, VNODECLASS<VNODERES...>>::value,
                       "Template arguments are not correctly set.");
        static_assert (std::is_base_of<SubstrateLink<SLINKRES...>, SLINKCLASS<SLINKRES...>>::value,
                       "Template arguments are not correctly set.");
        static_assert (std::is_base_of<VirtualLink<VLINKRES...>, VLINKCLASS<VLINKRES...>>::value,
                       "Template arguments are not correctly set.");
        static_assert (std::is_base_of<VirtualNetworkRequest<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>,
                       VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>::value, "Template arguments are not correctly set.");
        
    public:
        typedef VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>> VNR_TYPE;
        typedef Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>> SUBSTRATE_TYPE;
        
        ReleaseAlgorithm (NetworkBuilder<SUBSTRATE_TYPE>& _sb) : substrate_network(_sb.getNetwork()) {};
        ReleaseAlgorithm (std::shared_ptr<SUBSTRATE_TYPE> _sn) : substrate_network(_sn) {};
        
        virtual void releaseVNR (std::shared_ptr<VNR_TYPE> vnr);
        
    protected:
        std::shared_ptr<SUBSTRATE_TYPE> substrate_network;
    };
    
    template<
    typename ... SNODERES, template <typename ...> class SNODECLASS,
    typename... SLINKRES, template <typename...> class SLINKCLASS,
    typename ... VNODERES, template <typename ...> class VNODECLASS,
    typename... VLINKRES, template <typename...> class VLINKCLASS,
    template<typename> class VNRCLASS>
    void ReleaseAlgorithm<Network<SNODECLASS<SNODERES...>, SLINKCLASS<SLINKRES...>>,
    VNRCLASS<Network<VNODECLASS<VNODERES...>, VLINKCLASS<VLINKRES...>>>>::releaseVNR (std::shared_ptr<VNR_TYPE> vnr)
    {
        for (auto it = vnr->getNodeMap()->begin(); it != vnr->getNodeMap()->end(); it++)
        {
            substrate_network->getNode(it->second)->freeResources(it->first);
        }
        for(auto it1 = vnr->getLinkMap()->begin(); it1 != vnr->getLinkMap()->end(); it1++)
        {
            for(auto it2 = it1->second.begin(); it2 != it1->second.end() ;it2++)
            {
                std::cout<< "--------------------------------+++++++++++++++++++++++++++++------------------------" << std::endl;
                std::cout<< vnr->getVN()->getLink(it1->first)->getId() << std::endl;
                std::cout<< *it2<<std::endl;
                std::cout<< "--------------------------------+++++++++++++++++++++++++++++------------------------" << std::endl;
                this->substrate_network->getLink(*it2)->freeResources(it1->first);
            }
        }
    }
}
#endif

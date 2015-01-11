/**
 * @file vy-virtual-net-request.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date 7/16/14
 *
 * @copyright Copyright (c) 7/16/14                      SOROUSH HAERI
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

#ifndef VINEYARD_VY_VIRTUAL_NET_REQUEST_
#define VINEYARD_VY_VIRTUAL_NET_REQUEST_

#include "core/virtual-network-request.h"
#include "vy-virtual-link.h"
#include "vy-virtual-node.h"

namespace vne {
    namespace vineyard {
        
        template<typename = Network<VYVirtualNode<>, VYVirtualLink<>>>
        class VYVirtualNetRequest : public VirtualNetworkRequest<Network<VYVirtualNode<>, VYVirtualLink<>>>
        {
        public:
            VYVirtualNetRequest(std::shared_ptr<Network<VYVirtualNode<>,VYVirtualLink<>>> _vn,
                                double _time, double _duration, int _split, int _topology, int _maxD,
                                std::function<std::shared_ptr<std::pair<double,double>>(const VYVirtualNetRequest<>* vnr)> calcRevenue = nullptr,
                                std::function<std::shared_ptr<std::pair<double,double>>(const VYVirtualNetRequest<>* vnr)> calcCost = nullptr);
            ~VYVirtualNetRequest ();
            int getSplit () const;
            int getTopology () const;
            int getMaxDistance () const;
            virtual double getNodeRevenue () const;
            virtual double getLinkRevenue () const;
            virtual double getTotalRevenue () const;
            virtual double getNodeCost () const;
            virtual double getLinkCost () const;
            virtual double getTotalCost () const;
            // link cost and total cost depend on the mapping, therefore they cannot be calculated
            //  at creation time.
            bool operator> (const VYVirtualNetRequest<>& rhs);
            bool operator< (const VYVirtualNetRequest<>& rhs);
            bool operator> (const double& rhs);
            bool operator< (const double& rhs);
            
            //Values of objective function for link and node mappings.
            double linkMappingObjectiveVal;
            double nodeMappingObjectiveVal;
            
        private:
            int split;
            int topology;
            int maxDistance;
            double nodeRevenue;
            double linkRevenue;
            std::function<std::shared_ptr<std::pair<double,double>> (const VYVirtualNetRequest<>* vnr)> revenue;
            std::function<std::shared_ptr<std::pair<double,double>> (const VYVirtualNetRequest<>* vnr)> cost;
        };
    }
}

#endif /* defined(__vne_mcts__vy_virtual_net_request__) */

/**
 * @file vy-virtual-net-request.cc
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
#include "vy-virtual-net-request.h"
#include "core/config-manager.h"

namespace vne {
    namespace vineyard {
        template<>
        VYVirtualNetRequest<>::VYVirtualNetRequest (std::shared_ptr<Network<VYVirtualNode<>,VYVirtualLink<>>> _vn,
                                                        double _time, double _duration, int _split, int _topology, int _maxD,
                                                    std::function<std::shared_ptr<std::pair<double,double>>(const VYVirtualNetRequest<>* vnr)> calcRevenue,
                                                    std::function<std::shared_ptr<std::pair<double,double>>(const VYVirtualNetRequest<>* vnr)> calcCost) :
        VirtualNetworkRequest<Network<VYVirtualNode<>, VYVirtualLink<> > >(_vn, _time, _duration),
        split(_split),
        topology(_topology),
        maxDistance(_maxD)
        {
            if (calcRevenue == nullptr)
            {
                revenue = [] (const VYVirtualNetRequest<>* vnr) -> std::shared_ptr<std::pair<double,double>>
                {
                    std::shared_ptr<std::pair<double,double>> rev (new std::pair<double,double>(0,0));
                    const std::shared_ptr<std::vector<std::shared_ptr<VYVirtualNode<>>>> n = vnr->getVN()->getAllNodes();
                    for (int i=0;i<n->size();++i)
                    {
                        rev->first += n->at(i)->getCPU ();
                        
                    }
                    const std::shared_ptr<std::vector<std::shared_ptr< VYVirtualLink<>>>>  l = vnr->getVN ()->getAllLinks();
                    for (int j=0;j<l->size();++j)
                    {
                        rev->second += l->at(j)->getBandwidth();
                    }
                    return rev;
                };
            }
            else revenue = calcRevenue;
            if (calcCost == nullptr)
            {
                cost = [] (const VYVirtualNetRequest<>* vnr) -> std::shared_ptr<std::pair<double,double>>
                {
                    std::shared_ptr<std::pair<double,double>>  cost (new std::pair<double,double>(0,0));
                    const std::shared_ptr<std::vector<std::shared_ptr<VYVirtualNode<>>>> n = vnr->getVN()->getAllNodes();
                    for (int i=0;i<n->size();++i)
                    {
                        cost->first += n->at(i)->getCPU ();
                        
                    }
                    int count = 0;
                    BOOST_LOG_TRIVIAL(debug) << "---!!!!!!!!!!!!!!!!!!!!!!!!!!---" << std::endl;
                    for (auto it1 = vnr->getLinkMap()->begin(); it1 != vnr->getLinkMap()->end(); it1++)
                    {
                        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
                        {
                            //I think this calculation is not correct
                            //cost->second += (l->at(j)->getPathLength() * l->at(j)->getBandwidth());
                            BOOST_LOG_TRIVIAL(debug) << "vnr link ID: " << count << " SUB BW: " << std::get<0>(*it2->second) << std::endl;
                            cost->second += std::get<0>(*it2->second);
                        }
                        count++;
                    }
                    return cost;
                };
            }
            else cost = calcCost;
            std::shared_ptr<std::pair<double, double>> revPair = revenue(this);
            nodeRevenue = revPair->first;
            linkRevenue = revPair->second;
        }
        template<>
        VYVirtualNetRequest<>::~VYVirtualNetRequest()
        {
        }
        template<>
        int VYVirtualNetRequest<>::getSplit() const
        {
            return split;
        }
        template<>
        int VYVirtualNetRequest<>::getTopology() const
        {
            return topology;
        }
        template<>
        int VYVirtualNetRequest<>::getMaxDistance() const
        {
            return maxDistance;
        }
        template<>
        double VYVirtualNetRequest<>::getNodeRevenue() const
        {
            return nodeRevenue;
        }
        template<>
        double VYVirtualNetRequest<>::getLinkRevenue() const
        {
            return linkRevenue;
        }
        template<>
        double VYVirtualNetRequest<>::getTotalRevenue() const
        {
            float mult = ConfigManager::Instance()->getConfig<float>("vineyard.Constants.revenueMultiplier");
            return nodeRevenue +  mult * linkRevenue;
        }
        template<>
        double VYVirtualNetRequest<>::getNodeCost() const
        {
            return  cost(this)->first;
        }
        template<>
        double VYVirtualNetRequest<>::getLinkCost() const
        {
            return cost(this)->second;
        }
        template<>
        double VYVirtualNetRequest<>::getTotalCost() const
        {
            float mult = ConfigManager::Instance()->getConfig<float>("vineyard.Constants.costMultiplier");
            return cost(this)->first + mult * cost(this)->second;
        }
        template<>
        bool VYVirtualNetRequest<>::operator> (const VYVirtualNetRequest<>& rhs)
        {
            return maxDistance>rhs.maxDistance;
        }
        template<>
        bool VYVirtualNetRequest<>::operator< (const VYVirtualNetRequest<>& rhs)
        {
            return maxDistance<rhs.maxDistance;
        }
        template<>
        bool VYVirtualNetRequest<>::operator> (const double& rhs)
        {
            return maxDistance>rhs;
        }
        template<>
        bool VYVirtualNetRequest<>::operator< (const double& rhs)
        {
            return maxDistance<rhs;
        }
        template<>
        void VYVirtualNetRequest<>::writeVNRToFile (std::ofstream& ofstrm)
        {
            if (ofstrm.is_open()) {
                ofstrm << vn->getNumNodes() << " " << vn->getNumLinks() << " " << split << " " << "  " << arrivalTime << " " << duration << " " << topology << " " << maxDistance << std::endl;
                vn->writeNetworkToFile(ofstrm,false);
            }
            else
                BOOST_LOG_TRIVIAL(error) << "VYVirtualNetRequest<>::writeVNRToFile: VYThe file is not open for writing. " << std::endl;
        }
    }
}
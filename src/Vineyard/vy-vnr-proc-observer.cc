/**
 * @file vy-vnr-proc-observer.h
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

#include "vy-vnr-proc-observer.h"

namespace vne {
    namespace vineyard{
        
        template<>
        VYVNRProcObserver<>::VYVNRProcObserver (std::shared_ptr<SUBSTRATE_TYPE> _sn)
        : VNRProcessObserver<SUBSTRATE_TYPE, VNR_TYPE>  (_sn)
        {
        }
        template<>
        VYVNRProcObserver<>::~VYVNRProcObserver()
        {
        }
        template<>
        double VYVNRProcObserver<>::ta()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: ta()" << std::endl;
            return DBL_MAX;
        }
        template<>
        void VYVNRProcObserver<>::delta_int()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: delta_int()" << std::endl;
        }
        
        template<>
        inline void VYVNRProcObserver<>::setStatistics (VYStatistics& stat, std::shared_ptr<VYVirtualNetRequest<>> req)
        {
            stat.vnr_id = req->getId();
            double stress;
            std::shared_ptr<const std::set<int> > nodeIdSet = substrate_network->getNodeIdSet();
            std::shared_ptr<const std::set<int>> linkIdSet = substrate_network->getLinkIdSet();
            
	    int numHosts = 0;
            for (auto it = nodeIdSet->begin(); it != nodeIdSet->end(); it++)
            {
                std::shared_ptr<const VYSubstrateNode<>> n = substrate_network->getNode(*it);
		if (n->getMaxCPU() == 0.0)
		  continue;
		numHosts++;
                stress = (n->getMaxCPU() - n->getCPU())/ n->getMaxCPU();
                if (stress > stat.max_node_stress)
                    stat.max_node_stress = stress;
                stat.avg_node_stress += stress;
            }
            stat.avg_node_stress /= numHosts;
            
            for (auto it = nodeIdSet->begin(); it != nodeIdSet->end(); it++)
            {
                std::shared_ptr<const VYSubstrateNode<>> n = substrate_network->getNode(*it);
		if (n->getMaxCPU() == 0.0)
		  continue;
                stress = (n->getMaxCPU() - n->getCPU())/ n->getMaxCPU();
                stat.std_dev_node_stress += ((stat.avg_node_stress - stress) * (stat.avg_node_stress - stress));
            }
            stat.std_dev_node_stress = sqrt((stat.std_dev_node_stress/numHosts));
            
            for (auto it = linkIdSet->begin(); it != linkIdSet->end(); it++)
            {
                std::shared_ptr<const VYSubstrateLink<>> l = substrate_network->getLink(*it);
                stress = ((l->getMaxBandwidth() - l->getBandwidth()) / l->getMaxBandwidth());
                if (stress > stat.max_link_stress)
                    stat.max_link_stress = stress;
                stat.avg_link_stress += stress;
            }
            stat.avg_link_stress /= linkIdSet->size();
            
            for (auto it = linkIdSet->begin(); it != linkIdSet->end(); it++)
            {
                std::shared_ptr<const VYSubstrateLink<>> l = substrate_network->getLink(*it);
                stress = (l->getMaxBandwidth() - l->getBandwidth())/ l->getMaxBandwidth();
                stat.std_dev_link_stress += ((stat.avg_link_stress - stress) * (stat.avg_link_stress - stress));
            }
            stat.std_dev_link_stress = sqrt((stat.std_dev_link_stress/linkIdSet->size()));
            stat.node_revenue = req->getNodeRevenue();
            stat.node_cost = req->getNodeCost();
            stat.link_revenue = req->getLinkRevenue();
            stat.link_cost = req->getLinkCost();
            stat.processing_time = req->getProccessingTime();
            stat.actual_processing_time = req->getActualProccessingTime();
            stat.node_mapping_objective_val = req->nodeMappingObjectiveVal;
            stat.link_mapping_objective_val = req->linkMappingObjectiveVal;
            stat.vnr_arrival_time = req->getArrivalTime();
            stat.vnr_duration = req->getDuration();
        }
        template<>
        void VYVNRProcObserver<>::delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: delta_ext()" << std::endl;
            adevs::Bag<ADEVS_IO_TYPE>::const_iterator i = xb.begin();
            
            for (; i != xb.end (); i++)
            {
                VYStatistics stat;
                if ((*i).port == entered_embedding_queue)
                {
                    stat.event_type = get_Evenet_Type_Str(vne::Event_Types::EVENT_TYPE_ARRIVAL);
                    setStatistics(stat, (*i).value);
                    BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< entered_embedding_queue << ". A VNR has entered embedding queue."  << std::endl;
                }
                else if ((*i).port == embedding_successful)
                {
                    stat.event_type = get_Evenet_Type_Str(vne::Event_Types::EVENT_TYPE_SUCCESSFUL_EMBEDDING);
                    setStatistics(stat, (*i).value);
                    
                    BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< embedding_successful << ". A VNR has been successfully embedded."  << std::endl;
                }
                else if ((*i).port == embedding_unsuccessful)
                {
                    stat.event_type = get_Evenet_Type_Str(vne::Event_Types::EVENT_TYPE_FAIL_EMBEDDING);
                    setStatistics(stat, (*i).value);
                    BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< embedding_unsuccessful << ". A VNR embedding was unsuccessful."  << std::endl;
                    
                }
                else if ((*i).port == released_resources)
                {
                    stat.event_type = get_Evenet_Type_Str(vne::Event_Types::EVENT_TYPE_DEPARTURE);
                    setStatistics(stat, (*i).value);
                    
                    BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< released_resources << ". A VNR left the substrate network."  << std::endl;
                }
                else
                {
                   BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on an unknown port: " << (*i).port << ". A VNR left the substrate network."  << std::endl;
                }
                for (auto it = subscribers.begin (); it != subscribers.end(); it++)
                {
                    (*it)->statisticsGenerated(stat);
                }
            }
        }
        template<>
        void VYVNRProcObserver<>::delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
           BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: delta_conf()" << std::endl;
        }
        template<>
        void VYVNRProcObserver<>::output_func(adevs::Bag<ADEVS_IO_TYPE>& yb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: output_func()" << std::endl;
        }
    }
}

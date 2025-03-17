/**
 * @file experiment-parameters.cc
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 3, 2014
 *
 * @copyright Copyright (c) Jun 3, 2014                      SOROUSH HAERI
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
#ifndef EXPERIMENT_PARAMETERS_H_
#define EXPERIMENT_PARAMETERS_H_

#include "hiberlite/hiberlite.h"
#include "core-types.h"
#include "config-manager.h"

namespace vne {
class ExperimentParameters
    {
        friend class hiberlite::access;
        template<class Archive>
        void hibernate(Archive & ar)
        {
            ar & HIBERLITE_NVP (VNRLinkSplittingRate);
            
            ar & HIBERLITE_NVP (VNRNumNodesDist);
            ar & HIBERLITE_NVP (VNRNumNodesDistParam1);
            ar & HIBERLITE_NVP (VNRNumNodesDistParam2);
            ar & HIBERLITE_NVP (VNRNumNodesDistParam3);
            
            ar & HIBERLITE_NVP (VNRDurationDist);
            ar & HIBERLITE_NVP (VNRDurationDistParam1);
            ar & HIBERLITE_NVP (VNRDurationDistParam2);
            ar & HIBERLITE_NVP (VNRDurationDistParam3);
            
            ar & HIBERLITE_NVP (VNRArrivalDist);
            ar & HIBERLITE_NVP (VNRArrivalDistParam1);
            ar & HIBERLITE_NVP (VNRArrivalDistParam2);
            ar & HIBERLITE_NVP (VNRArrivalDistParam3);
            
            ar & HIBERLITE_NVP (VNRMaxDistanceDist);
            ar & HIBERLITE_NVP (VNRMaxDistanceDistParam1);
            ar & HIBERLITE_NVP (VNRMaxDistanceDistParam2);
            ar & HIBERLITE_NVP (VNRMaxDistanceDistParam3);
            
            ar & HIBERLITE_NVP (SNCPUDist);
            ar & HIBERLITE_NVP (SNCPUDistParam1);
            ar & HIBERLITE_NVP (SNCPUDistParam2);
            ar & HIBERLITE_NVP (SNCPUDistParam3);
            
            ar & HIBERLITE_NVP (SLBWDist);
            ar & HIBERLITE_NVP (SLBWDistParam1);
            ar & HIBERLITE_NVP (SLBWDistParam2);
            ar & HIBERLITE_NVP (SLBWDistParam3);
            
            ar & HIBERLITE_NVP (SLDelayDist);
            ar & HIBERLITE_NVP (SLDelayDistParam1);
            ar & HIBERLITE_NVP (SLDelayDistParam2);
            ar & HIBERLITE_NVP (SLDelayDistParam3);
            
            ar & HIBERLITE_NVP (VNCPUDist);
            ar & HIBERLITE_NVP (VNCPUDistParam1);
            ar & HIBERLITE_NVP (VNCPUDistParam2);
            ar & HIBERLITE_NVP (VNCPUDistParam3);
            
            ar & HIBERLITE_NVP (VLBWDist);
            ar & HIBERLITE_NVP (VLBWDistParam1);
            ar & HIBERLITE_NVP (VLBWDistParam2);
            ar & HIBERLITE_NVP (VLBWDistParam3);
                                
            ar & HIBERLITE_NVP (VLDelayDist);
            ar & HIBERLITE_NVP (VLDelayDistParam1);
            ar & HIBERLITE_NVP (VLDelayDistParam2);
            ar & HIBERLITE_NVP (VLDelayDistParam3);
            
            ar & HIBERLITE_NVP (sn_nodePlacement);
            ar & HIBERLITE_NVP (sn_numNeighbors);
            ar & HIBERLITE_NVP (sn_innerGridSize);
            ar & HIBERLITE_NVP (sn_outerGridSize);
            ar & HIBERLITE_NVP (sn_growthType);
            ar & HIBERLITE_NVP (sn_alpha);
            ar & HIBERLITE_NVP (sn_beta);

            ar & HIBERLITE_NVP (vn_nodePlacement);
            ar & HIBERLITE_NVP (vn_numNeighbors);
            ar & HIBERLITE_NVP (vn_innerGridSize);
            ar & HIBERLITE_NVP (vn_outerGridSize);
            ar & HIBERLITE_NVP (vn_growthType);
            ar & HIBERLITE_NVP (vn_alpha);
            ar & HIBERLITE_NVP (vn_beta);
            
            ar & HIBERLITE_NVP (sn_topology_type);
            ar & HIBERLITE_NVP (sn_dcn_n_switches);
            ar & HIBERLITE_NVP (sn_dcn_n_hosts);
            ar & HIBERLITE_NVP (sn_dcn_n_link);
            ar & HIBERLITE_NVP (sn_bcube_n);
            ar & HIBERLITE_NVP (sn_bcube_k);
            ar & HIBERLITE_NVP (sn_two_tier_core);
            ar & HIBERLITE_NVP (sn_two_tier_edge);
            ar & HIBERLITE_NVP (sn_two_tier_host);
            ar & HIBERLITE_NVP (sn_two_tier_core_bw_multiplier);
            ar & HIBERLITE_NVP (sn_fat_tree_k);
            ar & HIBERLITE_NVP (sn_fat_tree_core_bw_multiplier);
            ar & HIBERLITE_NVP (sn_hypercube_size);
        }
    
    public:
        void setAllParams (ptree &SNParams, ptree &SNNetParams, ptree &VNParams, ptree &VNBriteParams);

    private:
        inline void setVNParams (ptree &pt);
        inline void setSNParams (ptree &pt);
        inline void setVNBriteParams (ptree &pt);
        inline void setSNNetParams (ptree &pt, Topology_Type tt);
        
        int totalTime;
        int SubstrateNodeNum;
        
        double VNRLinkSplittingRate;
        
        std::string VNRNumNodesDist;
        double VNRNumNodesDistParam1;
        double VNRNumNodesDistParam2;
        double VNRNumNodesDistParam3;
        
        std::string VNRDurationDist;
        double VNRDurationDistParam1;
        double VNRDurationDistParam2;
        double VNRDurationDistParam3;
        
        std::string VNRArrivalDist;
        double VNRArrivalDistParam1;
        double VNRArrivalDistParam2;
        double VNRArrivalDistParam3;
        
        std::string VNRMaxDistanceDist;
        double VNRMaxDistanceDistParam1;
        double VNRMaxDistanceDistParam2;
        double VNRMaxDistanceDistParam3;
        
        std::string SNCPUDist;
        double SNCPUDistParam1;
        double SNCPUDistParam2;
        double SNCPUDistParam3;
        
        std::string SLBWDist;
        double SLBWDistParam1;
        double SLBWDistParam2;
        double SLBWDistParam3;
        
        std::string SLDelayDist;
        double SLDelayDistParam1;
        double SLDelayDistParam2;
        double SLDelayDistParam3;
        
        std::string VNCPUDist;
        double VNCPUDistParam1;
        double VNCPUDistParam2;
        double VNCPUDistParam3;
        
        std::string VLBWDist;
        double VLBWDistParam1;
        double VLBWDistParam2;
        double VLBWDistParam3;
        
        std::string VLDelayDist;
        double VLDelayDistParam1;
        double VLDelayDistParam2;
        double VLDelayDistParam3;
        
        std::string sn_nodePlacement;
        int sn_numNeighbors;
        int sn_innerGridSize;
        int sn_outerGridSize;

        std::string sn_growthType;
        double sn_alpha;
        double sn_beta;
        
        std::string vn_nodePlacement;
        int vn_numNeighbors;
        int vn_innerGridSize;
        int vn_outerGridSize;
        
        std::string vn_growthType;
        double vn_alpha;
        double vn_beta;
        
        std::string sn_topology_type;
        int sn_dcn_n_switches;
        int sn_dcn_n_hosts;
        int sn_dcn_n_link;
        int sn_bcube_n;
        int sn_bcube_k;
        int sn_two_tier_core;
        int sn_two_tier_edge;
        int sn_two_tier_host;
        int sn_two_tier_core_bw_multiplier;
        int sn_fat_tree_k;
        int sn_fat_tree_core_bw_multiplier;
        int sn_hypercube_size;
    };
}

#endif

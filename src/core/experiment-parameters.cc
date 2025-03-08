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

#include "core/experiment-parameters.h"

namespace vne {
    void ExperimentParameters::setAllParams(ptree &SNParams, ptree &SNNetParams, ptree &VNParams, ptree &VNBriteParams)
    {
        setSNParams(SNParams);
        setSNNetParams(SNNetParams, get_Topology_Type (SNParams.get<std::string>("SNTopologyType")));
        setVNParams(VNParams);
        setVNBriteParams(VNBriteParams);
    }
    void ExperimentParameters::setSNParams(boost::property_tree::ptree &pt)
    {
        SubstrateNodeNum = pt.get<int> ("SubstrateNodeNum");
        sn_topology_type =  pt.get<std::string>("SNTopologyType");
        SNCPUDist  =  get_Distribution_Str ((Distribution) pt.get<int> ("SNCPUDist"));
        SNCPUDistParam1 = pt.get<double> ("SNCPUDistParam1");
        SNCPUDistParam2 = pt.get<double> ("SNCPUDistParam2");
        SNCPUDistParam3 = pt.get<double> ("SNCPUDistParam3");
        
        SLBWDist = get_Distribution_Str ((Distribution) pt.get<int> ("SLBWDist"));
        SLBWDistParam1 = pt.get<double> ("SLBWDistParam1");
        SLBWDistParam2 = pt.get<double> ("SLBWDistParam2");
        SLBWDistParam3 = pt.get<double> ("SLBWDistParam3");
        
        SLDelayDist = get_Distribution_Str ((Distribution) pt.get<int> ("SLDelayDist"));
        SLDelayDistParam1 = pt.get<double> ("SLDelayDistParam1");
        SLDelayDistParam2 = pt.get<double> ("SLDelayDistParam2");
        SLDelayDistParam3 = pt.get<double> ("SLDelayDistParam3");
        
    }
    
    void ExperimentParameters::setVNParams (boost::property_tree::ptree &pt)
    {
        totalTime = pt.get<int> ("TotalTime");
        VNRLinkSplittingRate = pt.get<double> ("VNRLinkSplittingRate");

        VNRNumNodesDist = get_Distribution_Str ((Distribution) pt.get<int> ("VNRNumNodesDist"));
        VNRNumNodesDistParam1 = pt.get<double> ("VNRNumNodesDistParam1");
        VNRNumNodesDistParam2 = pt.get<double> ("VNRNumNodesDistParam2");
        VNRNumNodesDistParam3 = pt.get<double> ("VNRNumNodesDistParam3");
        
        VNRDurationDist = get_Distribution_Str ((Distribution) pt.get<int> ("VNRDurationDist"));
        VNRDurationDistParam1 = pt.get<double> ("VNRDurationDistParam1");
        VNRDurationDistParam2 = pt.get<double> ("VNRDurationDistParam2");
        VNRDurationDistParam3 = pt.get<double> ("VNRDurationDistParam3");
        
        VNRArrivalDist = get_Distribution_Str ((Distribution) pt.get<int> ("VNRArrivalDist"));
        VNRArrivalDistParam1 = pt.get<double> ("VNRArrivalDistParam1");
        VNRArrivalDistParam2 = pt.get<double> ("VNRArrivalDistParam2");
        VNRArrivalDistParam3 = pt.get<double> ("VNRArrivalDistParam3");
        
        VNRMaxDistanceDist = get_Distribution_Str ((Distribution) pt.get<int> ("VNRMaxDistanceDist"));
        VNRMaxDistanceDistParam1 = pt.get<double> ("VNRMaxDistanceDistParam1");
        VNRMaxDistanceDistParam2 = pt.get<double> ("VNRMaxDistanceDistParam2");
        VNRMaxDistanceDistParam3 = pt.get<double> ("VNRMaxDistanceDistParam3");
        
        VNCPUDist = get_Distribution_Str ((Distribution) pt.get<int> ("VNCPUDist"));
        VNCPUDistParam1 = pt.get<double> ("VNCPUDistParam1");
        VNCPUDistParam2 = pt.get<double> ("VNCPUDistParam2");
        VNCPUDistParam3 = pt.get<double> ("VNCPUDistParam3");
        
        VLBWDist = get_Distribution_Str ((Distribution) pt.get<int> ("VLBWDist"));
        VLBWDistParam1 = pt.get<double> ("VLBWDistParam1");
        VLBWDistParam2 = pt.get<double> ("VLBWDistParam2");
        VLBWDistParam3 = pt.get<double> ("VLBWDistParam3");
        
        VLDelayDist = get_Distribution_Str ((Distribution) pt.get<int> ("VLDelayDist"));
        VLDelayDistParam1 = pt.get<double> ("VLDelayDistParam1");
        VLDelayDistParam2 = pt.get<double> ("VLDelayDistParam2");
        VLDelayDistParam3 = pt.get<double> ("VLDelayDistParam3");
    }
    
    void ExperimentParameters::setSNNetParams (boost::property_tree::ptree &pt, Topology_Type tt)
    {

        if (tt == Topology_Type::DCNBCube)
        {
            sn_dcn_n_switches = pt.get<int> ("n_switches");
            sn_dcn_n_hosts = pt.get<int> ("n_hosts");
            sn_dcn_n_link = pt.get<int>("n_links");
            sn_bcube_k = pt.get<int>("DCNBCube.K");
            sn_bcube_n = pt.get<int>("DCNBCube.N");
        }
        else if (tt == Topology_Type::DCNFatTree)
        {
            sn_dcn_n_switches = pt.get<int> ("n_switches");
            sn_dcn_n_hosts = pt.get<int> ("n_hosts");
            sn_dcn_n_link = pt.get<int>("n_links");
            sn_fat_tree_k = pt.get<int>("DCNFatTree.K");
            sn_fat_tree_core_bw_multiplier = pt.get<int>("DCNFatTree.coreBWMultiplier");
        }
        else if (tt == Topology_Type::DCNTwoTier)
        {
            sn_dcn_n_switches = pt.get<int> ("n_switches");
            sn_dcn_n_hosts = pt.get<int> ("n_hosts");
            sn_dcn_n_link = pt.get<int>("n_links");
            sn_two_tier_core = pt.get<int>("DCNTwoTier.n_core");
            sn_two_tier_edge = pt.get<int>("DCNTwoTier.n_edges");
            sn_two_tier_host = pt.get<int>("DCNTwoTier.n_hosts");
            sn_two_tier_core_bw_multiplier = pt.get<int>("DCNTwoTier.coreBWMultiplier");
        }
		else if (tt == Topology_Type::HyperCube)
		{
            sn_hypercube_size = pt.get<int>("HyperCube.size");
		}
        else
        {
            int node_placement = pt.get<int> ("nodePlacement");
            sn_nodePlacement = (node_placement == 1)? "Random" : "Heavy-tailed";
            sn_numNeighbors = pt.get<int> ("numNeighbors");
            sn_innerGridSize = pt.get<int> ("innerGridSize");
            sn_outerGridSize = pt.get<int> ("outerGridSize");
            
            int grow_type = pt.get<int> ("RTWaxman.growthType");
            sn_growthType = (grow_type == 1) ? "Incremental" : "All";
            sn_alpha = pt.get<double> ("RTWaxman.alpha");
            sn_beta = pt.get<double> ("RTWaxman.beta");
        }
    }
    
    void ExperimentParameters::setVNBriteParams (boost::property_tree::ptree &pt)
    {
        int node_placement = pt.get<int> ("nodePlacement");
        vn_nodePlacement = (node_placement == 1)? "Random" : "Heavy-tailed";
        vn_numNeighbors = pt.get<int> ("numNeighbors");
        vn_innerGridSize = pt.get<int> ("innerGridSize");
        vn_outerGridSize = pt.get<int> ("outerGridSize");
        
        int grow_type = pt.get<int> ("RTWaxman.growthType");
        vn_growthType = (grow_type == 1) ? "Incremental" : "All";
        vn_alpha = pt.get<double> ("RTWaxman.alpha");
        vn_beta = pt.get<double> ("RTWaxman.beta");
    }
}
using namespace vne;
HIBERLITE_EXPORT_CLASS(ExperimentParameters);
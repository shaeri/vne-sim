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

#include "toml.hpp"
#include "core/experiment-parameters.h"

namespace vne
{
void ExperimentParameters::setAllParams(vne::ConfigType &SNParams, vne::ConfigType &VNParams)
{
    setSNParams(SNParams);
    setSNNetParams(SNParams, get_Topology_Type(toml::find<std::string>(
                                 SNParams, "NetworkFileGenerator", "SNTopologyType")));
    setVNParams(VNParams);
    setVNBriteParams(VNParams);
}
void ExperimentParameters::setSNParams(vne::ConfigType &pt)
{
    SubstrateNodeNum = toml::find<int>(pt, "NetworkFileGenerator", "SubstrateNodeNum");
    sn_topology_type = toml::find<std::string>(pt, "NetworkFileGenerator", "SNTopologyType");
    SNCPUDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "SNCPUDist"));
    SNCPUDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "SNCPUDistParam1");
    SNCPUDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "SNCPUDistParam2");
    SNCPUDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "SNCPUDistParam3");

    SLBWDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "SLBWDist"));
    SLBWDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "SLBWDistParam1");
    SLBWDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "SLBWDistParam2");
    SLBWDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "SLBWDistParam3");

    SLDelayDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "SLDelayDist"));
    SLDelayDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "SLDelayDistParam1");
    SLDelayDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "SLDelayDistParam2");
    SLDelayDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "SLDelayDistParam3");
}

void ExperimentParameters::setVNParams(vne::ConfigType &pt)
{
    totalTime = toml::find<int>(pt, "NetworkFileGenerator", "TotalTime");
    VNRLinkSplittingRate = toml::find<double>(pt, "NetworkFileGenerator", "VNRLinkSplittingRate");

    VNRNumNodesDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VNRNumNodesDist"));
    VNRNumNodesDistParam1 = toml::find<double>(pt, "NetworkFileGenerator",
                                               "VNRNumNodesDistParam1");
    VNRNumNodesDistParam2 = toml::find<double>(pt, "NetworkFileGenerator",
                                               "VNRNumNodesDistParam2");
    VNRNumNodesDistParam3 = toml::find<double>(pt, "NetworkFileGenerator",
                                               "VNRNumNodesDistParam3");

    VNRDurationDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VNRDurationDist"));
    VNRDurationDistParam1 = toml::find<double>(pt, "NetworkFileGenerator",
                                               "VNRDurationDistParam1");
    VNRDurationDistParam2 = toml::find<double>(pt, "NetworkFileGenerator",
                                               "VNRDurationDistParam2");
    VNRDurationDistParam3 = toml::find<double>(pt, "NetworkFileGenerator",
                                               "VNRDurationDistParam3");

    VNRArrivalDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VNRArrivalDist"));
    VNRArrivalDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "VNRArrivalDistParam1");
    VNRArrivalDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "VNRArrivalDistParam2");
    VNRArrivalDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "VNRArrivalDistParam3");

    VNRMaxDistanceDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VNRMaxDistanceDist"));
    VNRMaxDistanceDistParam1 = toml::find<double>(pt, "NetworkFileGenerator",
                                                  "VNRMaxDistanceDistParam1");
    VNRMaxDistanceDistParam2 = toml::find<double>(pt, "NetworkFileGenerator",
                                                  "VNRMaxDistanceDistParam2");
    VNRMaxDistanceDistParam3 = toml::find<double>(pt, "NetworkFileGenerator",
                                                  "VNRMaxDistanceDistParam3");

    VNCPUDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VNCPUDist"));
    VNCPUDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "VNCPUDistParam1");
    VNCPUDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "VNCPUDistParam2");
    VNCPUDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "VNCPUDistParam3");

    VLBWDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VLBWDist"));
    VLBWDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "VLBWDistParam1");
    VLBWDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "VLBWDistParam2");
    VLBWDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "VLBWDistParam3");

    VLDelayDist = get_Distribution_Str(
        (Distribution)toml::find<int>(pt, "NetworkFileGenerator", "VLDelayDist"));
    VLDelayDistParam1 = toml::find<double>(pt, "NetworkFileGenerator", "VLDelayDistParam1");
    VLDelayDistParam2 = toml::find<double>(pt, "NetworkFileGenerator", "VLDelayDistParam2");
    VLDelayDistParam3 = toml::find<double>(pt, "NetworkFileGenerator", "VLDelayDistParam3");
}

void ExperimentParameters::setSNNetParams(vne::ConfigType &pt, Topology_Type tt)
{
    if (tt == Topology_Type::DCNBCube) {
        sn_dcn_n_switches = toml::find<int>(pt, "n_switches");
        sn_dcn_n_hosts = toml::find<int>(pt, "n_hosts");
        sn_dcn_n_link = toml::find<int>(pt, "n_links");

        sn_bcube_k = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "DCNBCube", "K");
        sn_bcube_n = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "DCNBCube", "N");
    } else if (tt == Topology_Type::DCNFatTree) {
        sn_dcn_n_switches = toml::find<int>(pt, "n_switches");
        sn_dcn_n_hosts = toml::find<int>(pt, "n_hosts");
        sn_dcn_n_link = toml::find<int>(pt, "n_links");

        sn_fat_tree_k = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "DCNFatTree",
                                        "K");
        sn_fat_tree_core_bw_multiplier = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler",
                                                         "DCNFatTree", "coreBWMultiplier");
    } else if (tt == Topology_Type::DCNTwoTier) {
        sn_dcn_n_switches = toml::find<int>(pt, "n_switches");
        sn_dcn_n_hosts = toml::find<int>(pt, "n_hosts");
        sn_dcn_n_link = toml::find<int>(pt, "n_links");

        sn_two_tier_core = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_core");
        sn_two_tier_edge = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_edges");
        sn_two_tier_host = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_hosts");
        sn_two_tier_core_bw_multiplier = toml::find<int>(pt,"NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "coreBWMultiplier");
    } else if (tt == Topology_Type::HyperCube) {
        sn_hypercube_size = toml::find<int>(pt, "NetworkFileGenerator", "FNSSHandler", "HyperCube",
                                            "size");
    } else {
        int node_placement = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                             "nodePlacement");
        sn_nodePlacement = (node_placement == 1) ? "Random" : "Heavy-tailed";
        sn_numNeighbors = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                          "numNeighbors");
        sn_innerGridSize = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                           "innerGridSize");
        sn_outerGridSize = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                           "outerGridSize");

        int grow_type = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler", "RTWaxman",
                                        "growthType");
        sn_growthType = (grow_type == 1) ? "Incremental" : "All";
        sn_alpha = toml::find<double>(pt, "NetworkFileGenerator", "BriteHandler", "RTWaxman",
                                      "alpha");
        sn_beta = toml::find<double>(pt, "NetworkFileGenerator", "BriteHandler", "RTWaxman",
                                     "beta");
    }
}

void ExperimentParameters::setVNBriteParams(vne::ConfigType &pt)
{
    int node_placement = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                         "nodePlacement");
    vn_nodePlacement = (node_placement == 1) ? "Random" : "Heavy-tailed";
    vn_numNeighbors = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler", "numNeighbors");
    vn_innerGridSize = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                       "innerGridSize");
    vn_outerGridSize = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler",
                                       "outerGridSize");

    int grow_type = toml::find<int>(pt, "NetworkFileGenerator", "BriteHandler", "RTWaxman",
                                    "growthType");
    vn_growthType = (grow_type == 1) ? "Incremental" : "All";
    vn_alpha = toml::find<double>(pt, "NetworkFileGenerator", "BriteHandler", "RTWaxman", "alpha");
    vn_beta = toml::find<double>(pt, "NetworkFileGenerator", "BriteHandler", "RTWaxman", "beta");
}
}  // namespace vne
using namespace vne;
HIBERLITE_EXPORT_CLASS(ExperimentParameters);

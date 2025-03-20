/**
 * @file network-file-generator.h
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

#ifndef NFG_NETWORK_FILE_GENERATOR_
#define NFG_NETWORK_FILE_GENERATOR_

#include "BRITE/C++/Brite.h"

#include "brite-handler.h"
#ifdef WITH_FNSS_SUPPORT
#include "fnss-handler.h"
#endif

#include "core/core-types.h"
#include "core/config-manager.h"

#include "Vineyard/vy-substrate-link.h"
#include "Vineyard/vy-substrate-node.h"

using namespace vne::vineyard;

namespace vne
{
namespace nfg
{
    class NetworkFileGenerator
    {
       public:
        struct Parameters {
            Parameters();

            std::string DirToSaveFiles;
            int totalTime;
            int SubstrateNodeNum;

            double VNRLinkSplittingRate;

            Distribution VNRNumNodesDist;
            double VNRNumNodesDistParam1;
            double VNRNumNodesDistParam2;
            double VNRNumNodesDistParam3;

            Distribution VNRDurationDist;
            double VNRDurationDistParam1;
            double VNRDurationDistParam2;
            double VNRDurationDistParam3;

            Distribution VNRArrivalDist;
            double VNRArrivalDistParam1;
            double VNRArrivalDistParam2;
            double VNRArrivalDistParam3;

            Distribution VNRMaxDistanceDist;
            double VNRMaxDistanceDistParam1;
            double VNRMaxDistanceDistParam2;
            double VNRMaxDistanceDistParam3;

            Distribution SNCPUDist;
            double SNCPUDistParam1;
            double SNCPUDistParam2;
            double SNCPUDistParam3;

            Distribution SLBWDist;
            double SLBWDistParam1;
            double SLBWDistParam2;
            double SLBWDistParam3;

            Distribution SLDelayDist;
            double SLDelayDistParam1;
            double SLDelayDistParam2;
            double SLDelayDistParam3;

            Distribution VNCPUDist;
            double VNCPUDistParam1;
            double VNCPUDistParam2;
            double VNCPUDistParam3;

            Distribution VLBWDist;
            double VLBWDistParam1;
            double VLBWDistParam2;
            double VLBWDistParam3;

            Distribution VLDelayDist;
            double VLDelayDistParam1;
            double VLDelayDistParam2;
            double VLDelayDistParam3;

            vne::ConfigType vnrpt;
            vne::ConfigType snpt;

            std::string Handler;
            Topology_Type vn_topo_type;
            Topology_Type sn_topo_type;
        };

        NetworkFileGenerator();
        ~NetworkFileGenerator();

        std::shared_ptr<Network<VYSubstrateNode<>, VYSubstrateLink<>>> VYSubstrateNetFileGenerator(
            bool writeToFile);

        std::shared_ptr<std::list<std::shared_ptr<VYVirtualNetRequest<>>>>
        VYVirtualNetRequestGenerator(bool writeToFile);

       private:
        Parameters params;
    };
}  // namespace nfg
}  // namespace vne
#endif

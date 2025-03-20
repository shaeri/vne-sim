/**
 * @file vy-vnr-relase-proc.h
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

#ifndef VINEYARD_VY_VNR_RELEASE_PROC_
#define VINEYARD_VY_VNR_RELEASE_PROC_

#include "core/vnr-release-processor.h"

#include "Vineyard/vy-virtual-net-request.h"
#include "Vineyard/vy-substrate-node.h"

namespace vne
{
namespace vineyard
{

    template <typename = Network<VYSubstrateNode<>, VYSubstrateLink<>>,
              typename = VYVirtualNetRequest<>>
    class VYVNRReleaseProc
        : public VNRReleaseProcessor<Network<VYSubstrateNode<>, VYSubstrateLink<>>,
                                     VYVirtualNetRequest<>>
    {
       public:
        VYVNRReleaseProc(std::shared_ptr<ReleaseAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> releaseAlgo);
        ~VYVNRReleaseProc();

        virtual void delta_int();
        virtual void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE> &xb);
        virtual void delta_conf(const adevs::Bag<ADEVS_IO_TYPE> &xb);
        virtual void output_func(adevs::Bag<ADEVS_IO_TYPE> &yb);
        virtual double ta();

       private:
        double last_departure_time;
    };
}  // namespace vineyard
}  // namespace vne
#endif /* defined(__vne_mcts__vy_vnr_processor__) */
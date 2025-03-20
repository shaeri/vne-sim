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

#ifndef VINEYARD_VY_VNR_PROC_OBSERVER_
#define VINEYARD_VY_VNR_PROC_OBSERVER_

#include "vy-virtual-net-request.h"
#include "vy-substrate-node.h"
#include "vy-statistics.h"

#include "core/vnr-process-observer.h"

namespace vne
{
namespace vineyard
{
    template <typename = Network<VYSubstrateNode<>, VYSubstrateLink<>>,
              typename = VYVirtualNetRequest<>>
    class VYVNRProcObserver
        : public VNRProcessObserver<Network<VYSubstrateNode<>, VYSubstrateLink<>>,
                                    VYVirtualNetRequest<>>
    {
       public:
        VYVNRProcObserver(std::shared_ptr<SUBSTRATE_TYPE> _sn);
        virtual ~VYVNRProcObserver();

        virtual void delta_int();
        virtual void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE> &xb);
        virtual void delta_conf(const adevs::Bag<ADEVS_IO_TYPE> &xb);
        virtual void output_func(adevs::Bag<ADEVS_IO_TYPE> &yb);
        virtual double ta();

       private:
        inline void setStatistics(VYStatistics &stat, std::shared_ptr<VYVirtualNetRequest<>> req);
    };
}  // namespace vineyard
}  // namespace vne

#endif /* defined(__vne_mcts__vy_vnr_proc_observer__) */
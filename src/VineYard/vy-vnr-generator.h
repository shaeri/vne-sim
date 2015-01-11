/**
 * @file vy-vnr-generator.h
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

#ifndef VINEYARD_VY_VNR_GENERATOR_
#define VINEYARD_VY_VNR_GENERATOR_

#include "core/vnr-generator.h"
#include "core/config-manager.h"

#include "vy-virtual-net-request.h"

#include <queue>

#include <boost/filesystem.hpp>
/*
 * This class reads the request files.
 * To use this class, you need to have the
 * request files in numbered in ascending order
 * based on the arrival time of the VNRs.
 */
namespace vne {
    namespace vineyard {
        using namespace boost::filesystem;
        struct CompareRequestPaths{
            bool operator()(const std::pair<int, path>& lhs, const std::pair<int, path>& rhs)
            {
                return lhs.first>rhs.first;
            }
        };
        template<typename = VYVirtualNetRequest<>>
        class VYVNRGenerator : public VNRGenerator<VYVirtualNetRequest<>>
        {
        public:
            VYVNRGenerator (
        std::function<std::shared_ptr<std::pair<double,double>>(const VYVirtualNetRequest<>* vnr)> calcRevenue = nullptr,
        std::function<std::shared_ptr<std::pair<double,double>>(const VYVirtualNetRequest<>* vnr)> calcCost =nullptr);
            
            virtual ~VYVNRGenerator ();
            virtual void delta_int();
            virtual void output_func(adevs::Bag<ADEVS_IO_TYPE>& yb);
            virtual double ta();
            
        private:
            std::shared_ptr<VYVirtualNetRequest<>> vnr;
            std::function<std::shared_ptr<std::pair<double,double>> (const VYVirtualNetRequest<>* vnr)>  revenue;
            std::function<std::shared_ptr<std::pair<double,double>> (const VYVirtualNetRequest<>* vnr)>  cost;
            std::priority_queue<std::pair<int, path>, std::vector<std::pair<int, path>>, CompareRequestPaths> _reqFiles;
            inline void setVNR ();
            double last_vnr_arrival;
        };
    }
}
#endif /* defined(__vne_mcts__vy_virtual_net_request_builder__) */

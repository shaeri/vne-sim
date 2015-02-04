/**
 * @file vy-vnr-embedding-proc.h
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

#ifndef VINEYARD_VY_VNR_EMBEDDING_PROC_
#define VINEYARD_VY_VNR_EMBEDDING_PROC_

#include "core/vnr-embedding-processor.h"

#include "Vineyard/vy-virtual-net-request.h"
#include "Vineyard/vy-substrate-node.h"

#include <chrono>
#include <random>

namespace vne {
    namespace vineyard {
        
        template<typename = Network<VYSubstrateNode<>, VYSubstrateLink<>>,typename = VYVirtualNetRequest<>>
        class VYVNREmbeddingProc :
                public VNREmbeddingProcessor <Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>
        {
        public:
            VYVNREmbeddingProc (std::shared_ptr<EmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>, VYVirtualNetRequest<>>> embeddingAlgo);
            ~VYVNREmbeddingProc ();
            
            virtual void delta_int();
            virtual void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb);
            virtual void delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb);
            virtual void output_func(adevs::Bag<ADEVS_IO_TYPE>& yb);
            virtual double ta();
            
        private:
            std::chrono::time_point<std::chrono::system_clock> start, end;
            std::chrono::duration<double> elapsed_seconds;
        };
    }
}
#endif /* defined(__vne_mcts__vy_vnr_processor__) */
/**
 * @file vy-vnr-proc-digraph.h
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

#ifndef VINEYARD_VY_VNR_PROC_DIGRAPH_
#define VINEYARD_VY_VNR_PROC_DIGRAPH_

#include "core/vnr-process-digraph.h"

#include "Vineyard/vy-vnr-release-proc.h"
#include "Vineyard/vy-vnr-proc-observer.h"
#include "Vineyard/vy-vnr-generator.h"
#include "Vineyard/vy-vnr-embedding-proc.h"

namespace vne {
    namespace vineyard {
        template<
            typename = VYVNRGenerator<>, typename = VYVNREmbeddingProc<>,
            typename = VYVNRReleaseProc<>, typename = VYVNRProcObserver<>>
        class VYVNRProcDigraph
        : public VNRProcessDigraph<VYVNRGenerator<>, VYVNREmbeddingProc<>, VYVNRReleaseProc<>, VYVNRProcObserver<>>
        {
        public:
            VYVNRProcDigraph (VYVNREmbeddingProc<>* embed,
                              VYVNRReleaseProc<>* release,
                              VYVNRGenerator<>* gen,
                              VYVNRProcObserver<>* obs);
            ~VYVNRProcDigraph ();
        };
    }
}
#endif /* defined(__vne_mcts__vy_vnr_proc_digraph__) */
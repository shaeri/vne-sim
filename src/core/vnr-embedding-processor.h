/**
 * @file vnr-embedding-processor.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 12, 2014
 *
 * @copyright Copyright (c) Jun 12, 2014                      SOROUSH HAERI
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
#ifndef VNR_EMBEDDING_PROCESSOR_H_
#define VNR_EMBEDDING_PROCESSOR_H_

#include "adevs/adevs.h"
#include "core/virtual-network-request.h"
#include "core/embedding-algorithm.h"

#include <queue>

namespace vne
{
    template<typename, typename> class VNREmbeddingProcessor;
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    class VNREmbeddingProcessor<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>
    : public adevs::Atomic<adevs::PortValue<std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> >>
    {
    public:
        
        static_assert (std::is_base_of<
                       VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>,
                       VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::value,
                       "Template argument of VNREmbeddingProcessor must be derived from VirtualNetworkRequest.");
        
        typedef adevs::PortValue<std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> > ADEVS_IO_TYPE;
        typedef std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> PTR_TYPE;
        
        virtual ~VNREmbeddingProcessor(){};
        virtual void delta_int() = 0;
        virtual void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb) = 0;
        virtual void delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb) = 0;
        virtual void output_func(adevs::Bag<ADEVS_IO_TYPE>& yb) = 0;
        virtual double ta() = 0;
        virtual void gc_output(adevs::Bag<ADEVS_IO_TYPE>& g) {};
        
        static const int arrive;
        static const int depart_successful_embedding;
        static const int depart_unsuccessful_embedding;
        
        struct CompareVNRArrivalTime{
            bool operator()(const PTR_TYPE lhs, const PTR_TYPE rhs)
            {
                return lhs->getArrivalTime() > rhs->getArrivalTime();
            }
         };
    protected:
        typedef Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>> SUBSTRATE_TYPE;
        typedef VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> VNR_TYPE;
        
        double time;
        std::priority_queue<PTR_TYPE, vector<PTR_TYPE>,CompareVNRArrivalTime> vnr_queue;
        std::shared_ptr<EmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> embeddingAlgorithm;
        
        VNREmbeddingProcessor(std::shared_ptr<EmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> embeddingAlgo) :
        adevs::Atomic<ADEVS_IO_TYPE> (),
        embeddingAlgorithm(std::move(embeddingAlgo)),
        time (0.0)
        {};
    };
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNREmbeddingProcessor<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::arrive = 0;
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNREmbeddingProcessor<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::depart_successful_embedding = 1;
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNREmbeddingProcessor<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::depart_unsuccessful_embedding = 2;
}
#endif /* VNR_EMBEDDING_PROCESSOR_H_ */
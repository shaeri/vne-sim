/**
 * @file vnr-process-observer.h
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
#ifndef VNR_PROCESS_OBSERVER_H_
#define VNR_PROCESS_OBSERVER_H_

#include "adevs/adevs.h"
#include "core/virtual-network-request.h"
#include "core/statistics-subscriber.h"

namespace vne
{
    template<typename,typename> class VNRProcessObserver;
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    class VNRProcessObserver<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>
    : public adevs::Atomic<adevs::PortValue<std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> >>
    {
    public:
        
        static_assert (std::is_base_of<
                       VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>,
                       VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::value,
                       "Template argument of VNRProcessObserver must be derived from VirtualNetworkRequest.");
        
        typedef adevs::PortValue<std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> > ADEVS_IO_TYPE;
        typedef std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> PTR_TYPE;
        
        virtual ~VNRProcessObserver(){};
        virtual void delta_int() {};
        virtual void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb) = 0;
        virtual void delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb) {};
        virtual void output_func(adevs::Bag<ADEVS_IO_TYPE>& yb) {};
        virtual double ta() {return DBL_MAX;};
        virtual void gc_output(adevs::Bag<ADEVS_IO_TYPE>& g) {};
        
        void registerSubscriber (StatisticsSubscriber* subscriber) {subscribers.push_back(subscriber);};
        
        static const int entered_embedding_queue;
        static const int embedding_successful;
        static const int embedding_unsuccessful;
        static const int released_resources;
        
    protected:
        typedef Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>> SUBSTRATE_TYPE;
        typedef VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> VNR_TYPE;
        
        std::shared_ptr<SUBSTRATE_TYPE> substrate_network;
        
        std::list<StatisticsSubscriber*> subscribers;
        
        VNRProcessObserver(std::shared_ptr<SUBSTRATE_TYPE> _sn) :
        substrate_network(std::move(_sn)),
        adevs::Atomic<ADEVS_IO_TYPE> ()
        {};
    };
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNRProcessObserver<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::entered_embedding_queue = 0;
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNRProcessObserver<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::embedding_successful = 1;
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNRProcessObserver<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::embedding_unsuccessful = 2;
    
    template<template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNRProcessObserver<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::released_resources = 3;
}
#endif /* VNR_EMBEDDING_PROCESSOR_H_ */

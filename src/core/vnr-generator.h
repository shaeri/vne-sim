/**
 * @file vnr-generator.h
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

#ifndef VNR_GENERATOR_H_
#define VNR_GENERATOR_H_

#include "core/virtual-network-request.h"
#include "adevs/include/adevs.h"

namespace vne{
    //template <typename ...> class NetworkBuilder;
    //template<typename ... NODET, typename ... LINKT>
    //class NetworkBuilder<Network<Node<NODET...>, Link <LINKT...>>>{
    
    template<typename> class VNRGenerator;
    template<template<typename> class VNR,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    class VNRGenerator<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>
    : public adevs::Atomic<adevs::PortValue<std::shared_ptr<VNR
                <Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> >>
    {
    public:
        static_assert (std::is_base_of<
                       VirtualNetworkRequest<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>,
                       VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::value,
                       "Template argument of VNRGenerator must be derived from VirtualNetworkRequest.");
        
        typedef adevs::PortValue< std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> > ADEVS_IO_TYPE;
        typedef std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> PTR_TYPE;
        
        virtual ~VNRGenerator() {};
        
        // This is the internal transition function. It should entail the logic of the sequence of events
        // that will occur after an output has been generated.
        virtual void delta_int() = 0;
        
        // The generator is input free, and so it ignores external events.
        virtual void delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb) {} ;
        // The generator is input free, and so it ignores input.
        virtual void delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb) {delta_int();};
        virtual void output_func(adevs::Bag<ADEVS_IO_TYPE>& yb) = 0;
        virtual double ta() = 0;
        virtual void gc_output(adevs::Bag<ADEVS_IO_TYPE>& g) {};
        
        static const int arrive;
        //virtual std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> getVNR () = 0;
        
    protected:
        VNRGenerator()
        : adevs::Atomic<ADEVS_IO_TYPE> ()
        {};
    };
    template<template<typename> class VNR,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    const int VNRGenerator<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::arrive = 0;
}
#endif

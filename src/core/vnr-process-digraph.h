/**
 * @file vnr-process-digraph.h
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

#ifndef VNR_PROCESS_DIGRAPH_H_
#define VNR_PROCESS_DIGRAPH_H_

#include "adevs.h"
#include "core/vnr-release-processor.h"
#include "core/vnr-embedding-processor.h"
#include "core/vnr-generator.h"
#include "core/vnr-process-observer.h"

namespace vne {
    template<typename,typename,typename,typename> class VNRProcessDigraph;
    template<
    template<typename> class GEN,
    template<typename, typename> class EMBEDDING_PROC,
    template<typename, typename> class RELEASE_PROC,
    template<typename, typename> class OBSERVER,
    template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    class VNRProcessDigraph<
    GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    EMBEDDING_PROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    RELEASE_PROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    OBSERVER<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>
    >
    : public adevs::Digraph<std::shared_ptr<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>> >
    {
    public:
        
        typedef VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> VNR_TYPE;
        typedef Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>> SUBSTRATE_TYPE;
        
        VNRProcessDigraph (EMBEDDING_PROC<SUBSTRATE_TYPE, VNR_TYPE>* _vnr_embedding,
                           RELEASE_PROC<SUBSTRATE_TYPE, VNR_TYPE>* _vnr_release,
                           GEN<VNR_TYPE>* _vnr_generator,
                           OBSERVER<SUBSTRATE_TYPE, VNR_TYPE>* _vnr_observer);
    private:
        inline void init ();
        GEN<VNR_TYPE>* vnr_generator;
        EMBEDDING_PROC<SUBSTRATE_TYPE, VNR_TYPE>* vnr_embedding;
        RELEASE_PROC<SUBSTRATE_TYPE, VNR_TYPE>* vnr_release;
        OBSERVER<SUBSTRATE_TYPE,VNR_TYPE>* vnr_observer;
        
    };
    
    template<
    template<typename> class GEN,
    template<typename, typename> class EMBEDDING_PROC,
    template<typename, typename> class RELEASE_PROC,
    template<typename, typename> class OBSERVER,
    template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    inline void VNRProcessDigraph<
    GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    EMBEDDING_PROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    RELEASE_PROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    OBSERVER<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>>::init ()
    {
        this->add(vnr_generator);
        this->add(vnr_embedding);
        this->add(vnr_release);
        this->add(vnr_observer);
        
        this->couple (vnr_generator, vnr_generator->arrive, vnr_embedding, vnr_embedding->arrive);
        this->couple (vnr_generator, vnr_generator->arrive, vnr_observer, vnr_observer->entered_embedding_queue);
        
        this->couple (vnr_embedding, vnr_embedding->depart_successful_embedding, vnr_release, vnr_release->arrive);
        this->couple (vnr_embedding, vnr_embedding->depart_successful_embedding, vnr_observer, vnr_observer->embedding_successful);
        
        this->couple (vnr_embedding, vnr_embedding->depart_unsuccessful_embedding, vnr_observer, vnr_observer->embedding_unsuccessful);
        
        this->couple (vnr_release, vnr_release->depart, vnr_observer, vnr_observer->released_resources);
    }
    
    template<
    template<typename> class GEN,
    template<typename, typename> class EMBEDDING_PROC,
    template<typename, typename> class RELEASE_PROC,
    template<typename, typename> class OBSERVER,
    template<typename> class VNR,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    VNRProcessDigraph<
    GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    EMBEDDING_PROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    RELEASE_PROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
    OBSERVER<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>,
                    VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>>::
    VNRProcessDigraph (EMBEDDING_PROC<SUBSTRATE_TYPE, VNR_TYPE>* _vnr_embedding,
                       RELEASE_PROC<SUBSTRATE_TYPE, VNR_TYPE>* _vnr_release,
                       GEN<VNR_TYPE>* _vnr_generator,
                       OBSERVER<SUBSTRATE_TYPE,VNR_TYPE>* _vnr_observer):
    adevs::Digraph<std::shared_ptr<VNR_TYPE>> (),
    vnr_embedding(_vnr_embedding),
    vnr_release(_vnr_release),
    vnr_generator(_vnr_generator),
    vnr_observer(_vnr_observer)
    {
        init ();
    }
}
#endif
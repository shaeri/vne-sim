/**
 * @file experiment.h
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

#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_


#include "core-types.h"
#include "embedding-algorithm.h"
#include "vnr-process-digraph.h"
#include "network-builder.h"
#include "statistics-subscriber.h"
#include "experiment-parameters.h"

#include "hiberlite/hiberlite.h"

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

namespace vne {
    template<typename> class Experiment;
    template<
    template<typename> class VNR,
    template<typename,typename,typename,typename> class PROCDIGRAPH,
    template<typename,typename> class EMBEDPROC,
    template<typename,typename> class OBSERVER,
    template<typename,typename> class RELEASEPROC,
    template<typename> class GEN,
    typename ... SUBNODERES, template <typename ...> class SUBNODECLASS,
    typename ... SUBLINKRES, template <typename ...> class SUBLINKCLASS,
    typename ... NODERES, template <typename ...> class NODECLASS,
    typename ... LINKRES, template <typename ...> class LINKCLASS>
    class Experiment<
        PROCDIGRAPH<
            GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
            EMBEDPROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>, VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
            RELEASEPROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>, VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
            OBSERVER<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>, VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>
    >>>> : public  StatisticsSubscriber
    {
        friend class hiberlite::access;
        template<class Archive>
        void hibernate(Archive & ar)
        {
            ar & HIBERLITE_NVP(algo_type);
            ar & HIBERLITE_NVP(node_mapping_algo);
            ar & HIBERLITE_NVP(link_mapping_algo);
            ar & HIBERLITE_NVP(params);
#ifdef ENABLE_MPI
            ar & HIBERLITE_NVP (num_mpi_proc);
#endif
        }

    public:
        virtual void statisticsGenerated (Statistics& stat) = 0;
        virtual void run ()
        {
            while (sim->nextEventTime() < DBL_MAX)
            {
                sim->execNextEvent();
            }
        };
        virtual ~Experiment () {};
        
    protected:
        Experiment () {};
        
        typedef Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>> SUBSTRATE_TYPE;
        typedef VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>> VNR_TYPE;
        typedef PROCDIGRAPH<
        GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
        EMBEDPROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>, VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
        RELEASEPROC<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>, VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>,
        OBSERVER<Network<SUBNODECLASS<SUBNODERES...>, SUBLINKCLASS<SUBLINKRES...>>, VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>
        >>> DIGRAPH_TYPE;
        
        std::string algo_type;
        std::string node_mapping_algo;
        std::string link_mapping_algo;

#ifdef ENABLE_MPI
        int num_mpi_proc;
#endif
        
        ExperimentParameters params;
        
        void initialize (DIGRAPH_TYPE* _proc_digraph, Embedding_Algorithm_Types _algo_t, std::string _node_mapping_algo,
                    std::string _link_mapping_algo)
        {
            algo_type = get_Embedding_Algorithm_Type_Str(_algo_t);
            node_mapping_algo = _node_mapping_algo;
            link_mapping_algo = _link_mapping_algo;
#ifdef ENABLE_MPI
            num_mpi_proc = MPI::COMM_WORLD.Get_size();
#endif
            sim =  new adevs::Simulator<typename GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::ADEVS_IO_TYPE> (_proc_digraph);
        };
         
        adevs::Simulator<typename GEN<VNR<Network<NODECLASS<NODERES...>, LINKCLASS<LINKRES...>>>>::ADEVS_IO_TYPE>* sim;
    };
}
#endif

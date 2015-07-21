/**
 * @file rng.cc
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 3, 2014
 *
 * @copyright Copyright (c) Jun 3, 2014                      SOROUSH HAERI
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
#include "rng.h"
#include "core/config-manager.h"

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

namespace vne
{
    //std::map<std::type_index, int> IdGenerator::m_map;
    std::shared_ptr<RNG> RNG::_instance = nullptr;
    std::shared_ptr<RNG> RNG::Instance()
    {
        if (_instance==nullptr)
        {
            _instance = std::shared_ptr<RNG> (new RNG(ConfigManager::Instance()->getConfig<unsigned long int>("core.rngSeed")));
        }
        return _instance;
    }

    void
    RNG::Destroy()
    {
    	if (_instance==nullptr)
    		return;
    	_instance.reset();
    	_instance = nullptr;
    	return;
    }

#ifdef ENABLE_MPI
    RNG::RNG(unsigned long int _seed)

    {
        useSameSeedForParallelRuns = ConfigManager::Instance()->getConfig<bool>("core.rngUseSameSeedForParallelRuns");
        if (useSameSeedForParallelRuns)
            seed = _seed;
        else
            seed  = _seed + MPI::COMM_WORLD.Get_rank();
#else
        RNG::RNG(unsigned long int _seed)
        : seed(_seed)
        {
#endif
        
        std::string type = ConfigManager::Instance()->getConfig<std::string>("core.rngType");

        if (type.compare("gsl_rng_mt19937")==0)
            rng_type = *gsl_rng_mt19937;
        else if (type.compare("gsl_rng_ranlxs0")==0)
            rng_type = *gsl_rng_ranlxs0;
        else if (type.compare("gsl_rng_ranlxs1")==0)
            rng_type = *gsl_rng_ranlxs1;
        else if (type.compare("gsl_rng_ranlxs2")==0)
            rng_type = *gsl_rng_ranlxs2;
        else if (type.compare("gsl_rng_ranlxd1")==0)
            rng_type = *gsl_rng_ranlxd1;
        else if (type.compare("gsl_rng_ranlxd2")==0)
            rng_type = *gsl_rng_ranlxd2;
        else if (type.compare("gsl_rng_mrg")==0)
            rng_type = *gsl_rng_mrg;
        else if (type.compare("gsl_rng_taus")==0)
            rng_type = *gsl_rng_taus;
        else if (type.compare("gsl_rng_taus2")==0)
            rng_type = *gsl_rng_taus2;
        else if (type.compare("gsl_rng_gfsr4")==0)
            rng_type = *gsl_rng_ranlux389;
        //default case
        else
            rng_type = *gsl_rng_ranlux;
        
        generalRNG = gsl_rng_alloc (&rng_type);
        gsl_rng_set(generalRNG, seed);
    }
    RNG::~RNG()
    {
        for (auto it = rng_map.begin(); it != rng_map.end(); it++)
        {
            gsl_rng_free(it->second);
            rng_map.erase(it);
        }
        gsl_rng_free(generalRNG);
    }
    const gsl_rng* RNG::getMyRNG (RNGSubscriber* objectPtr)
    {
        if (rng_map.find(objectPtr)==rng_map.end())
        {
            gsl_rng* r = gsl_rng_alloc (&rng_type);
            gsl_rng_set(r, seed);
            rng_map[objectPtr] = r;
            return r;
        }
        return rng_map[objectPtr];
    }
    const gsl_rng* RNG::getGeneralRNG()
    {
        return generalRNG;
    }
    void RNG::unsubscribe (RNGSubscriber* objectPtr)
    {
        auto it = rng_map.find(objectPtr);
        if (it==rng_map.end())
        {
            return;
        }
        gsl_rng_free(it->second);
        rng_map.erase(it);
        return;
    }
}

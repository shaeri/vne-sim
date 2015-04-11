/**
 * @file rng.h
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

#ifndef RNG_H_
#define RNG_H_

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <memory>
#include <map>
#include <limits.h>

#include "core/core-types.h"

namespace vne {
    struct RNGSubscriber {};
    class RNG {
        
    public:
        static std::shared_ptr<RNG> Instance ();
        static void Destroy ();

        const gsl_rng* getMyRNG (RNGSubscriber* objectPtr);
        const gsl_rng* getGeneralRNG ();
        void unsubscribe (RNGSubscriber* objectPtr);
        
        template<typename RetVAL, typename... PARAMS>
        RetVAL sampleDistribution (Distribution d, std::tuple<PARAMS...> p, const gsl_rng* r = nullptr)
        {
            if (r == nullptr)
            {
                r = getGeneralRNG ();
            }
            switch (d) {
                case Distribution::UNIFORM :
                    return gsl_ran_flat (r, std::get<0> (p), std::get<1> (p));
                case Distribution::EXPONENTIAL :
                    return gsl_ran_exponential (r, std::get<0> (p));
                case Distribution::POISSON :
                    return gsl_ran_poisson (r, std::get<0> (p));
                default:
                    return INT_MAX;
            }
        }
        ~RNG ();
    protected:
        RNG(unsigned long int _seed);
    private:
        static std::shared_ptr<RNG> _instance;
        
        std::map<RNGSubscriber*, gsl_rng*> rng_map;
        gsl_rng* generalRNG;
        gsl_rng_type rng_type;
        unsigned long int seed;
    };
}

#endif /* defined(__vne_mcts__rng__) */

/**
 * @file vy-vnr-release-proc.cc
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

#include "vy-vnr-release-proc.h"

namespace vne {
    namespace vineyard{
        
        template<>
        VYVNRReleaseProc<>::VYVNRReleaseProc ()
        : VNRReleaseProcessor<VYVirtualNetRequest<>>(),
        last_departure_time(0.0)
        {
        }
        template<>
        VYVNRReleaseProc<>::~VYVNRReleaseProc()
        {
        }
        template<>
        double VYVNRReleaseProc<>::ta()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: ta() "<< std::endl;
            if (vnr_queue.empty())
                return DBL_MAX;
            
            BOOST_LOG_TRIVIAL(info) << "---> Next departure at: " << vnr_queue.top()->getDepartureTime() << std::endl;
            BOOST_LOG_TRIVIAL(info) << "---> This departure is in : " << vnr_queue.top()->getDepartureTime()- time << std::endl;
            return vnr_queue.top()->getDepartureTime()-time;
        }
        template<>
        void VYVNRReleaseProc<>::delta_int()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: delta_int() "<< std::endl;
            time += ta ();
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: delta_int() time: "<< time << std::endl;
            vnr_queue.pop();
        }
        template<>
        void VYVNRReleaseProc<>::delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC:  delta_ext(). e:"<< e << std::endl;
            time += e;
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC:  delta_ext(). time:"<< time << std::endl;
            
            adevs::Bag<ADEVS_IO_TYPE>::const_iterator i = xb.begin();
            for (; i != xb.end(); i++)
            {
                //Copy the incoming Customer and place it at the back of the line.
                vnr_queue.push((*i).value);
            }
        }
        template<>
        void VYVNRReleaseProc<>::delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: delta_conf() "<< std::endl;
            delta_int();
            delta_ext(0.0,xb);
        }
        template<>
        void VYVNRReleaseProc<>::output_func(adevs::Bag<ADEVS_IO_TYPE>& yb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: output_func() "<< std::endl;
            //////////////////
            //The release logic should be added here.
            //////////////////
            PTR_TYPE leaving = vnr_queue.top ();
            last_departure_time = leaving->getDepartureTime();
            ADEVS_IO_TYPE y (depart, leaving);
            yb.insert(y);
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: output_func(): VNR leaving: ID: " << leaving->getId() << std::endl << "It was supposed to leave at: " << leaving->getDepartureTime() << std::endl <<
            "Its actual time of departure: " << time << std::endl;
        }
    }
}
/**
 * @file vy-vnr-proc-observer.h
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

#include "vy-vnr-proc-observer.h"
namespace vne {
    namespace vineyard{
        
        template<>
        VYVNRProcObserver<>::VYVNRProcObserver ()
        : VNRProcessObserver<VYVirtualNetRequest<>>  ()
        {
        }
        template<>
        VYVNRProcObserver<>::~VYVNRProcObserver()
        {
        }
        template<>
        double VYVNRProcObserver<>::ta()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: ta()" << std::endl;
            return DBL_MAX;
        }
        template<>
        void VYVNRProcObserver<>::delta_int()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: delta_int()" << std::endl;
        }
        template<>
        void VYVNRProcObserver<>::delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: delta_ext()" << std::endl;
            adevs::Bag<ADEVS_IO_TYPE>::const_iterator i = xb.begin();
            for (; i != xb.end (); i++)
            {
                if ((*i).port == entered_embedding_queue)
                {
                   BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< entered_embedding_queue << ". A VNR has entered embedding queue."  << std::endl;
                }
                else if ((*i).port == embedding_successful)
                {
                   BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< embedding_successful << ". A VNR has been successfully embedded."  << std::endl;
                }
                else if ((*i).port == embedding_unsuccessful)
                {
                   BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< embedding_unsuccessful << ". A VNR embedding was unsuccessful."  << std::endl;
                }
                else if ((*i).port ==released_resources)
                {
                   BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on port: "<< released_resources << ". A VNR left the substrate network."  << std::endl;
                }
                else
                {
                   BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: received an event on an unknown port: " << (*i).port << ". A VNR left the substrate network."  << std::endl;
                }
            }
        }
        template<>
        void VYVNRProcObserver<>::delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
           BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: delta_conf()" << std::endl;
        }
        template<>
        void VYVNRProcObserver<>::output_func(adevs::Bag<ADEVS_IO_TYPE>& yb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-PROC-OBSERVER: output_func()" << std::endl;
        }
    }
}
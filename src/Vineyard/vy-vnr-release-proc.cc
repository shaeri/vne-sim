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

namespace vne
{
namespace vineyard
{

    template <>
    VYVNRReleaseProc<>::VYVNRReleaseProc(
        std::shared_ptr<ReleaseAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>> releaseAlgo)
        : VNRReleaseProcessor<SUBSTRATE_TYPE, VNR_TYPE>(releaseAlgo), last_departure_time(0.0)
    {
    }
    template <>
    VYVNRReleaseProc<>::~VYVNRReleaseProc()
    {
    }
    template <>
    double VYVNRReleaseProc<>::ta()
    {
        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: ta() ";
        if (vnr_queue.empty())
            return DBL_MAX;

        BOOST_LOG_TRIVIAL(info) << "---> Next departure at: "
                                << vnr_queue.top()->getDepartureTime();
        BOOST_LOG_TRIVIAL(info) << "---> Time to next departure in : "
                                << vnr_queue.top()->getDepartureTime() - time;
        BOOST_LOG_TRIVIAL(info) << "---> Next departure VNR id : " << vnr_queue.top()->getId();
        BOOST_LOG_TRIVIAL(info) << "---> VNR queue size : " << vnr_queue.size();

        return vnr_queue.top()->getDepartureTime() - time;
    }
    template <>
    void VYVNRReleaseProc<>::delta_int()
    {
        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: delta_int() ";
        time = vnr_queue.top()->getDepartureTime();
        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: delta_int() time: " << time;
        vnr_queue.pop();
    }
    template <>
    void VYVNRReleaseProc<>::delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE> &xb)
    {
        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC:  delta_ext(). e:" << e;
        time += e;

        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC:  delta_ext(). time:" << time;

        adevs::Bag<ADEVS_IO_TYPE>::const_iterator i = xb.begin();

        for (; i != xb.end(); i++) {
            //Copy the embedded VNR and place it at the back of the queue.
            vnr_queue.push((*i).value);
        }
    }
    template <>
    void VYVNRReleaseProc<>::delta_conf(const adevs::Bag<ADEVS_IO_TYPE> &xb)
    {
        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: delta_conf() ";
        delta_int();
        delta_ext(0.0, xb);
    }
    template <>
    void VYVNRReleaseProc<>::output_func(adevs::Bag<ADEVS_IO_TYPE> &yb)
    {
        BOOST_LOG_TRIVIAL(info) << "VY-RELEASE-PROC: output_func() ";
        //////////////////
        //The release logic should be added here.
        //////////////////
        PTR_TYPE leaving = vnr_queue.top();
        this->releaseAlgorithm->releaseVNR(leaving);
        last_departure_time = leaving->getDepartureTime();
        ADEVS_IO_TYPE y(depart, leaving);
        yb.insert(y);
        BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: output_func(): VNR leaving: ID: "
                                << leaving->getId()
                                << "; It was supposed to leave at: " << leaving->getDepartureTime()
                                << "; its actual time of departure: " << time;
        //time = leaving->getDepartureTime();
    }
}  // namespace vineyard
}  // namespace vne
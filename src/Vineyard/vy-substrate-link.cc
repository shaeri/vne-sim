/**
 * @file vy-substrate-link.cc
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

#include "vy-substrate-link.h"
#include <fstream>

namespace vne
{
namespace vineyard
{
    template <>
    VYSubstrateLink<>::VYSubstrateLink(double _bw, double _delay, int _from, int _to)
        : SubstrateLink<double>(_bw, _from, _to), count(0), delay(_delay)
    {
    }
    template <>
    VYSubstrateLink<>::~VYSubstrateLink()
    {
    }
    /*
        template<>
        Embedding_Result VYSubstrateLink<>::embedLink(std::shared_ptr<VYVirtualLink<> > _l)
        {
            Embedding_Result r = SubstrateLink<double>::embedLink(_l);
            if (r== Embedding_Result::SUCCESSFUL_EMBEDDING)
                _l->addHostLink(this);
            return r;
        }
        template<>
        Embedding_Result VYSubstrateLink<>::embedLink(std::shared_ptr<VYVirtualLink<> > _l, std::shared_ptr<Resources<double>> _res)
        {
            return Embedding_Result::SUCCESSFUL_EMBEDDING;
        }
*/
    template <>
    int VYSubstrateLink<>::getCount() const
    {
        return count;
    }
    template <>
    double VYSubstrateLink<>::getBandwidth() const
    {
        return std::get<0>(this->resources);
    }
    template <>
    double VYSubstrateLink<>::getMaxBandwidth() const
    {
        return std::get<0>(this->getMaxResources());
    }
    template <>
    double VYSubstrateLink<>::getDelay() const
    {
        return delay;
    }
    template <>
    int VYSubstrateLink<>::operator++(int)
    {
        int val = count;
        count++;
        return val;
    }
    template <>
    int VYSubstrateLink<>::operator--(int)
    {
        int val = count;
        count--;
        return val;
    }
    template <>
    int VYSubstrateLink<>::operator++()
    {
        return ++count;
    }
    template <>
    int VYSubstrateLink<>::operator--()
    {
        return --count;
    }
    template <>
    void VYSubstrateLink<>::writeLinkToFile(std::ofstream &ofstrm)
    {
        if (ofstrm.is_open()) {
            ofstrm << node_from_local_id << " " << node_to_local_id << " " << getBandwidth() << " "
                   << getDelay() << std::endl;
        } else
            BOOST_LOG_TRIVIAL(error)
                << "VYSubstrateLink<>::writeLinkToFile: VYThe file is not open for writing. "
                << std::endl;
    }
}  // namespace vineyard
}  // namespace vne
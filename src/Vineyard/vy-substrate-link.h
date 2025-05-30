/**
 * @file vy-substrate-link.h
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

#ifndef VINEYARD_VY_SUBSTRATE_LINK_
#define VINEYARD_VY_SUBSTRATE_LINK_

#include "core/substrate-link.h"

namespace vne
{
namespace vineyard
{
    template <typename = double>
    class VYSubstrateLink : public SubstrateLink<double>
    {
       public:
        VYSubstrateLink(double _bw, double _delay, int _from, int _to);
        virtual ~VYSubstrateLink();
        double getBandwidth() const;
        double getMaxBandwidth() const;
        double getDelay() const;
        int getCount() const;
        int operator++(int);
        int operator--(int);
        int operator++();
        int operator--();
        virtual void writeLinkToFile(std::ofstream &ofstrm);

       private:
        int count;
        double delay;
    };
}  // namespace vineyard
}  // namespace vne
#endif /* defined(__vne_mcts__vy_substrate_link__) */

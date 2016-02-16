/**
 * @file vy-virtual-link.h
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

#ifndef VINEYARD_VY_VIRTUAL_LINK_
#define VINEYARD_VY_VIRTUAL_LINK_

#include "core/virtual-link.h"
#include "vy-substrate-link.h"

namespace vne {
    namespace vineyard {
        template<typename = double>
        class VYVirtualLink : public VirtualLink<double>
        {
        public:
            VYVirtualLink (double _bw, double _delay, int _from, int _to);
            virtual ~VYVirtualLink();
            virtual void addHostLink (SubstrateLink<double>* _l) override;
            virtual void addHostLink (SubstrateLink<double>* _l, std::shared_ptr<Resources<double>> _res) override;
            virtual void removeHostLink (SubstrateLink<double>* _l) override ;
            double getBandwidth () const;
            double getDelay () const;
            int getPathLength () const {return pathLength;};
            double getPathDelay () {return pathDelay;};
            virtual void writeLinkToFile (std::ofstream& ofstrm) override;
        private:
            int pathLength;
            double pathDelay;
            double delay;
        };
    }
}
#endif /* defined(__vne_mcts__vy_virtual_link__) */

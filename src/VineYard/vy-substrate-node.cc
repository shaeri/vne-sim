  /**
   * @file vy-substrate-node.cc
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

#include "vy-substrate-node.h"

namespace vne {
    namespace vineyard {
        template<>
        VYSubstrateNode<>::VYSubstrateNode (double cpu, const VYCoordinate& coord) :
            SubstrateNode<double>(cpu),
            coordinate(coord),
            touched(false),
            count(0)
        {
        }
        template<>
        VYSubstrateNode<>::VYSubstrateNode (double cpu, int _x, int _y) :
            SubstrateNode<double>(cpu),
            coordinate(VYCoordinate(_x, _y)),
            touched(false),
            count(0)
        {
        }
        template<>
        VYSubstrateNode<>::~VYSubstrateNode()
        {
        }
        template<>
        const VYCoordinate& VYSubstrateNode<>::getCoordinates() const
        {
            return coordinate;
        }
        template<>
        double VYSubstrateNode<>::getCPU() const
        {
            return std::get<0>(this->resources);
        }
        template<>
        int VYSubstrateNode<>::getCount() const
        {
            return count;
        }
        template<>
        int VYSubstrateNode<>::operator++(int)
        {
            int val = count;
            count++;
            return val;
        }
        template<>
        int VYSubstrateNode<>::operator--(int)
        {
            int val = count;
            count--;
            return val;
        }
        template<>
        int VYSubstrateNode<>::operator++()
        {
            return ++count;
        }
        template<>
        int VYSubstrateNode<>::operator--()
        {
            return --count;
        }
    }
}
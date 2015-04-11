/**
 * @file vy-virtual-node.cc
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

#include "vy-virtual-node.h"
#include <fstream>

namespace vne {
    namespace vineyard {
        template<>
        VYVirtualNode<>::VYVirtualNode (double cpu, const VYCoordinate& coord) :
        VirtualNode<double>(cpu,true),
        coordinate(coord)
        {
           this->id  = vne::IdGenerator::Instance()->getId<this_t>(this);
        }
        template<>
        VYVirtualNode<>::VYVirtualNode (double cpu, int _x, int _y) :
        VirtualNode<double>(cpu,true),
        coordinate(VYCoordinate(_x, _y))
        {
             this->id  = vne::IdGenerator::Instance()->getId<this_t>(this);
        }
        template<>
        VYVirtualNode<>::~VYVirtualNode()
        {
        }
        template<>
        const VYCoordinate& VYVirtualNode<>::getCoordinates() const
        {
            return coordinate;
        }
        template<>
        double VYVirtualNode<>::getCPU() const
        {
            return std::get<0>(this->resources);
        }
                template<>
        void VYVirtualNode<>::writeNodeToFile (std::ofstream& ofstrm)
        {
            if (ofstrm.is_open()) {
                ofstrm << getCoordinates().first << " " << getCoordinates().second << " " << getCPU() <<  std::endl;
            }
            else
                BOOST_LOG_TRIVIAL(error) << "VYVirtualNode<>::writeNodeToFile: VYThe file is not open for writing. " << std::endl;
        }
    }
}
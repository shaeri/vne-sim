  /**
   * @file vy-substrate-node.h
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

#ifndef VINEYARD_VY_SUBSTRATE_NODE_
#define VINEYARD_VY_SUBSTRATE_NODE_

#include "vy-coordinate.h"
#include "vy-virtual-net-request.h"

#include "core/substrate-node.h"
#include "core/network.h"

#include <vector>

namespace vne {
    namespace vineyard {
    template<typename = double>
    class VYSubstrateNode : public SubstrateNode<double>
    {
    public:
        VYSubstrateNode (double cpu, const VYCoordinate& coord);
        VYSubstrateNode (double cpu, int _x, int _y);
        ~VYSubstrateNode ();
        const VYCoordinate& getCoordinates () const;
        Embedding_Result embedNode(std::shared_ptr<VYVirtualNode<> > _n);
        virtual void writeNodeToFile (std::ofstream& ofstrm);
        double getCPU () const;
        double getMaxCPU () const;
        int getCount () const;
        int operator++ (int);
        int operator-- (int);
        int operator++ ();
        int operator-- ();
        bool touched;
        
    private:
        VYCoordinate coordinate;
        int count;
    };
    }
}
#endif /* defined(__VNE_MCTS__VY_SUBSTRATE_NODE__) */
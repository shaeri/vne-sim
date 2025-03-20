/**
 * @file vne-nm-state.h
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

#ifndef MCVNE_VNE_NM_STATE_
#define MCVNE_VNE_NM_STATE_

#include "mcts/state.h"

#include "core/network.h"
#include "core/virtual-network-request.h"

using namespace vne::mcts;

namespace vne
{
namespace mcvne
{

    class VNENMState : public State
    {
       public:
        VNENMState(std::shared_ptr<std::set<int>> _VNRNodeIdSet, int _vnrid,
                   const std::map<int, int> *mappedNodes);
        virtual ~VNENMState();

        virtual std::shared_ptr<State> getCopy() const override;

        void addNodeMapping(int sNodeId);
        const std::map<int, int> *getNodeMap() const;

        int getPreviousVNId();
        int getCurrentVNId() const;
        int getNextVNId();

        int getVNRId() const;

        std::shared_ptr<std::set<int>> getUsedSNIds();

        bool isTreminal() const;
        bool isStartState() const;

       protected:
        VNENMState();

       private:
        int vnrID;
        std::set<int>::const_iterator VNIdSetIterator;
        std::shared_ptr<std::set<int>> VNNodeIdSet;
        //keeps the valid choices for embedding of the current VN.
        //nodeMap<VirtualnodeId, SubstrateNodeId>;
        std::map<int, int> nodeMap;
    };
}  // namespace mcvne
}  // namespace vne
#endif
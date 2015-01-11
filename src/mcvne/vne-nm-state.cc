/**
 * @file vne-nm-state.cc
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

#include "vne-nm-state.h"

namespace vne {
    namespace mcvne {
        VNENMState::VNENMState (std::shared_ptr<std::set<int>> _idSet, int vnrid, const std::map<int,int>* mappedNodes) :
        VNNodeIdSet (_idSet),
        vnrID(vnrid)
        {
            if (mappedNodes->size() > 0)
            {
                for (auto it = mappedNodes->begin(); it != mappedNodes->end(); it++)
                {
                    auto it2 = VNNodeIdSet->find(it->first);
                    assert(it2 != VNNodeIdSet->end());
                    nodeMap.insert(std::make_pair(it->first,it->second));
                    VNNodeIdSet->erase(it2);
                }
            }
            VNIdSetIterator = VNNodeIdSet->begin();
        };
        
        VNENMState::VNENMState ()
        {
        };
        
        VNENMState::~VNENMState ()
        {
        };
        
        const std::map<int,int>*
        VNENMState::getNodeMap () const
        {
            return &nodeMap;
        };
        
        int
        VNENMState::getPreviousVNId ()
        {
            if (VNIdSetIterator == VNNodeIdSet->begin())
                return -1;
            VNIdSetIterator--;
            int nextId = *(VNIdSetIterator);
            VNIdSetIterator++;
            
            return nextId;
        };
        
        int
        VNENMState::getCurrentVNId () const
        {
            if (VNIdSetIterator == VNNodeIdSet->end())
                return -1;
            
            return *(VNIdSetIterator);
        };
        
        int
        VNENMState::getNextVNId ()
        {
            VNIdSetIterator++;
            if (VNIdSetIterator == VNNodeIdSet->end())
                return -1;
            int nextId = *(VNIdSetIterator);
            VNIdSetIterator--;
            return nextId;
        };
        
        int
        VNENMState::getVNRId() const
        {
            return vnrID;
        }
        
        bool
        VNENMState::isTreminal () const
        {
            return (VNIdSetIterator == VNNodeIdSet->end());
        };
        
        bool
        VNENMState::isStartState() const
        {
            return nodeMap.empty();
        }
        
        std::shared_ptr<State>
        VNENMState::getCopy() const
        {
            std::shared_ptr<VNENMState> newSt (new VNENMState ());
            *newSt = *this;
            return newSt;
        }
        
        
        std::shared_ptr<std::set<int>> VNENMState::getUsedSNIds ()
        {
            std::shared_ptr<std::set<int>> outSet (new std::set<int> ());
            for (auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
                outSet->insert(it->second);
            }
            return outSet;
        }
        
        void VNENMState::addNodeMapping(int sNodeId)
        {
            assert (nodeMap.find(*VNIdSetIterator) == nodeMap.end());
            nodeMap.insert(std::make_pair(*VNIdSetIterator, sNodeId));
            VNIdSetIterator++;
        }
    }
}

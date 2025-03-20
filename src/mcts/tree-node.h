/**
 * @file tree-node.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date 7/16/14
 *
 * @note This class is based on the POMCP implementation by David Silver and Joel Veness.
 * POMCP is published in NIPS 2010: "Online Monte-Carlo Plannin/g in Large POMDPs".
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

#ifndef MCTS_TREE_NODE_
#define MCTS_TREE_NODE_

#include <memory>
#include <vector>

#include "core/core-types.h"

#include <mcts/state.h>

namespace vne
{
namespace mcts
{
    template <class COUNT>
    class Value
    {
       public:
        void set(COUNT count, double value)
        {
            Count = count;
            Total = value * count;
            //Total = value;
            SumSquaredValue = count * value * value;
        }

        void add(double totalReward)
        {
            Count += 1.0;
            Total += totalReward;
            //if (Total == 0)
            //    Total = totalReward;
            //else if (totalReward > Total)
            //    Total = totalReward;
            SumSquaredValue += totalReward * totalReward;
        }

        void add(double totalReward, COUNT weight)
        {
            Count += weight;
            Total += totalReward * weight;
            SumSquaredValue += totalReward * totalReward * weight;
        }

        double getValue() const
        {
            return Count == 0 ? Total : Total / Count;
            //return Total;
        }

        double getSumSquaredValue() const { return SumSquaredValue; }

        COUNT getCount() const { return Count; }

       private:
        COUNT Count;
        double Total;
        long double SumSquaredValue;
    };

    class TreeNode
    {
       public:
        Value<int> value;
        Value<double> AMAF;

        void setChildren(int count, double value);
        std::shared_ptr<TreeNode> child(int c);

        const std::shared_ptr<State> getState() const;

        static int NumChildren;

        ~TreeNode();
        TreeNode(std::shared_ptr<State> st);

       protected:
        TreeNode();
        void Initialize();
        std::shared_ptr<State> state;
        std::vector<std::shared_ptr<TreeNode>> Children;
    };
}  // namespace mcts
}  // namespace vne
#endif
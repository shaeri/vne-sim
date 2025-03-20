/**
 * @file tree-node.cc
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
#include "tree-node.h"

#include <assert.h>
#include <iostream>

namespace vne
{
namespace mcts
{
    int TreeNode::NumChildren = 0;
    TreeNode::TreeNode(std::shared_ptr<State> st) : state(std::move(st))
    {
        assert(NumChildren);
        Children.resize(TreeNode::NumChildren);
        Initialize();
    }

    TreeNode::TreeNode() : state(nullptr)
    {
        assert(NumChildren);
        Children.resize(TreeNode::NumChildren);
    }

    void TreeNode::Initialize()
    {
        for (int action = 0; action < Children.size(); action++) {
            Children[action] = std::shared_ptr<TreeNode>(new TreeNode());
        }
    }

    TreeNode::~TreeNode() {}

    const std::shared_ptr<State> TreeNode::getState() const
    {
        return state;
    }

    void TreeNode::setChildren(int count, double value)
    {
        for (int action = 0; action < NumChildren; action++) {
            std::shared_ptr<TreeNode> node = Children[action];
            node->value.set(count, value);
            node->AMAF.set(count, value);
        }
    }
    std::shared_ptr<TreeNode> TreeNode::child(int c)
    {
        return Children[c];
    }
}  // namespace mcts
}  // namespace vne
/**
   * @file network-builder.h
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

#ifndef NETWORK_BUILDER_H_
#define NETWORK_BUILDER_H_
#include "core/network.h"

namespace vne
{
//template <typename ...> class NetworkBuilder;
//template<typename ... NODET, typename ... LINKT>
//class NetworkBuilder<Network<Node<NODET...>, Link <LINKT...>>>{
template <typename>
class NetworkBuilder;
template <typename... NodeT, template <typename...> class NodeC, typename... LinkT,
          template <typename...> class LinkC>
class NetworkBuilder<Network<NodeC<NodeT...>, LinkC<LinkT...>>>
{
   public:
    virtual std::shared_ptr<Network<NodeC<NodeT...>, LinkC<LinkT...>>> getNetwork() { return 0; };

   protected:
    NetworkBuilder() {};
};
}  // namespace vne
#endif
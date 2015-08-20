/**
 * @file external-lib-handler.h
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

#ifndef  NFG_EXTERNAL_LIB_HANDLER_
#define  NFG_EXTERNAL_LIB_HANDLER_


#include "core/network.h"
#include "core/config-manager.h"


namespace vne{
    namespace nfg {
      template<typename A, typename B>
      class ExternalLibHandler
      {
      public:
          const boost::property_tree::ptree& getProperties () {return pt;};
          virtual std::shared_ptr<Network<A, B>> getNetwork
            (Topology_Type tt, int n, Distribution cpu_dist, double cpu_param1, double cpu_param2, double cpu_param3,
                    Distribution bw_dist, double bw_param1, double bw_param2, double bw_param3,
                    Distribution delay_dist, double delay_param1, double delay_param2, double delay_param3) = 0;
          virtual std::string getPreferredFileName () = 0;
      protected:
          boost::property_tree::ptree pt;
          ExternalLibHandler () {};
      };
    }
}
#endif
/**
 * @file vy-substrate-network-builder.cc
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

#include "vy-substrate-network-builder.h"
#include "core/config-manager.h"

namespace vne {
    namespace vineyard {
        template<>
        VYSubstrateNetworkBuilder<>::VYSubstrateNetworkBuilder ()
        {
            _net = 0;
        }
        template<>
        std::shared_ptr<Network<VYSubstrateNode<>, VYSubstrateLink<>>>
        VYSubstrateNetworkBuilder<>::getNetwork ()
        {
            if (_net != 0)
                return _net;
            //get generate the path to the substrate network file
            std::stringstream snfilePath;
            snfilePath << ConfigManager::Instance()->getConfig<std::string>("vineyard", "SubstrateNetwork", "path") <<
            ("/") << (ConfigManager::Instance()->getConfig<std::string>("vineyard", "SubstrateNetwork", "filename"));
            
            //open the file
            std::ifstream snFile;
            snFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
            try {
                //open the substrate network file
                snFile.open(snfilePath.str().c_str(), std::ifstream::in);
                _net = std::make_shared<Network<VYSubstrateNode<>, VYSubstrateLink<>>>
                (Network<VYSubstrateNode<>, VYSubstrateLink<>>());
                
                //Read the first line that includes the number of nodes and edges in the substrate network
                std::string line;
                getline(snFile,line);
                std::istringstream iss (line);
                
                int numNodes, numEdges;
                iss >> numNodes >> numEdges;
                int nodeId_offset = IdGenerator::Instance()->peekId<SubstrateNode<double>>();
                //create the nodes;
                for (int i = 0; i< numNodes; i++)
                {
                    int x, y;
                    double cpu;
                    getline(snFile,line);
                    std::istringstream iss (line);
                    iss >> x >> y >> cpu;
                    BOOST_LOG_TRIVIAL (debug) << x << " " << y << " " << cpu << std::endl;
                    std::shared_ptr<VYSubstrateNode<>> n (new VYSubstrateNode<> (cpu,x,y));
                    _net->addNode(n);
                }
                for (int i = 0; i < numEdges; i++)
                {
                    int from, to;
                    double bw, delay;
                    getline(snFile,line);
                    std::istringstream iss (line);
                    iss >> from >> to >> bw >> delay;
                    BOOST_LOG_TRIVIAL(debug) << from << " " << to << " " << bw << " " << delay << std::endl;
                    std::shared_ptr<VYSubstrateLink<>> l (new VYSubstrateLink<> (bw,delay,from+nodeId_offset,to+nodeId_offset));
                    _net->addLink(l);
                }
                snFile.close();
                return _net;
            } catch (std::ifstream::failure& e) {
                //throw e;
                
                std::cerr << ">>>>Exception in reading substrate network creation file.<<< \n" <<
                "File: " << ConfigManager::Instance()->getConfig<std::string>("vineyard", "SubstrateNetwork", "filename")
                << " does not exist in: " <<
                ConfigManager::Instance()->getConfig<std::string>("vineyard", "SubstrateNetwork", "path")
                << std::endl;
                throw e;
            }
        }
    }
}
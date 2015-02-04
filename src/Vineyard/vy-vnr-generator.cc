/**
 * @file vy-vnr-generator.cc
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
#include "vy-vnr-generator.h"

#include <regex>

namespace vne {
    namespace vineyard {
        
        template<>
        inline void VYVNRGenerator<>::setVNR ()
        {
            std::ifstream vnFile;
            vnFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
            try {
                //open the substrate network file
                //BOOST_LOG_TRIVIAL(debug) << "opening: " << _reqFiles.top().second.string().c_str() << std::endl;
                vnFile.open(_reqFiles.top().second.string().c_str(), std::ifstream::in);
                std::shared_ptr<Network<VYVirtualNode<>, VYVirtualLink<>>> vn
                (new Network<VYVirtualNode<>, VYVirtualLink<>>());
                
                //we need to have the offset of the node ids because the VirtualNode Ids will be generated
                //in increasing order irrespective of the number of request.
                int nodeID_offset = IdGenerator::Instance()->peekId<VirtualNode<double>>();
                
                //Read the first line that includes number of nodes, number of edges, split,
                //arrival time, duration, topology, and maximum distance
                std::string line;
                getline(vnFile,line);
                std::istringstream iss (line);
                
                int numNodes, numEdges, split, topo, maxd;
                double time, duration;
                iss >> numNodes >> numEdges >> split >> time >> duration >> topo >> maxd;
                
                //create the nodes;
                for (int i = 0; i< numNodes; i++)
                {
                    int x, y;
                    double cpu;
                    getline(vnFile,line);
                    std::istringstream iss (line);
                    iss >> x >> y >> cpu;
                    //std::cout<< x << " " << y << " " << cpu << std::endl;
                    std::shared_ptr<VYVirtualNode<>> n (new VYVirtualNode<> (cpu,x,y));
                    vn->addNode(n);
                }
                for (int i = 0; i < numEdges; i++)
                {
                    int from, to;
                    double bw, delay;
                    getline(vnFile,line);
                    std::istringstream iss (line);
                    iss >> from >> to >> bw >> delay;
                    //std::cout<< from << " " << to << " " << bw << " " << delay << std::endl;
                    std::shared_ptr<VYVirtualLink<>> l (new VYVirtualLink<> (bw,delay,from+nodeID_offset,to+nodeID_offset));
                    vn->addLink(l);
                }
                //close the request file
                vnFile.close();
                //There is a problem with make_shared (using libc++) it first creates the object and then copies the object and then destroys the
                //original object. As a result the address of this pointer in the constructor and other function calls are different.
                //vnr = std::make_shared<VYVirtualNetRequest<>>(VYVirtualNetRequest<>(vn,time,duration,split,topo,maxd,revenue,cost));
                vnr = std::shared_ptr<VYVirtualNetRequest<>> (new VYVirtualNetRequest<>(vn,time,duration,split,topo,maxd,revenue,cost));
            }
            catch (std::ifstream::failure& e) {
                std::cerr << ">>>>Exception in reading a request file.<<< \n" <<
                "File: " << _reqFiles.top().second.string().c_str()
                << " does not exist. ";
                // << ConfigManager::Instance()->getConfig<std::string>("vineyard.SubstrateNetwork.path")
                //<< std::endl;
                throw e;
            }
        }
        
        template<> VYVNRGenerator<>::~VYVNRGenerator ()
        {
        }
        
        template<>
        VYVNRGenerator<>::VYVNRGenerator (
            std::function<std::shared_ptr<std::pair<double,double>> (const VYVirtualNetRequest<>* vnr)> calcRevenue,
            std::function<std::shared_ptr<std::pair<double,double>> (const VYVirtualNetRequest<>* vnr)> calcCost
                                          ) :
        VNRGenerator<VYVirtualNetRequest<> >(),
        revenue(calcRevenue),
        cost(calcCost),
        last_vnr_arrival(0.0)
        {
            vnr = nullptr;
            
            std::stringstream vnDirPath;
            vnDirPath << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.path") <<
            ("/") << (ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.dir"));
            const std::string fileExtension = ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetRequest.reqfileExtension");
            path p (vnDirPath.str());
            
            directory_iterator end_itr;
            
            try
            {
                if ( exists(p) && is_directory(p))
                {
                    for( directory_iterator dir_iter(p) ; dir_iter != end_itr ; ++dir_iter)
                    {
                        if (is_regular_file(dir_iter->status()) && extension(dir_iter->path()).compare(fileExtension)==0)
                        {
                            std::regex rgx ("/*([0-9]+)\\.txt");
                            std::smatch match;
                            if (std::regex_search(dir_iter->path().string().begin(), dir_iter->path().string().end(), match, rgx))
                            {
                                _reqFiles.push(std::make_pair(atoi(match[1].str().c_str()), dir_iter->path()));
                            }
                            else
                            {
                                std::cerr << "File could not be processed (bad name string): "
                                << dir_iter->path().string().substr(vnDirPath.str().size(),dir_iter->path().string().size());
                                
                            }
                        }
                    }
                    if (_reqFiles.size()==0)
                    {
                        std::cerr <<"There are no matching request files. "
                        << "The request files need to be numbered and have the extension: "
                        << fileExtension << std::endl;
                        throw;
                    }
                }
                else
                {
                    std::cerr << ">>>>Exception in reading virtual network request directory.<<< \n" <<
                    "Directory: " << ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetwork.dir")
                    << " does not exist in: " <<
                    ConfigManager::Instance()->getConfig<std::string>("vineyard.VirtualNetwork.path");
                    throw;
                }
            }
            catch (...)
            {
            }
        }
        template<>
        void VYVNRGenerator<>::output_func(adevs::Bag<ADEVS_IO_TYPE> &yb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-VNR-GEN: output_func()" << std::endl;
            if (vnr == nullptr)
            {
                setVNR();
                if (vnr == nullptr)
                {
                    std::cerr << "Unable to generate new VNRs." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            ADEVS_IO_TYPE adevs_vnr (arrive,vnr);
            yb.insert(adevs_vnr);
            //BOOST_LOG_TRIVIAL(info) << "VY-VNR-GEN: leaving output_func(). Bag size: "<< yb.size() << std::endl;
        }
        
        template<>
        void VYVNRGenerator<>::delta_int()
        {
            //remove the current last element from the req_files_queue
            _reqFiles.pop();
            BOOST_LOG_TRIVIAL(info) << "VY-VNR-GEN: delta_int()" << std::endl;
            if(vnr != nullptr)
            {
                last_vnr_arrival = vnr->getArrivalTime();
                vnr.reset();
            }
        }
        
        template<>
        double VYVNRGenerator<>::ta ()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-VNR-GEN: ta ()" << std::endl;
            if (_reqFiles.size()==0)
            {
                return DBL_MAX;
            }
            if (vnr == nullptr) setVNR();
            BOOST_LOG_TRIVIAL(info) << "---> Next arrival at: " << vnr->getArrivalTime() << std::endl;
            BOOST_LOG_TRIVIAL(info) << "---> Next arrival is in : " << vnr->getArrivalTime()-last_vnr_arrival << std::endl;
            return vnr->getArrivalTime()-last_vnr_arrival;
        }
    }
}

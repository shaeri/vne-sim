/**
 * @file vy-vnr-embedding-proc.cc
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

#include "vy-vnr-embedding-proc.h"

namespace vne {
    namespace vineyard{
        
        template<>
        VYVNREmbeddingProc<>::VYVNREmbeddingProc
                        (std::shared_ptr<EmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>>embeddingAlgo)
        : VNREmbeddingProcessor<SUBSTRATE_TYPE,VNR_TYPE>(embeddingAlgo)
        {
        }
        template<>
        VYVNREmbeddingProc<>::~VYVNREmbeddingProc()
        {
        }
        template<>
        double VYVNREmbeddingProc<>::ta()
        {
            /*if (!vnr_queue.empty() && !vnr_release_queue.empty())
                return  std::min (vnr_embedding_queue.top()->getArrivalTime(), vnr_release_queue.top()->getDepartureTime());
            else if (vnr_embedding_queue.empty() && !vnr_release_queue.empty())
                return vnr_release_queue.top()-> getDepartureTime();
            else if (!vnr_embedding_queue.empty() && vnr_release_queue.empty())
                return vnr_embedding_queue.top()->getArrivalTime();
            else return DBL_MAX;*/
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: ta (): "  << std::endl;
            if (vnr_queue.empty())
                return DBL_MAX;
            
            BOOST_LOG_TRIVIAL(info) << "VNR ID: " << vnr_queue.top()->getId() << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Current time: " << time << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Processing time: " << vnr_queue.top()->getProccessingTime() << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Arrival time: " << vnr_queue.top()->getArrivalTime() << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Queuing Time: " << vnr_queue.top()->getQueuingTime() << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Ta Returns: " << (vnr_queue.top()->getArrivalTime() + vnr_queue.top()->getProccessingTime() + vnr_queue.top()->getQueuingTime())-time << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Processing queue size: " << vnr_queue.size() << std::endl;
            
            return (vnr_queue.top()->getArrivalTime() + vnr_queue.top()->getProccessingTime() + vnr_queue.top()->getQueuingTime())-time;
        }
        
        template<>
        void VYVNREmbeddingProc<>::delta_int()
        {
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: delta_int()" << std::endl;
            BOOST_LOG_TRIVIAL(info) << "Time advance: " << (vnr_queue.top()->getArrivalTime() + vnr_queue.top()->getProccessingTime()+ vnr_queue.top()->getQueuingTime())-time<< std::endl;
            time += (vnr_queue.top()->getArrivalTime() + vnr_queue.top()->getProccessingTime()+ vnr_queue.top()->getQueuingTime())-time;
            double queue_time = 0.0;
            double prev_arriv_t = 0.0;
            if (vnr_queue.size()>1)
            {
                queue_time = vnr_queue.top()->getQueuingTime() + vnr_queue.top()->getProccessingTime();
                prev_arriv_t = vnr_queue.top()->getArrivalTime();
                vnr_queue.pop();
                vnr_queue.top()->setQueuingTime(queue_time - (vnr_queue.top()->getArrivalTime() - prev_arriv_t));
                assert(vnr_queue.top()->getQueuingTime()>0);
            }
            else
                vnr_queue.pop();
            
            BOOST_LOG_TRIVIAL(info) << "VNR ID: " << vnr_queue.top()->getId()<< std::endl;
            BOOST_LOG_TRIVIAL(info) << "VNR Queuing Time: " << vnr_queue.top()->getQueuingTime() << std::endl;
        }
        
        template<>
        void VYVNREmbeddingProc<>::delta_ext(double e, const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC:  delta_ext(). e:"<< e << std::endl;
            time += e;
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC:  delta_ext(). time:"<< time << std::endl;
            if (vnr_queue.size()>0)
            {
            }
            adevs::Bag<ADEVS_IO_TYPE>::const_iterator i = xb.begin();
            for (; i != xb.end(); i++)
            {
                //statrt the chrono
                start = std::chrono::system_clock::now();
                this->embeddingAlgorithm->embeddVNR((*i).value);
                //end the chrono
                end = std::chrono::system_clock::now();
                elapsed_seconds = end - start;
                //Would be intersting to see various processing time distributions.
               //((*i).value)->setProccessingTime(elapsed_seconds.count());
                ((*i).value)->setProccessingTime(1);
                // Copy the incoming vnr and place it at the back of the line.
                vnr_queue.push((*i).value);
            }
        }
        template<>
        void VYVNREmbeddingProc<>::delta_conf(const adevs::Bag<ADEVS_IO_TYPE>& xb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: delta_conf()" << std::endl;
            delta_int();
            delta_ext(0.0,xb);
        }
        template<>
        void VYVNREmbeddingProc<>::output_func(adevs::Bag<ADEVS_IO_TYPE>& yb)
        {
            BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: output_func()" << std::endl;
            PTR_TYPE leaving = vnr_queue.top ();
            ADEVS_IO_TYPE y;
            if (leaving->getVN()->getNumLinks() == leaving->getLinkMap()->size() &&
                    leaving->getVN()->getNumNodes() == leaving->getNodeMap()->size())
            {
                y = ADEVS_IO_TYPE (depart_successful_embedding,leaving);
                BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: output_func(): VNR leaving: Successful Mapping " <<  std::endl;
            }
            else
            {
                y = ADEVS_IO_TYPE (depart_unsuccessful_embedding,leaving);
                BOOST_LOG_TRIVIAL(info) << "VY-EMBEDDING-PROC: output_func(): VNR leaving: Failed Mapping " <<  std::endl;
            }
            yb.insert(y);
        }
    }
}
/**
 * @file vy-statistics.h
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

#ifndef VINEYARD_VY_STATISTICS_
#define VINEYARD_VY_STATISTICS_

#include "core/statistics-subscriber.h"

#include "hiberlite/hiberlite.h"

namespace vne {
    namespace vineyard {
        class VYStatistics : public Statistics
        {
            friend class hiberlite::access;
            template<class Archive>
            void hibernate(Archive & ar)
            {
                ar & HIBERLITE_BASE_CLASS(Statistics);
                ar & HIBERLITE_NVP(event_type);
                ar & HIBERLITE_NVP(vnr_arrival_time);
                ar & HIBERLITE_NVP(vnr_duration);
                ar & HIBERLITE_NVP(vnr_id);
                ar & HIBERLITE_NVP(avg_node_stress);
                ar & HIBERLITE_NVP(max_node_stress);
                ar & HIBERLITE_NVP(std_dev_node_stress);
                ar & HIBERLITE_NVP(avg_link_stress);
                ar & HIBERLITE_NVP(max_link_stress);
                ar & HIBERLITE_NVP(std_dev_link_stress);
                ar & HIBERLITE_NVP(link_mapping_objective_val);
                ar & HIBERLITE_NVP(node_mapping_objective_val);
                ar & HIBERLITE_NVP(processing_time);
                ar & HIBERLITE_NVP(actual_processing_time);
                ar & HIBERLITE_NVP(node_revenue);
                ar & HIBERLITE_NVP(link_revenue);
                ar & HIBERLITE_NVP(node_cost);
                ar & HIBERLITE_NVP(link_cost);
            }
        public:
            std::string event_type;
            int vnr_id = 0;
            double vnr_arrival_time;
            double vnr_duration;
            double avg_node_stress = 0.0;
            double max_node_stress = 0.0;
            double std_dev_node_stress = 0.0;
            double avg_link_stress = 0.0;
            double max_link_stress = 0.0;
            double std_dev_link_stress = 0.0;
            double link_mapping_objective_val = 0.0;
            double node_mapping_objective_val = 0.0;
            double processing_time = 0.0;
            double actual_processing_time = 0.0;
            double node_revenue = 0.0;
            double link_revenue = 0.0;
            double total_revenue = 0.0;
            double link_cost = 0.0;
            double node_cost = 0.0;
            double total_cost  = 0.0;
        };
    }
}
#endif /* defined(__vne_mcts__vy_statistics__) */

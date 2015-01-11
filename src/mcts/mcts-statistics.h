/**
 * @file mcts-statistics.h
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

#ifndef MCTS_MCTS_STATISTICS_
#define MCTS_MCTS_STATISTICS_

#include <math.h>
#include <assert.h>
#include <iostream>

#include "core/core-types.h"

namespace vne{
    namespace mcts{
        class MCTSStatistics {
        public:
            
            MCTSStatistics();
            MCTSStatistics(double val, int count);
            
            void Add(double val);
            void Clear();
            int GetCount() const;
            void Initialise(double val, int count);
            double GetTotal() const;
            double GetMean() const;
            double GetVariance() const;
            double GetStdDev() const;
            double GetStdErr() const;
            double GetMax() const;
            double GetMin() const;
            void Print(const std::string& name, std::ostream& ostr) const;
            
        private:
            
            int Count;
            double Mean;
            double Variance;
            double Min, Max;
            
        };
        
        inline MCTSStatistics::MCTSStatistics()
        {
            Clear();
        }
        
        inline MCTSStatistics::MCTSStatistics(double val, int count)
        {
            Initialise(val, count);
        }
        
        inline void MCTSStatistics::Add(double val)
        {
            double meanOld = Mean;
            int countOld = Count;
            ++Count;
            assert(Count > 0); // overflow
            Mean += (val - Mean) / Count;
            Variance = (countOld * (Variance + meanOld * meanOld)
                        + val * val) / Count - Mean * Mean;
            if (val > Max)
                Max = val;
            if (val < Min)
                Min = val;
        }
        
        inline void MCTSStatistics::Clear()
        {
            Count = 0;
            Mean = 0;
            Variance = 0;
            Min = +Infinity;
            Max = -Infinity;
        }
        
        inline int MCTSStatistics::GetCount() const
        {
            return Count;
        }
        
        inline void MCTSStatistics::Initialise(double val, int count)
        {
            Count = count;
            Mean = val;
        }
        
        inline double MCTSStatistics::GetTotal() const
        {
            return Mean * Count;
        }
        
        inline double MCTSStatistics::GetMean() const
        {
            return Mean;
        }
        
        inline double MCTSStatistics::GetStdDev() const
        {
            return sqrt(Variance);
        }
        
        inline double MCTSStatistics::GetStdErr() const
        {
            return sqrt(Variance / Count);
        }
        
        inline double MCTSStatistics::GetMax() const
        {
            return Max;
        }
        
        inline double MCTSStatistics::GetMin() const
        {
            return Min;
        }
        
        inline void MCTSStatistics::Print(const std::string& name, std::ostream& ostr) const
        {
            ostr << name << ": " << Mean << " [" << Min << ", " << Max << "]" << std::endl;
        }
    }
}
#endif

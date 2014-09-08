  /**
   * @file coordinate.h
   * @author Soroush Haeri <soroosh.haeri@me.com>
   * @date 7/22/14
   * 
   * @copyright Copyright (c) 7/22/14                      SOROUSH HAERI
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

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <utility>
#include <math.h>

namespace vne{
    template <typename T1, typename T2>
    class Coordinate2D : public std::pair<T1,T2>
    {
    public:
        Coordinate2D (T1 _1, T2 _2);
        virtual inline double distanceFrom (const std::pair<T1,T2>& point) const = 0;
        virtual ~Coordinate2D ();
    };
    template <typename T1, typename T2>
    Coordinate2D<T1, T2>::Coordinate2D (T1 _1, T2 _2)
        : std::pair<T1, T2> (_1,_2)
    {
    }
    template <typename T1, typename T2>
    Coordinate2D<T1, T2>::~Coordinate2D ()
    {
    }
    
    template<typename T1, typename T2>
    class CartesianCoord2D : public Coordinate2D<T1,T2>
    {
    public:
        CartesianCoord2D (T1 _1, T2 _2);
        virtual inline double distanceFrom (const std::pair<T1,T2>& point) const;
    };
    template<typename T1, typename T2>
    CartesianCoord2D<T1, T2>::CartesianCoord2D (T1 _1, T2 _2)
    :   Coordinate2D<T1,T2>::Coordinate2D (_1, _2)
    {
    }
    template<typename T1, typename T2>
    inline double CartesianCoord2D<T1,T2>::distanceFrom(const std::pair<T1,T2>& point) const
    {
        return sqrt (pow((this->first - point.first), 2) + pow ((this->second - point.second),2));
    }
}
#endif /* defined(__vne_mcts__coordinate__) */

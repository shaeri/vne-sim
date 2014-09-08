  /**
   * @file resources.h
   * @author Soroush Haeri <soroosh.haeri@me.com>
   * @date 7/2/14
   * 
   * @copyright Copyright (c) 7/2/14                      SOROUSH HAERI
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

#ifndef RESOURCES_H_
#define RESOURCES_H_
#define BOOST_LOG_DYN_LINK

#include "core/core-types.h"

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

namespace vne {

template<typename... RES>
    class Resources : public std::tuple<RES...>
{
public:
    Resources ();
    Resources (RES ... _res);
    virtual ~Resources ();
    bool hasResources(const std::tuple<RES...>& _res);
    bool hasResources(const RES &... _res);
	Embedding_Result embedResources(const std::tuple<RES...>&  _res);
    void freeResources(const std::tuple<RES...>& _res);
private:
    template<std::size_t> struct int_
	{
	};
    
	template<size_t Pos>
	bool hasResources(const std::tuple<RES...>& t, int_<Pos>, bool previousResult)
	{
		bool retVal;
		if (std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(t)>
            std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(*this))
		{
			retVal = previousResult && false;
		}
		else
			retVal = previousResult && true;
		return hasResources(t, int_<Pos - 1>(), retVal);
	}
    
	bool hasResources(const std::tuple<RES...>& t, int_<1>, bool previousResult = true)
	{
		bool retVal;
		if (std::get<std::tuple_size<std::tuple<RES...>>::value - 1>(t)>
            std::get<std::tuple_size<std::tuple<RES...>>::value - 1>(*this))
		{
			retVal = false;
		}
		else
			retVal = true;
		return retVal;
	}
    
	template<size_t Pos>
	void embedResources(const std::tuple<RES...>& t, int_<Pos>)
	{
		auto val =
        std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(*this);
		std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(*this)
        = val - std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(t);
		embedResources(t, int_<Pos - 1>());
	}
    
	void embedResources(const std::tuple<RES...>& t, int_<1>)
	{
		auto val = std::get<std::tuple_size<std::tuple<RES...>>::value - 1> (*this);
		std::get<std::tuple_size<std::tuple<RES...>>::value - 1> (*this)
        = val - std::get<std::tuple_size<std::tuple<RES...>>::value - 1>(t);
	}
	template<size_t Pos>
	void freeResources(const std::tuple<RES...>& t, int_<Pos>)
	{
		auto val = std::get<std::tuple_size<std::tuple<RES...>>::value - Pos> (*this);
		std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(*this)
        = val + std::get<std::tuple_size<std::tuple<RES...>>::value - Pos>(t);
		freeResources(t, int_<Pos - 1>());
	}
    
	void freeResources(const std::tuple<RES...>& t, int_<1>)
	{
		auto val = std::get<std::tuple_size<std::tuple<RES...>>::value - 1> (*this);
		std::get<std::tuple_size<std::tuple<RES...>>::value - 1>(*this) = val
        + std::get<std::tuple_size<std::tuple<RES...>>::value - 1>(t);
	}
};
template<typename... RES>
Resources<RES...>::Resources ()
    : std::tuple<RES...> ()
{
}
template<typename ... RES>
Resources<RES...>::Resources(RES ... _res)
    : std::tuple<RES...> (_res...)
{
}
template<typename... RES>
Resources<RES...>::~Resources()
{
}
template<typename ... RES>
bool Resources<RES...>::hasResources(const std::tuple<RES...>& _res)
{
	return hasResources(_res, int_<sizeof...(RES)>(), true);
}
template<typename ... RES>
bool Resources<RES...>::hasResources(const RES &... _res)
{
	return hasResources(Resources<RES...>(_res...), int_<sizeof...(RES)>(), true);
}
template<typename ... RES>
Embedding_Result Resources<RES...>::embedResources(const std::tuple<RES...>&  _res)
{
	BOOST_LOG_NAMED_SCOPE("Resources::embedResources");
	if(!hasResources(_res))
	{
		return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
	}
	embedResources(_res, int_<sizeof...(RES)>());
	return Embedding_Result::SUCCESSFUL_EMBEDDING;
}
template<typename ... RES>
void Resources<RES...>::freeResources(const std::tuple<RES...>& _res)
{
	freeResources(_res, int_<sizeof...(RES)>());
}
}
#endif
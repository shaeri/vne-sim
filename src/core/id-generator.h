/**
 * @file network.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 3, 2014
 *
 * @copyright Copyright (c) Jun 3, 2014                      SOROUSH HAERI
 *      All Rights Reserved
 *
 *      Permission to use, copy, modify, and distribute this software and its
 *      documentation for any purpose and without fee is hereby granted, provided
 *      that the above copyright notice appear in all copies and that both that
 *      copyright notice and this permission notice appear in supporting
 *      documentation, and that the name of the author not be used in advertising or
 *      publicity pertaining to distribution of the software without specific,
 *      written prior permission.
 *
 *      THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *      ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *      AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *      DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *      AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *      OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef ID_GENERATOR_H_
#define ID_GENERATOR_H_

#include <typeinfo>
#include <typeindex>
#include <map>

namespace vne
{
class IdGenerator
{
public:
	IdGenerator();
	virtual ~IdGenerator();
	template<typename T>
	static int getId(T *obj);
	template<typename T>
	static int peekId();
	//template<typename T>
	//static void resetTypeCounter(T obj);
	//static void resetAllCounters();
private:
    static std::map<std::type_index, int> m_map;
};
/*
 IdGenerator::IdGenerator ()
 {
 }
 IdGenerator::~IdGenerator ()
 {
 }*/
template<typename T>
int IdGenerator::getId(T *obj)
{
	int ret;
	//const char* type_name = typeid(*obj).name();
	auto it = m_map.find(typeid(*obj));
	if (it == m_map.end())
	{
		m_map[typeid(*obj)] = 0;
		ret = 0;
	}
	else
	{
		ret = ++it->second;
	}
	return ret;
}
template<typename T>
int IdGenerator::peekId()
{
	int ret;
	//const char* type_name = typeid(T).name();
	auto it = m_map.find(typeid(T));
	if (it == m_map.end())
	{
		ret = 0;
	}
	else
	{
		ret = it->second + 1;
	}
	return ret;
}
}
#endif

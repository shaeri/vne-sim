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
#ifndef LINK_H_
#define LINK_H_

#include <tuple>
#include <memory>

#include "core-types.h"
#include "id-generator.h"
#include "core/resources.h"

namespace vne
{
template<typename ... Args>
class Link
{
    
template<typename, typename> friend class Network;
    
public:
	Link(const Resources<Args...>& _res, const Entity_t& t, int node_from, int node_to);
    Link(const Args &... _args, const Entity_t& t, int node_from, int node_to);
	virtual ~Link();
	const int& getId() const;
	const int& getNodeFromId () const;
	const int& getNodeToId () const;
    const int& getNodeFromLocalId () const {return node_from_local_id;};
    const int& getNodeToLocalId () const {return node_to_local_id;};
	const Entity_t& getType() const;
    const Resources<Args...>& getResources () const;
    virtual void writeLinkToFile (std::ofstream& ofstrm) {};
	/*
	 * I do not think these functions are necessary
	 */
	//std::tuple<Args...> getResources();
	//void setResources(std::tuple<Args...>);
private:
	typedef Link<Args...> this_t;
protected:
    Link(const Resources<Args...>& _res, const Entity_t& t, int node_from, int node_to, bool noid);
    Link(const Args &... _args, const Entity_t& t, int node_from, int node_to, bool noid);
	int id;
	Entity_t type;
    Resources<Args...> resources;
	int node_from_id;
	int node_to_id;
    int node_from_local_id;
    int node_to_local_id;
};
template<typename ... Args>
        Link<Args...>::Link(const Resources<Args...>& _res, const Entity_t& t, int node_from, int node_to) :
		id(IdGenerator::Instance()->getId<this_t>(this)),
        resources(_res),
		type(t),
		node_from_id(node_from),
		node_to_id(node_to)
{
}
template<typename ... Args>
        Link<Args...>::Link(const Args &... _args, const Entity_t& t, int node_from, int node_to) :
		id(IdGenerator::Instance()->getId<this_t>(this)),
        resources(Resources<Args...> (_args...)),
		type(t),
		node_from_id(node_from),
		node_to_id(node_to)
{
}
template<typename ... Args>
        Link<Args...>::Link(const Resources<Args...>& _res, const Entity_t& t, int node_from, int node_to, bool noid) :
        resources(_res),
		type(t),
		node_from_id(node_from),
		node_to_id(node_to)
{
}
template<typename ... Args>
        Link<Args...>::Link(const Args &... _args, const Entity_t& t, int node_from, int node_to, bool noid) :
        resources(Resources<Args...> (_args...)),
		type(t),
		node_from_id(node_from),
		node_to_id(node_to)
{
}
template<typename ... Args>
Link<Args...>::~Link()
{
}
template<typename ... Args>
const Entity_t& Link<Args...>::getType() const
{
	return type;
}
template<typename ... Args>
const int& Link<Args...>::getId() const
{
	return id;
}
template<typename ... Args>
const int& Link<Args...>::getNodeFromId() const
{
	return node_from_id;
}
template<typename ... Args>
const int& Link<Args...>::getNodeToId() const
{
	return node_to_id;
}
template<typename ... Args>
const Resources<Args...>& Link<Args...>::getResources() const
{
	return resources;
}
}
#endif
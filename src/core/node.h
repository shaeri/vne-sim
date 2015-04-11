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
#ifndef NODE_H_
#define NODE_H_

#include "core-types.h"
#include "id-generator.h"
#include "core/resources.h"

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <tuple>
#include <map>

namespace vne
{
template<typename ... Args>
class Node 
{
template<typename, typename> friend class Network;
    
public:
	//Node();
	//Node(Entity_t t);
    Node(const Resources<Args...>& _res, const Entity_t& t);
    Node(const Args &... _args, const Entity_t& t);
    virtual ~Node();
    int getId() const;
    int getLocalId() const {return local_id;};
    const Entity_t& getType() const;
    const Resources<Args...>& getResources() const;
    virtual void writeNodeToFile (std::ofstream& ofstrm) {};
    	//void setResources(std::tuple<Args...>);
private:
	typedef Node<Args...> this_t;
protected:
    Node(const Resources<Args...>& _res, const Entity_t& t, bool noid);
    Node(const Args &... _args, const Entity_t& t, bool noid);
    // id is a unique id assigned by IdGenerator
    int id;
    //local_id is a number strating from 0 it is local to the node.
    int local_id;
    Entity_t type;
    Resources<Args...> resources;
};
/*
template<typename ... Args>
Node<Args...>::Node() :
		id(vne::IdGenerator::getId<this_t>(this)), type(Entity_t::substrate)
{
}
template<typename ... Args>
Node<Args...>::Node(Entity_t t) :
		id(vne::IdGenerator::getId<this_t>(this)), type(t)
{
}
*/
template<typename ... Args>
Node<Args...>::Node(const Resources<Args...>& _res, const Entity_t& t) :
        id (vne::IdGenerator::Instance()->getId<this_t>(this)),
        type(t),
        resources (_res)
{
}
template<typename ... Args>
Node<Args...>::Node(const Resources<Args...>& _res, const Entity_t& t, bool noid)
        : type(t),
          resources (_res)
{
}
template<typename ... Args>
Node<Args...>::Node(const Args &... _args, const Entity_t& t)
    : id (vne::IdGenerator::Instance()->getId<this_t>(this)),
      type(t),
      resources (Resources<Args...> (_args...))
{
}
template<typename ... Args>
Node<Args...>::Node(const Args &... _args, const Entity_t& t, bool noid)
: type(t),
resources (Resources<Args...> (_args...))
{
}
template<typename ... Args>
Node<Args...>::~Node()
{
}
/*
template<typename ... Args>
void Node<Args...>::setResources(std::tuple<Args...> t)
{
	resources = t;
}
*/
template<typename ... Args>
const Resources<Args...>& Node<Args...>::getResources() const
{
	return resources;
}

template<typename ... Args>
const Entity_t& Node<Args...>::getType() const
{
	return type;
}
template<typename ... Args>
int Node<Args...>::getId() const
{
	return id;
}
}
#endif
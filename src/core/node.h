#ifndef NODE_H_
#define NODE_H_

#include <tuple>
#include "core-types.h"
#include "id-generator.h"

namespace vne{
template<typename... Args>
class Node
{
public:
	Node ();
	Node (Entity_t t);
	virtual ~Node ();
	int getId ();
	Entity_t getType();
	std::tuple<Args...> getResources ();
	void setResources(std::tuple<Args...>);
private:
	typedef Node<Args...> this_t;
	int id;
	Entity_t type;
	std::tuple<Args...> resources;
};

template<typename... Args>
Node<Args...>::Node ()
       : id(vne::IdGenerator::getId<this_t>(this)),
         type (Entity_t::substrate)
{
}

template<typename... Args>
Node<Args...>::Node (Entity_t t)
	: id(vne::IdGenerator::getId<this_t>(this)),
	  type (t)
{
}
template<typename... Args>
Node<Args...>::~Node ()
{
}
template<typename... Args>
void Node<Args...>::setResources (std::tuple<Args...> t){
        resources = t;
}
template<typename... Args>
std::tuple<Args...> Node<Args...>::getResources (){
        return resources;
}
template<typename... Args>
Entity_t Node<Args...>::getType () {
	return type;
}
template<typename... Args>
int Node<Args...>::getId () {
	return id;
}
}
#endif

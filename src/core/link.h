#ifndef LINK_H_
#define LINK_H_

#include <tuple>
#include <memory>

#include "core-types.h"
#include "id-generator.h"

namespace vne{
template<typename... Args>
class Link
{
public:
	Link ();
	Link (Entity_t t, int node_from, int node_to);
	virtual ~Link ();
	int getId ();
	Entity_t getType ();
	std::tuple<Args...> getResources ();
	void setResources(std::tuple<Args...>);
private:
	typedef Link<Args...> this_t;
	int id;
	Entity_t type;
	std::tuple<Args...> resources;
	int node_from_id;
	int node_to_id;
};
template<typename... Args>
Link<Args...>::Link ()
        : id (IdGenerator::getId<this_t>(this)),
	  type(Entity_t::substrate)
{
}
template<typename... Args>
Link<Args...>::Link (Entity_t t, int node_from, int node_to)
	: id(IdGenerator::getId<this_t>(this)),
	  type(t),
	  node_from_id(node_from),
	  node_to_id(node_to)
{
}
template<typename... Args>
Link<Args...>::~Link ()
{
}
template<typename... Args>
void Link<Args...>::setResources (std::tuple<Args...> t){
        resources = t;
}
template<typename... Args>
std::tuple<Args...> Link<Args...>::getResources (){
        return resources;
}
template<typename... Args>
Entity_t Link<Args...>::getType (){
	return type;
}
template<typename... Args>
int Link<Args...>::getId (){
	return id;
}
}
#endif


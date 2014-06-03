#ifndef ID_GENERATOR_H_
#define ID_GENERATOR_H_

#include <typeinfo>
#include <map>

namespace vne{
class IdGenerator
{
public:
	IdGenerator ();
	virtual ~IdGenerator ();
	template<typename T>
	static int getId (T *obj);
	template<typename T>
	static int peekId ();
	//template<typename T>
	//static void resetTypeCounter(T obj);
	//static void resetAllCounters();
private:
	static std::map<const char*, int> m_map;
};
std::map<const char*, int> IdGenerator::m_map;
IdGenerator::IdGenerator () 
{
}
IdGenerator::~IdGenerator () 
{
}
template<typename T>
int IdGenerator::getId (T *obj)
{
        int ret;
        const char* type_name = typeid(*obj).name();
        auto it = m_map.find(type_name);
        if (it == m_map.end())
        {
                m_map[type_name] = 0;
                ret = 0;
        }
        else
        {
                ret = ++it->second;
        }
        return ret;
}
template<typename T>
int IdGenerator::peekId ()
{
        int ret;
        const char* type_name = typeid(T).name();
        auto it = m_map.find(type_name);
        if (it == m_map.end())
        {
                ret = 0;
        }
        else
        {
                ret = it->second +1;
        }
        return ret;
}
}
#endif

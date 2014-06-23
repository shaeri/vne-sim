#ifndef CORE_TYPES_H_
#define CORE_TYPES_H_

namespace vne
{
/*
 * type of an entity either virtual or substrate (real)
 */
enum class Entity_t
{
	virt = 0, substrate
};
enum class Embedding_Result
{
	SUCCESSFUL_NODE_EMBEDDING =0, NOT_ENOUGH_SUBSTRATE_NODE_RESOURCES
};
}

#endif

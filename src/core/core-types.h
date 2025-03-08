/**
 * @file core-types.cc
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 3, 2014
 *
 * @copyright Copyright (c) Jun 3, 2014                      SOROUSH HAERI
 *            All Rights Reserved
 *
 *            Permission to use, copy, modify, and distribute this software and its
 *            documentation for any purpose and without fee is hereby granted, provided
 *            that the above copyright notice appear in all copies and that both that
 *            copyright notice and this permission notice appear in supporting
 *            documentation, and that the name of the author not be used in advertising or
 *            publicity pertaining to distribution of the software without specific,
 *            written prior permission.
 *
 *            THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *            ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *            AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *            DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *            AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *            OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef CORE_TYPES_H_
#define CORE_TYPES_H_

#include <string>
#include <boost/concept_check.hpp>

namespace vne
{
    
#define LargeInteger 100000
#define Infinity 1e+10
    
/*
 * type of an entity either virtual or substrate (real)
*/
enum class Entity_t
{
    virt = 0, 
    substrate
};
    
enum class Embedding_Result
{
    SUCCESSFUL_EMBEDDING = 0, 
    NOT_ENOUGH_SUBSTRATE_RESOURCES,
     ERROR_IN_SOLUTION
};
    
enum class Embedding_Algorithm_Types
{
    TWO_STAGE = 0, 
    SINGLE_STAGE
};
    
enum class Link_Embedding_Algo_Types
{
    WITH_PATH_SPLITTING = 0, 
    NO_PATH_SPLITTING
};
    
enum class Event_Types
{
    EVENT_TYPE_ARRIVAL = 0,
    EVENT_TYPE_SUCCESSFUL_EMBEDDING,
    EVENT_TYPE_FAIL_EMBEDDING,
    EVENT_TYPE_DEPARTURE
};
    
enum class Distribution
{
    UNIFORM = 0,
    EXPONENTIAL,
    POISSON
};

enum class Topology_Type
{
    DCNTwoTier = 0,
    DCNThreeTier,
    DCNBCube,
    DCNFatTree,
    Waxman,
    Barabasi,
    Erdos,
    HyperCube
};

static inline std::string get_Topology_Type_Str (Topology_Type tt)
{
    if (tt == Topology_Type::DCNTwoTier)
        return std::string ("DCNTwoTier");
    if (tt == Topology_Type::DCNThreeTier)
        return std::string ("DCNThreeTier");
    if (tt == Topology_Type::DCNBCube)
        return std::string ("DCNBCube");
    if (tt == Topology_Type::DCNFatTree)
	return std::string ("DCNFatTree");
    if (tt == Topology_Type::Waxman)
        return std::string ("Waxman");
    if (tt == Topology_Type::Barabasi)
        return std::string ("Barabasi");
    if (tt == Topology_Type::Erdos)
        return std::string ("Erdos");
	if (tt == Topology_Type::HyperCube)
		return std::string("HyperCube");

	// Fallthrough
	return std::string ("Uniform");
};
    
static inline Topology_Type get_Topology_Type (std::string tt)
{
    if (tt.compare ("DCNTwoTier") == 0)
		return Topology_Type::DCNTwoTier;
    if (tt.compare ("DCNThreeTier") == 0)
		return Topology_Type::DCNThreeTier;
    if (tt.compare ("DCNBCube") == 0)
		return Topology_Type::DCNBCube;
    if (tt.compare ("DCNFatTree") == 0)
		return Topology_Type::DCNFatTree;
    if (tt.compare ("Waxman") == 0)
		return Topology_Type::Waxman;
    if (tt.compare("Barabasi") == 0)
		return Topology_Type::Barabasi;
    if (tt.compare("Erdos") == 0)
		return Topology_Type::Erdos;
	if (tt.compare("HyperCube") == 0)
		return Topology_Type::HyperCube;
}

static inline std::string get_Distribution_Str (Distribution d)
{
    if (d == Distribution::UNIFORM)
        return std::string ("Uniform");
    if (d == Distribution::EXPONENTIAL)
        return std::string ("Exponential");
    if (d == Distribution::POISSON)
        return std::string ("Poisson");
    return std::string ("Uniform");
};
    
static inline std::string get_Embedding_Algorithm_Type_Str (Embedding_Algorithm_Types t)
{
    if (t == Embedding_Algorithm_Types::TWO_STAGE)
        return std::string ("TWO_STAGE");
    return std::string ("SINGLE_STAGE");
};
    
static inline std::string get_Evenet_Type_Str (Event_Types t)
{
    if (t == Event_Types::EVENT_TYPE_ARRIVAL)
        return std::string ("VNR_ARRIVAL");
    if (t == Event_Types::EVENT_TYPE_SUCCESSFUL_EMBEDDING)
        return std::string ("EMBD_SUCCESS");
    if (t == Event_Types::EVENT_TYPE_FAIL_EMBEDDING)
        return std::string ("EMBD_FAIL");
    return std::string ("VNR_DEPARTURE");
};
}

#endif

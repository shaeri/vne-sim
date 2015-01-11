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

namespace vne
{
    
#define LargeInteger 100000000
#define Infinity 1e+10
    
/*
 * type of an entity either virtual or substrate (real)
*/
enum class Entity_t
{
	virt = 0, substrate
};
enum class Embedding_Result
{
	SUCCESSFUL_EMBEDDING = 0, NOT_ENOUGH_SUBSTRATE_RESOURCES,
    ERROR_IN_SOLUTION
};
enum class Embedding_Algorithm_Types
{
    TWO_STAGE = 0, SINGLE_STAGE
};
static std::string get_Embedding_Algorithm_Type_Str (Embedding_Algorithm_Types t)
{
    if (t == Embedding_Algorithm_Types::TWO_STAGE)
        return std::string ("TWO_STAGE");
    return std::string ("SINGLE_STAGE");
};
enum class Event_Types
{
    EVENT_TYPE_ARRIVAL = 0,
    EVENT_TYPE_SUCCESSFUL_EMBEDDING,
    EVENT_TYPE_FAIL_EMBEDDING,
    EVENT_TYPE_DEPARTURE
};
static std::string get_Evenet_Type_Str (Event_Types t)
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

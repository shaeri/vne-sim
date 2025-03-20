/**
 * @file db-manager.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date 7/16/14
 *
 * @copyright Copyright (c) 7/16/14                      SOROUSH HAERI
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

#include "db-manager.h"

namespace vne
{
std::shared_ptr<DBManager> DBManager::_instance = nullptr;
std::shared_ptr<DBManager> DBManager::Instance()
{
    if (_instance == nullptr) {
        _instance = std::shared_ptr<DBManager>(new DBManager);
    }
    return _instance;
}
DBManager::DBManager() {}
DBManager::~DBManager() {}
std::shared_ptr<hiberlite::Database> DBManager::createDB(std::string &dbName)
{
    std::shared_ptr<hiberlite::Database> db(new hiberlite::Database(dbName));
    int id = (int)dbMap.size();
    dbIdNameMap[dbName] = id;
    dbMap[id] = db;
    return db;
}
std::shared_ptr<hiberlite::Database> DBManager::getDB(int dbId)
{
    return dbMap[dbId];
}
std::shared_ptr<hiberlite::Database> DBManager::getDB(std::string &dbName)
{
    return dbMap[dbIdNameMap[dbName]];
}
}  // namespace vne
/**
 * @file   experiments-test.cc
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date July 23, 2014
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

#include "experiments/vineyard-experiments.h"
#include "experiments/mcvne-experiments.h"

#include "core/db-manager.h"
#include "core/config-manager.h"
#include "utilities/logger.h"

#include <boost/test/unit_test.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace vne;
using namespace vne::vineyard;
using namespace vne::utilities;

BOOST_AUTO_TEST_SUITE (AlgorithmExperiments)

BOOST_AUTO_TEST_CASE(MCVNE_MCF)
{
    Logger::Instance()->logInfo("Running MCVNE_MCF Experiment");
    vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();
    
    std::string dbName;
    dbName = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
    
    exp.run();
    
    db->registerBeanClass<vne::experiments::MCVNENodeMCFLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}

BOOST_AUTO_TEST_CASE(DeterministicVine_MCF)
{
    Logger::Instance()->logInfo("Running DeterministicVine_MCF Experiment");
    vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();
    
    std::string dbName;
    dbName = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
    
    exp.run();
    
    db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
BOOST_AUTO_TEST_SUITE_END ()

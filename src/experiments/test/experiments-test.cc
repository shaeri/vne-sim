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
#include "experiments/grc-experiments.h"

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

BOOST_AUTO_TEST_CASE(MCVNE_MCF_VNR_TESTS)
{
    //std::string vnr_dirs[] =
    //	{"r-2000-50-20-10-10-10-50", "r-2000-50-50-20-10-10-50", "r-2000-50-50-20-10-15-50","r-2000-50-50-20-10-5-25"};
    //std::string vnr_dirs[] =
    //    	{"r-2000-50-50-20-10-5-25"};
    std::string vnr_dirs[] =
        	{"r-10-50-50-20-10-10-50"};
    int num_sims[] = {1};
	for (int i=0; i<1; i++)
	{
		bool ret = ConfigManager::Instance()->setConfig("MCTS.MCTSParameters.NumSimulations", num_sims[i]);
		assert (ret);
		for (int j = 0; j < 1; j++)
		{
			ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
			assert(ret);
			Logger::Instance()->logInfo("Running MCVNE_MCF_NUMSIMS Experiment");
			vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

			std::string dbPath;
			std::stringstream dbName;
			dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
			dbName << dbPath <<"mcvne_mcf_VNRs_" <<  vnr_dirs[j] << "_numSims_"<< num_sims[i] <<".db";
			std::string str = dbName.str();
			std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

			exp.run();


			db->registerBeanClass<vne::experiments::MCVNENodeMCFLinkExp<>>();
            db->dropModel();
			db->createModel();
			db->copyBean(exp);

			//destroy all singleton classes to start fresh for next simulations
			ConfigManager::Destroy();
			IdGenerator::Destroy();
			RNG::Destroy();
		}
	}
}

BOOST_AUTO_TEST_CASE(MCVNE_MCF_NUMSIMS_TEST)
{
    for (int i = 30; i>=5; i-=5)
    {
    	bool ret = ConfigManager::Instance()->setConfig("MCTS.MCTSParameters.NumSimulations", i);
    	assert (ret);
    	Logger::Instance()->logInfo("Running MCVNE_MCF_NUMSIMS Experiment");
    	vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

    	std::string dbPath;
    	std::stringstream dbName;
    	dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    	dbName << dbPath <<"mcvne_mcf_numsims_" << i << ".db";
    	std::string str = dbName.str();
    	std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

    	exp.run();

    	db->registerBeanClass<vne::experiments::MCVNENodeMCFLinkExp<>>();
    	db->dropModel();
    	db->createModel();
    	db->copyBean(exp);

    	ConfigManager::Destroy();
    }
}
            
BOOST_AUTO_TEST_CASE(GRC_BFS)
{
    Logger::Instance()->logInfo("Running GRC BFS Experiment");
    vne::experiments::GRCNodeBFSLinkExp<> exp = vne::experiments::GRCNodeBFSLinkExp<> ();
    
    std::string dbName;
    dbName = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
    
    exp.run();
    
    db->registerBeanClass<vne::experiments::GRCNodeBFSLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
    
BOOST_AUTO_TEST_CASE(GRC_MCF)
{
    Logger::Instance()->logInfo("Running GRC_BFS Experiment");
    vne::experiments::GRCNodeMCFLinkExp<> exp = vne::experiments::GRCNodeMCFLinkExp<> ();
    
    std::string dbName;
    dbName = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
    
    exp.run();
    
    db->registerBeanClass<vne::experiments::GRCNodeMCFLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
            
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

BOOST_AUTO_TEST_CASE(MCVNE_BFS)
{
    Logger::Instance()->logInfo("Running MCVNE_BFS Experiment");
    vne::experiments::MCVNENodeBFSLinkExp<> exp = vne::experiments::MCVNENodeBFSLinkExp<> ();
                
    std::string dbName;
    dbName = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
                
    exp.run();
                
    db->registerBeanClass<vne::experiments::MCVNENodeBFSLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
BOOST_AUTO_TEST_CASE(Vine_MCF)
{
    Logger::Instance()->logInfo("Running MCVNE_BFS Experiment");
    vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();
                
    std::string dbName;
    dbName = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(dbName);
                
    exp.run();
                
    db->registerBeanClass<vne::experiments::MCVNENodeBFSLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
/*
BOOST_AUTO_TEST_CASE(Vine_MCF)
{
	std::string vnr_dir = "r-2000-50-50-20-10-5-25";
	bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dir);
	assert(ret);
    Logger::Instance()->logInfo("Running DeterministicVine_MCF Experiment");
    vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();
    
    std::string dbPath;
	std::stringstream dbName;
    dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
    dbName << dbPath <<"Deterministic_Vine_VNRs_" << vnr_dir  << ".db";
    std::string str = dbName.str();
    std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);
    
    exp.run();
    
    db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
    db->dropModel();
    db->createModel();
    db->copyBean(exp);
}
*/
BOOST_AUTO_TEST_SUITE_END ()

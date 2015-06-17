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

BOOST_AUTO_TEST_CASE(ARRIVAL_RATE_TESTS)
{
    std::string vnr_dirs[] =
        	{"reqs-12-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-14-1000-nodesMin-3-nodesMax-10-grid-25"
      	, "reqs-16-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-20-1000-nodesMin-3-nodesMax-10-grid-25",
			"reqs-25-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-33-1000-nodesMin-3-nodesMax-10-grid-25",
        	"reqs-50-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-100-1000-nodesMin-3-nodesMax-10-grid-25"};




    //MCVNE MCF - BFS simulator
	for (int j = 0; j < 8; j++)
	{
		bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
		assert(ret);
		std::string str1 = "BFS-SP";
		ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
		vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"mcvne_bfs_mcf_" <<  vnr_dirs[j] <<".db";
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


		// MCVNE MCF MCF
	for (int j = 0; j < 8; j++)
		{
			bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
			assert(ret);
			std::string str1 = "MCF";
			ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
			assert(ret);
			vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

			std::string dbPath;
			std::stringstream dbName;
			dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
			dbName << dbPath <<"mcvne_mcf_mcf_" <<  vnr_dirs[j] <<".db";
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

	//MCVNE BFS - BFS simulator
	for (int j = 0; j < 8; j++)
	{
		bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
		assert(ret);
		std::string str1 = "BFS-SP";
		ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
		vne::experiments::MCVNENodeBFSLinkExp<> exp = vne::experiments::MCVNENodeBFSLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"mcvne_bfs_bfs_" <<  vnr_dirs[j] << ".db";
		std::string str = dbName.str();
		std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

		exp.run();


		db->registerBeanClass<vne::experiments::MCVNENodeBFSLinkExp<>>();
		db->dropModel();
		db->createModel();
		db->copyBean(exp);

		//destroy all singleton classes to start fresh for next simulations
		ConfigManager::Destroy();
		IdGenerator::Destroy();
		RNG::Destroy();
	}

	//GRC MCF
	for (int j = 0; j < 8; j++)
	{
		bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
		assert(ret);
		vne::experiments::GRCNodeMCFLinkExp<> exp = vne::experiments::GRCNodeMCFLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"grc_mcf_" <<  vnr_dirs[j] << ".db";
		std::string str = dbName.str();
		std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

		exp.run();


		db->registerBeanClass<vne::experiments::GRCNodeMCFLinkExp<>>();
		db->dropModel();
		db->createModel();
		db->copyBean(exp);

		//destroy all singleton classes to start fresh for next simulations
		ConfigManager::Destroy();
		IdGenerator::Destroy();
		RNG::Destroy();
	}

	//GRC BFS
	for (int j = 0; j < 8; j++)
	{
		bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
		assert(ret);
		vne::experiments::GRCNodeBFSLinkExp<> exp = vne::experiments::GRCNodeBFSLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"grc_bfs_" <<  vnr_dirs[j] << ".db";
		std::string str = dbName.str();
		std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

		exp.run();


		db->registerBeanClass<vne::experiments::GRCNodeBFSLinkExp<>>();
		db->dropModel();
		db->createModel();
		db->copyBean(exp);

		//destroy all singleton classes to start fresh for next simulations
		ConfigManager::Destroy();
		IdGenerator::Destroy();
		RNG::Destroy();
	}

	//Vineyard Deterministic
	for (int j = 0; j < 8; j++)
	{
		bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
		assert(ret);
		vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"vineyard_deterministic_" <<  vnr_dirs[j] << ".db";
		std::string str = dbName.str();
		std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

		exp.run();


		db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
		db->dropModel();
		db->createModel();
		db->copyBean(exp);

		//destroy all singleton classes to start fresh for next simulations
		ConfigManager::Destroy();
		IdGenerator::Destroy();
		RNG::Destroy();
	}

	//Vineyard Randomized
	for (int j = 0; j < 8; j++)
	{
		bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[j]);
		assert(ret);
		std::string str1 = "randomized";
		ret = ConfigManager::Instance()->setConfig("vineyard.Configs.nodeMappingType", str1);
		assert(ret);
		vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"vineyard_randomized_" <<  vnr_dirs[j] << ".db";
		std::string str = dbName.str();
		std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

		exp.run();


		db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
		db->dropModel();
		db->createModel();
		db->copyBean(exp);

		//destroy all singleton classes to start fresh for next simulations
		ConfigManager::Destroy();
		IdGenerator::Destroy();
		RNG::Destroy();
	}
}

BOOST_AUTO_TEST_CASE(MCVNE_NUMSIMS_TEST)
{
	std::string vnr_dirs[] =
	        	{"reqs-12-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-14-1000-nodesMin-3-nodesMax-10-grid-25"
	      	, "reqs-16-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-20-1000-nodesMin-3-nodesMax-10-grid-25",
				"reqs-25-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-33-1000-nodesMin-3-nodesMax-10-grid-25",
	        	"reqs-50-1000-nodesMin-3-nodesMax-10-grid-25", "reqs-100-1000-nodesMin-3-nodesMax-10-grid-25"};

	int numSims[] = {5,10,20,40,50,75,100,250};

	// MCVNE BFS BFS
    for (int i = 0; i<8; i++)
    {
    	bool ret = ConfigManager::Instance()->setConfig("MCTS.MCTSParameters.NumSimulations", numSims[i]);
    	assert (ret);
    	ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[6]);
    	assert (ret);
		std::string str1 = "BFS-SP";
		ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
		assert (ret);
    	vne::experiments::MCVNENodeBFSLinkExp<> exp = vne::experiments::MCVNENodeBFSLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"mcvne_bfs_bfs_" <<  vnr_dirs[6] << "_" << numSims[i] <<".db";
		std::string str = dbName.str();
		std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

		exp.run();


		db->registerBeanClass<vne::experiments::MCVNENodeBFSLinkExp<>>();
		db->dropModel();
		db->createModel();
		db->copyBean(exp);

		//destroy all singleton classes to start fresh for next simulations
		ConfigManager::Destroy();
		IdGenerator::Destroy();
		RNG::Destroy();
    }

	//MCVNE MCF MCF
    for (int i = 0; i<8; i++)
    {
    	bool ret = ConfigManager::Instance()->setConfig("MCTS.MCTSParameters.NumSimulations", numSims[i]);
    	assert (ret);
    	ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_dirs[6]);
    	assert (ret);
		std::string str1 = "MCF";
		ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
		assert (ret);
    	vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

		std::string dbPath;
		std::stringstream dbName;
		dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
		dbName << dbPath <<"mcvne_mcf_mcf_" <<  vnr_dirs[6] << "_" << numSims[i] <<".db";
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
                
    db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
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

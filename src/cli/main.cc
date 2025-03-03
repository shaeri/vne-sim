//
// Created by Rob Ballantyne on 2025-01-20.
//

#include <iostream>
#include <boost/program_options.hpp>

#include "experiments/vineyard-experiments.h"
#include "experiments/mcvne-experiments.h"
#include "experiments/grc-experiments.h"
#include "core/db-manager.h"
#include "core/config-manager.h"
#include "core/experiment.h"
#include "network-file-generator/network-file-generator.h"
#include "utilities/logger.h"

using namespace vne;
using namespace vne::vineyard;
using namespace vne::utilities;
using namespace vne::nfg;
using namespace std;

namespace po = boost::program_options;

int run_experiment(string vnr_file, string algo) {

    if ( algo == "mcvne_bfs_mcf" ) {

        string str1 = "BFS-SP";
        bool ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
        vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath << "mcvne_bfs_mcf_" << vnr_file <<".db";
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

    } else if ( algo == "mcvne_mcf_mcf" ) {

        bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_file);
        assert(ret);
        std::string str1 = "MCF";
        ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
        assert(ret);
        vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath <<"mcvne_mcf_mcf_" <<  vnr_file <<".db";
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

    } else if ( algo == "mcvne_bfs_bfs" ) {

        bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_file);
        assert(ret);
        std::string str1 = "BFS-SP";
        ret = ConfigManager::Instance()->setConfig("MCVNE.NodeEmbeddingAlgo.LinkEmbedder", str1);
        vne::experiments::MCVNENodeBFSLinkExp<> exp = vne::experiments::MCVNENodeBFSLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath <<"mcvne_bfs_bfs_" <<  vnr_file << ".db";
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

    } else if ( algo == "grc_mcf" ) {

        bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_file);
        assert(ret);
        vne::experiments::GRCNodeMCFLinkExp<> exp = vne::experiments::GRCNodeMCFLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath <<"grc_mcf_" <<  vnr_file << ".db";
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

    } else if ( algo == "grc_bfs" ) {

        bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_file);
        assert(ret);
        vne::experiments::GRCNodeBFSLinkExp<> exp = vne::experiments::GRCNodeBFSLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath <<"grc_bfs_" <<  vnr_file << ".db";
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

    } else if ( algo == "vineyard_d" ) {

        bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_file);
        assert(ret);
        vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath <<"vineyard_deterministic_" <<  vnr_file << ".db";
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

    } else if ( algo == "vineyard_r" ) {

        bool ret = ConfigManager::Instance()->setConfig("vineyard.VirtualNetRequest.dir", vnr_file);
        assert(ret);
        std::string str1 = "randomized";
        ret = ConfigManager::Instance()->setConfig("vineyard.Configs.nodeMappingType", str1);
        assert(ret);
        vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<> ();

        std::string dbPath;
        std::stringstream dbName;
        dbPath = ConfigManager::Instance()->getConfig<std::string>("core.dbPath");
        dbName << dbPath <<"vineyard_randomized_" <<  vnr_file << ".db";
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

    } else {
        cout << "Error - unkown algorithm name" << endl;
        exit(-1);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    vne::utilities::Logger::Instance()->logWarn("vne-startup");

    po::options_description desc("Root Options");
    desc.add_options()
            ( "help,h", "help message here")
            ("config", po::value<string>(), "location of the config file")
            ("subcmd", po::value<string>(), "a single sub command")
            ("subargs", po::value<vector<string>>(), "Arguments for subcommand");

    po::positional_options_description subcmds;
    subcmds.add("subcmd",1)
           .add("subargs", -1);

    po::variables_map opts;
    po::parsed_options parsed = po::command_line_parser(argc, argv)
            .options(desc)
            .positional(subcmds)
            .allow_unregistered()
            .run();
    try {
         po::store(parsed, opts);
         po::notify(opts);
    }
    catch (po::error &e) {
        cout << "Something went wrong--like a bad option.\nUsage:\n" << e.what() << desc << endl;
        exit(-1);
    }

    if ( opts.count("help") ) {
        cout << desc << endl;
        return 1;
    }

    if ( opts.count("verbose") ) {
        cout << "Verbosity on!" << endl;
    }

    if ( opts.count("config") ) {
        vne::config_path = opts["config"].as<string>();
        string logMsg = "Using file: " + vne::config_path + " as configuration file.";
        Logger::Instance()->logInfo(logMsg);
    }

    if ( opts["subcmd"].as<string>() == "simulate" ) {

        po::options_description simulate_opts("simulate options");
        simulate_opts.add_options()
                ("algorithm,a", po::value<string>(),
                 "An algorithm: mcvne_bfs_mcf, mcvne_mcf_mcf, mcvne_bfs_bfs, grc_mcf, grc_bfs, vineyard_d, vineyard_r")
                ("vnr_dir,r", po::value<string>(), "The directory name (within the VNR directory) that holds the VNRs");

        vector<string> remaining = po::collect_unrecognized(parsed.options, po::include_positional);
        remaining.erase(remaining.begin());
        po::variables_map simulate_vars;
        po::store(po::command_line_parser(remaining).options(simulate_opts).run(), simulate_vars);

        int ret = run_experiment(simulate_vars["vnr_dir"].as<string>(), simulate_vars["algorithm"].as<string>());

        if ( ret ) {
            cout << "Something went wrong with the simulation." << endl;
        }

    } else if ( opts["subcmd"].as<string>() == "substrategen" ) {

        NetworkFileGenerator nfg;
        nfg.VYSubstrateNetFileGenerator(true);

    } else if ( opts["subcmd"].as<string>() == "requestgen") {

        NetworkFileGenerator nfg;
        nfg.VYVirtualNetRequestGenerator(true);

    } else {

        cout << "No such sub-command: " << opts["subcmd"].as<string>() << endl;
        exit(-1);

    }


    return 0;
}
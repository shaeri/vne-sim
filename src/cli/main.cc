/**
 * @file main.cc
 * @brief Main CLI interface for VNE-SIM Virtual Network Embedding Simulator
 */

#include <iostream>
#include "experiments/vineyard-experiments.h"
#include "experiments/mcvne-experiments.h"
#include "experiments/grc-experiments.h"
#include "core/db-manager.h"
#include "core/config-manager.h"
#include "core/experiment.h"
#include "network-file-generator/network-file-generator.h"
#include "utilities/logger.h"
#include "CLI11.hpp"

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

using namespace vne;
using namespace vne::vineyard;
using namespace vne::utilities;
using namespace vne::nfg;
using namespace std;

/**
 * Runs the specified VNE algorithm experiment and stores results in database
 *
 * @param algo The algorithm to run (mcvne_mcf, mcvne_sp, grc_mcf, grc_sp, vineyard)
 * @return 0 on success, -1 on unknown algorithm
 */
int run_experiment(string algo)
{
    auto dbPath = ConfigManager::Instance()->getConfig<std::string>("core", "dbPath");
    auto vnr_file = ConfigManager::Instance()->getConfig<std::string>("vineyard",
                                                                      "VirtualNetRequest", "dir");

    if (algo == "mcvne_mcf") {
        vne::experiments::MCVNENodeMCFLinkExp<> exp = vne::experiments::MCVNENodeMCFLinkExp<>();

        std::string str = dbPath + "mcvne_mcf_" + vnr_file + ".db";
        std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

        exp.run();

        db->registerBeanClass<vne::experiments::MCVNENodeMCFLinkExp<>>();
        db->dropModel();
        db->createModel();
        db->copyBean(exp);

    } else if (algo == "mcvne_sp") {
        vne::experiments::MCVNENodeBFSLinkExp<> exp = vne::experiments::MCVNENodeBFSLinkExp<>();

        std::string str = dbPath + "mcvne_sp_" + vnr_file + ".db";
        std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

        exp.run();

        db->registerBeanClass<vne::experiments::MCVNENodeBFSLinkExp<>>();
        db->dropModel();
        db->createModel();
        db->copyBean(exp);

    } else if (algo == "grc_mcf") {
        vne::experiments::GRCNodeMCFLinkExp<> exp = vne::experiments::GRCNodeMCFLinkExp<>();

        std::string str = dbPath + "grc_mcf_" + vnr_file + ".db";
        std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

        exp.run();

        db->registerBeanClass<vne::experiments::GRCNodeMCFLinkExp<>>();
        db->dropModel();
        db->createModel();
        db->copyBean(exp);

    } else if (algo == "grc_sp") {
        vne::experiments::GRCNodeBFSLinkExp<> exp = vne::experiments::GRCNodeBFSLinkExp<>();

        std::string str = dbPath + "grc_sp_" + vnr_file + ".db";
        std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

        exp.run();

        db->registerBeanClass<vne::experiments::GRCNodeBFSLinkExp<>>();
        db->dropModel();
        db->createModel();
        db->copyBean(exp);

    } else if (algo == "vineyard") {
        vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<>();

        std::string str = dbPath + "vineyard_" + vnr_file + ".db";
        std::shared_ptr<hiberlite::Database> db = DBManager::Instance()->createDB(str);

        exp.run();

        db->registerBeanClass<vne::experiments::VineNodeMCFLinkExp<>>();
        db->dropModel();
        db->createModel();
        db->copyBean(exp);

    } else {
        cout << "Error - unkown algorithm name" << endl;

        //destroy all singleton classes to start fresh for next simulations
        ConfigManager::Destroy();
        IdGenerator::Destroy();
        RNG::Destroy();

        exit(-1);
    }

    //destroy all singleton classes to start fresh for next simulations
    ConfigManager::Destroy();
    IdGenerator::Destroy();
    RNG::Destroy();

    return 0;
}

/**
 * Custom formatter for CLI output with colored text
 */
class ColorFormatter : public CLI::Formatter
{
   private:
    std::string replaceCharWithTwo(const std::string &original, char target, char replacement1,
                                   char replacement2) const
    {
        std::string result;
        result.reserve(original.size() * 2);
        for (char c : original) {
            if (c == target) {
                result += replacement1;
                result += replacement2;
            } else {
                result += c;
            }
        }
        return result;
    }

   public:
    std::string make_description(const CLI::App *app) const override
    {
        std::string desc = CLI::Formatter::make_description(app);
        return fmt::format(fg(fmt::color::aquamarine) | fmt::emphasis::bold, desc);
    }

    std::string make_option_opts(const CLI::Option *opt) const override
    {
        std::string opts;
        // First the required label
        if (opt->get_required()) {
            opts += fmt::format(fg(fmt::color::crimson) | fmt::emphasis::bold, "  REQUIRED");
        }

        // then type names
        if (!opt->get_type_name().empty()) {
            std::string str = "  " + opt->get_type_name();
            str = replaceCharWithTwo(str, '{', '{', '{');
            str = replaceCharWithTwo(str, '}', '}', '}');
            opts += fmt::format(fg(fmt::color::medium_spring_green), str);
        }

        // then default value
        if (opt->get_default_str().size() > 0) {
            opts += fmt::format(fg(fmt::color::medium_purple),
                                "  [" + opt->get_default_str() + "]");
        }

        return opts;
    }

    std::string make_option_name(const CLI::Option *opt, bool positional) const override
    {
        std::string str = CLI::Formatter::make_option_name(opt, positional);
        if (opt->get_required()) {
            return fmt::format(fg(fmt::color::light_sky_blue) | fmt::emphasis::bold, str);
        }
        return fmt::format(fg(fmt::color::light_sky_blue), str);
    }

    std::string make_option_desc(const CLI::Option *opt) const override
    {
        std::string str = CLI::Formatter::make_option_desc(opt);
        return fmt::format(fg(fmt::color::khaki) | fmt::emphasis::italic, str);
    }
};

int main(int argc, char **argv)
{
    // First parse just the config file path if specified
    std::string config_path;
    CLI::App prefix_app;
    prefix_app.prefix_command();
    prefix_app
        .add_option_function<std::string>("-c,--config",
                                          [&config_path](const std::string &cfg_path) {
                                              config_path = cfg_path;
                                              ConfigManager::Instance()->loadConfig(cfg_path);
                                          })
        ->description("Path to TOML configuration file")
        ->default_val("./config.toml")
        ->check(CLI::ExistingFile);
    prefix_app.allow_extras();
    prefix_app.set_help_flag("", "");  // Disable help for prefix command
    prefix_app.set_help_all_flag("", "");

    try {
        prefix_app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return prefix_app.exit(e);
    }

    // Main CLI application setup
    CLI::App app{"VNE-SIM: A Virtual Network Embedding Simulator", "vnesim"};

    // Set callback to lock the configs after parsing of the arguments is finished
    app.parse_complete_callback([]() { ConfigManager::Instance()->lockConfigs(); });

    // Enable help all flag to show all subcommands
    app.set_help_all_flag("--help-all", "Expand all help");

    // Setup colored formatter
    auto fmt = std::make_shared<ColorFormatter>();
    fmt->column_width(20);
    fmt->right_column_width(120);

    // set the app formatter
    app.formatter(fmt);

    // Config file option (already parsed, just show status)
    app.add_option("-c,--config", "Path to config file")
        ->description(config_path.empty()
                          ? "not specified, default TOML config file path will be used"
                          : "Already specified: [" + config_path + "] will be used.")
        ->default_val("./config.toml");

    // Shared variables
    std::string link_embedding;

    ////////////////////////////////////////////////////////////////////////////
    // Experiment Subcommands
    ////////////////////////////////////////////////////////////////////////////
    auto experiment = app.add_subcommand("experiment", "Run VNE experiments");

    // Vineyard experiment
    auto vineyard_exp = experiment->add_subcommand("vineyard", "Run Vineyard algorithm");
    vineyard_exp->callback([]() { run_experiment("vineyard"); });

    // GRC experiment
    auto grc_exp = experiment->add_subcommand("grc", "Run GRC algorithm");
    grc_exp
        ->add_option("-l,--link-embedding-algo", link_embedding,
                     "Link embedding algorithm: sp (Shortest Path) or mcf (Multi-Commodity Flow)")
        ->required()
        ->check(CLI::IsMember({"sp", "mcf"}));
    grc_exp->callback([&]() { run_experiment(link_embedding == "sp" ? "grc_sp" : "grc_mcf"); });

    // MCVNE experiment
    auto mcvne_exp = experiment->add_subcommand("mcvne", "Run MCVNE algorithm");
    mcvne_exp
        ->add_option("-l,--link-embedding-algo", link_embedding,
                     "Link embedding algorithm: sp (Shortest Path) or mcf (Multi-Commodity Flow)")
        ->required()
        ->check(CLI::IsMember({"sp", "mcf"}));
    mcvne_exp->callback(
        [&]() { run_experiment(link_embedding == "sp" ? "mcvne_sp" : "mcvne_mcf"); });

    ////////////////////////////////////////////////////////////////////////////
    // Network Generation Subcommands
    ////////////////////////////////////////////////////////////////////////////
    auto netgen = app.add_subcommand("netgen", "Generate network files");

    // Substrate network generation
    auto substrate_gen = netgen->add_subcommand("substrate", "Generate substrate network");
    substrate_gen->callback([]() {
        NetworkFileGenerator nfg;
        nfg.VYSubstrateNetFileGenerator(true);
    });

    // Virtual network requests generation
    auto vr_gen = netgen->add_subcommand("vr", "Generate virtual network requests");
    vr_gen->callback([]() {
        NetworkFileGenerator nfg;
        nfg.VYVirtualNetRequestGenerator(true);
    });

    // [utilities]
    app.add_option_function<std::string>("-l,--logLevel",
                                         [](const std::string &level) {
                                             ConfigManager::Instance()->setConfig(
                                                 level, "utilities", "logLevel");
                                         })
        ->description("Log level (debug, info, warning, error, fatal)")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("utilities", "logLevel"))
        ->check(CLI::IsMember({"debug", "info", "warning", "error", "fatal"}));

    app.add_option_function<std::string>("-p,--pythonPath",
                                         [](const std::string &path) {
                                             ConfigManager::Instance()->setConfig(
                                                 path, "utilities", "pythonPath");
                                         })
        ->description("Path to python interpreter for FNSS")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("utilities", "pythonPath"))
        ->check(CLI::ExistingFile);

    // [core]
    app.add_flag_function("-i,--ignoreLocationConstraint",
                          [](long ignore) {
                              ConfigManager::Instance()->setConfig(ignore > 0, "core",
                                                                   "ignoreLocationConstrain");
                          })
        ->description("Ignore location constraints during embedding")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("core", "ignoreLocationConstrain"));

    app.add_option_function<std::string>("-d,--dbPath",
                                         [](const std::string &path) {
                                             ConfigManager::Instance()->setConfig(path, "core",
                                                                                  "dbPath");
                                         })
        ->description("Path to store result databases")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("core", "dbPath"))
        ->check(CLI::ExistingDirectory);

    app.add_option_function<int>(
           "-s,--seed",
           [](int seed) { ConfigManager::Instance()->setConfig(seed, "core", "rngSeed"); })
        ->description("Random number generator seed (0 for random)")
        ->default_val(ConfigManager::Instance()->getConfig<int>("core", "rngSeed"))
        ->check(CLI::NonNegativeNumber);

    app.add_flag_function("-S,--parallel-runs-identical-seeds",
                          [](long use) {
                              ConfigManager::Instance()->setConfig(
                                  use > 0, "core", "rngUseSameSeedForParallelRuns");
                          })
        ->description("Use same RNG seed across parallel runs")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("core", "rngUseSameSeedForParallelRuns"));

    app.add_option_function<std::string>("-r,--rng-algo",
                                         [](const std::string &type) {
                                             ConfigManager::Instance()->setConfig(type, "core",
                                                                                  "rngType");
                                         })
        ->description("GSL RNG algorithm (e.g. gsl_rng_mt19937)")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("core", "rngType"))
        ->check(CLI::IsMember({"gsl_rng_mt19937", "gsl_rng_ranlxs4096", "gsl_rng_default"}));

    // Network generation options
    netgen
        ->add_option_function<std::string>("-H,--handler",
                                           [](const std::string &handler) {
                                               ConfigManager::Instance()->setConfig(
                                                   handler, "NetworkFileGenerator", "Handler");
                                           })
        ->description("Network generator handler (BRITE or FNSS)")
        ->default_val(
            ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator", "Handler"))
        ->check(CLI::IsMember({"BRITE", "FNSS"}))
        ->required();

    netgen
        ->add_option_function<std::string>("-d,--dest",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "NetworkFileGenerator", "DirToSaveFiles");
                                           })
        ->description("Directory to save generated network files")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator",
                                                                        "DirToSaveFiles"))
        ->check(CLI::ExistingDirectory);

    netgen
        ->add_option_function<std::string>("-b,--brite-seed-file",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "NetworkFileGenerator", "BriteHandler",
                                                   "BriteSeedFile");
                                           })
        ->description("BRITE seed file path")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "NetworkFileGenerator", "BriteHandler", "BriteSeedFile"))
        ->check(CLI::ExistingFile);

    netgen
        ->add_option_function<int>("-p,--brite-node-placement",
                                   [](int placement) {
                                       ConfigManager::Instance()->setConfig(
                                           placement, "NetworkFileGenerator", "BriteHandler",
                                           "nodePlacement");
                                   })
        ->description("BRITE node placement (1:Random, 2:HeavyTailed)")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "BriteHandler", "nodePlacement"))
        ->check(CLI::Range(1, 2));

    netgen
        ->add_option_function<int>("-n,--brite-num-neighbors",
                                   [](int num) {
                                       ConfigManager::Instance()->setConfig(
                                           num, "NetworkFileGenerator", "BriteHandler",
                                           "numNeighbors");
                                   })
        ->description("BRITE neighbors per node")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "BriteHandler", "numNeighbors"))
        ->check(CLI::PositiveNumber);

    netgen
        ->add_option_function<int>("-i,--brite-inner-grid",
                                   [](int size) {
                                       ConfigManager::Instance()->setConfig(
                                           size, "NetworkFileGenerator", "BriteHandler",
                                           "innerGridSize");
                                   })
        ->description("BRITE inner grid size")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "BriteHandler", "innerGridSize"))
        ->check(CLI::PositiveNumber);

    netgen
        ->add_option_function<int>("-o,--brite-outer-grid",
                                   [](int size) {
                                       ConfigManager::Instance()->setConfig(
                                           size, "NetworkFileGenerator", "BriteHandler",
                                           "outerGridSize");
                                   })
        ->description("BRITE outer grid size")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "BriteHandler", "outerGridSize"))
        ->check(CLI::PositiveNumber);

    // [NetworkFileGenerator.BriteHandler.RTWaxman]
    netgen
        ->add_option_function<int>("-g,--waxman-growth-type",
                                   [](int type) {
                                       ConfigManager::Instance()->setConfig(
                                           type, "NetworkFileGenerator", "BriteHandler",
                                           "RTWaxman", "growthType");
                                   })
        ->description("Waxman growth type (1:Incremental, 2:All)")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "BriteHandler", "RTWaxman", "growthType"))
        ->check(CLI::Range(1, 2));

    netgen
        ->add_option_function<double>("-A,--waxman-alpha",
                                      [](double alpha) {
                                          ConfigManager::Instance()->setConfig(
                                              alpha, "NetworkFileGenerator", "BriteHandler",
                                              "RTWaxman", "alpha");
                                      })
        ->description("Waxman alpha parameter (0-1)")
        ->default_val(ConfigManager::Instance()->getConfig<double>(
            "NetworkFileGenerator", "BriteHandler", "RTWaxman", "alpha"))
        ->check(CLI::Range(0.0, 1.0));

    netgen
        ->add_option_function<double>("-B,--waxman-beta",
                                      [](double beta) {
                                          ConfigManager::Instance()->setConfig(
                                              beta, "NetworkFileGenerator", "BriteHandler",
                                              "RTWaxman", "beta");
                                      })
        ->description("Waxman beta parameter (0-1)")
        ->default_val(ConfigManager::Instance()->getConfig<double>(
            "NetworkFileGenerator", "BriteHandler", "RTWaxman", "beta"))
        ->check(CLI::Range(0.0, 1.0));

    substrate_gen
        ->add_option_function<std::string>("-t,--topology",
                                           [](const std::string &type) {
                                               ConfigManager::Instance()->setConfig(
                                                   type, "NetworkFileGenerator", "SNTopologyType");
                                           })
        ->description("Substrate network topology type")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator",
                                                                        "SNTopologyType"))
        ->check(CLI::IsMember({"Waxman", "Barabasi", "DCNTwoTier", "DCNThreeTier", "DCNBCube",
                               "DCNFatTree", "HyperCube"}))
        ->required();

    substrate_gen
        ->add_option_function<int>("-n,--num-nodes",
                                   [](int num) {
                                       ConfigManager::Instance()->setConfig(
                                           num, "NetworkFileGenerator", "SubstrateNodeNum");
                                   })
        ->description("Number of nodes in substrate network")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator", "SubstrateNodeNum"))
        ->check(CLI::PositiveNumber);

    vr_gen
        ->add_option_function<std::string>("-t,--topology",
                                           [](const std::string &type) {
                                               ConfigManager::Instance()->setConfig(
                                                   type, "NetworkFileGenerator", "VNTopologyType");
                                           })
        ->description("Virtual network topology type")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator",
                                                                        "VNTopologyType"))
        ->check(CLI::IsMember({"Waxman", "Barabasi"}))
        ->required();

    vr_gen
        ->add_option_function<int>("-T,--total-time",
                                   [](int time) {
                                       ConfigManager::Instance()->setConfig(
                                           time, "NetworkFileGenerator", "TotalTime");
                                   })
        ->description("Total simulation time in an arbitrary unit.")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator", "TotalTime"))
        ->check(CLI::PositiveNumber);

    vr_gen
        ->add_option_function<double>("-r,--link-splitting-rate",
                                      [](double rate) {
                                          ConfigManager::Instance()->setConfig(
                                              rate, "NetworkFileGenerator",
                                              "VNRLinkSplittingRate");
                                      })
        ->description("VNR link splitting probability (0-1)")
        ->default_val(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator",
                                                                   "VNRLinkSplittingRate"))
        ->check(CLI::Range(0.0, 1.0));

    // Distribution parameters
    auto add_dist_param = [&](CLI::App *sub_cmd, const std::string &name,
                              const std::string &config, const std::string &desc) {
        sub_cmd
            ->add_option_function<int>("--" + name + "-dist",
                                       [name, config](int dist) {
                                           ConfigManager::Instance()->setConfig(
                                               dist, "NetworkFileGenerator", config + "Dist");
                                       })
            ->description(desc + " distribution (0:Uniform, 1:Exponential, 2:Poisson)")
            ->default_val(
                ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator", config + "Dist"))
            ->check(CLI::Range(0, 2));

        sub_cmd
            ->add_option_function<double>("--" + name + "-p1",
                                          [name, config](double param) {
                                              ConfigManager::Instance()->setConfig(
                                                  param, "NetworkFileGenerator",
                                                  config + "DistParam1");
                                          })
            ->description(desc + " distribution parameter 1")
            ->default_val(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator",
                                                                       config + "DistParam1"));

        sub_cmd
            ->add_option_function<double>("--" + name + "-p2",
                                          [name, config](double param) {
                                              ConfigManager::Instance()->setConfig(
                                                  param, "NetworkFileGenerator",
                                                  config + "DistParam2");
                                          })
            ->description(desc + " distribution parameter 2")
            ->default_val(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator",
                                                                       config + "DistParam2"));

        sub_cmd
            ->add_option_function<double>("--" + name + "-p3",
                                          [name, config](double param) {
                                              ConfigManager::Instance()->setConfig(
                                                  param, "NetworkFileGenerator",
                                                  config + "DistParam3");
                                          })
            ->description(desc + " distribution parameter 3")
            ->default_val(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator",
                                                                       config + "DistParam3"));
    };

    add_dist_param(substrate_gen, "sn-cpu", "SNCPU", "Substrate node CPU distribution");
    add_dist_param(substrate_gen, "sl-bw", "SLBW", "Substrate link bandwidth distribution");
    add_dist_param(substrate_gen, "sl-delay", "SLDelay", "Substrate link delay distribution");

    add_dist_param(vr_gen, "vnr-num-nodes", "VNRNumNodes", "VNR number of nodes distribution");
    add_dist_param(vr_gen, "vnr-duration", "VNRDuration", "VNR duration distribution");
    add_dist_param(vr_gen, "vnr-arrival", "VNRArrival", "VNR arrival time distribution");
    add_dist_param(vr_gen, "vnr-max-distance", "VNRMaxDistance", "VNR max distance distribution");
    add_dist_param(vr_gen, "vn-cpu", "VNCPU", "Virtual node CPU distribution");
    add_dist_param(vr_gen, "vl-bw", "VLBW", "Virtual link bandwidth distribution");
    add_dist_param(vr_gen, "vl-delay", "VLDelay", "Virtual link delay distribution");

    // DCNBCube
    substrate_gen
        ->add_option_function<int>("--bcube-n",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNBCube",
                                           "N");
                                   })
        ->description("Hosts per Bcube_0")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "FNSSHandler", "DCNBCube", "N"))
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--bcube-k",
                                   [](int k) {
                                       ConfigManager::Instance()->setConfig(
                                           k, "NetworkFileGenerator", "FNSSHandler", "DCNBCube",
                                           "K");
                                   })
        ->description("Bcube level")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "FNSSHandler", "DCNBCube", "K"))
        ->check(CLI::PositiveNumber);

    // DCNTwoTier
    substrate_gen
        ->add_option_function<int>("--twotier-n-cores",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_core");
                                   })
        ->description("Number of core switches")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier", "n_core"))
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--twotier-n-edges",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_edges");
                                   })
        ->description("Number of edges per switch")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier", "n_edges"))
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--twotier-n-hosts",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_hosts");
                                   })
        ->description("Number of hosts per switch")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier", "n_hosts"))
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--twotier-bw-multiplier",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "coreBWMultiplier");
                                   })
        ->description("Core links bandwidth multiplier")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier", "coreBWMultiplier"))
        ->check(CLI::PositiveNumber);

    // DCNFatTree
    substrate_gen
        ->add_option_function<int>("--fattree-k",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNFatTree",
                                           "K");
                                   })
        ->description("Number of switch ports.")
        ->default_val(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator",
                                                                "FNSSHandler", "DCNFatTree", "K"))
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--fattree-bw-multiplier",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNFatTree",
                                           "coreBWMultiplier");
                                   })
        ->description("Core links bandwidth multiplier")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "FNSSHandler", "DCNFatTree", "coreBWMultiplier"))
        ->check(CLI::PositiveNumber);

    // HyperCube
    substrate_gen
        ->add_option_function<int>("--hypercube-size",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "HyperCube",
                                           "size");
                                   })
        ->description("Hypercube dimenstion")
        ->default_val(ConfigManager::Instance()->getConfig<int>(
            "NetworkFileGenerator", "FNSSHandler", "HyperCube", "size"))
        ->check(CLI::PositiveNumber);

    // [vineyard.SubstrateNetwork]
    experiment
        ->add_option_function<std::string>("-p,--substrate-path",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "SubstrateNetwork", "path");
                                           })
        ->description("Substrate network files directory")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "vineyard", "SubstrateNetwork", "path"))
        ->check(CLI::ExistingDirectory);

    experiment
        ->add_option_function<std::string>("-f,--substreate-filename",
                                           [](const std::string &name) {
                                               ConfigManager::Instance()->setConfig(
                                                   name, "vineyard", "SubstrateNetwork",
                                                   "filename");
                                           })
        ->description("Substrate network filename")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "vineyard", "SubstrateNetwork", "filename"));

    // [vineyard.VirtualNetRequest]
    experiment
        ->add_option_function<std::string>("-r,--vnr-root-path",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "VirtualNetRequest", "path");
                                           })
        ->description("Virtual network requests root directory")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "vineyard", "VirtualNetRequest", "path"))
        ->check(CLI::ExistingDirectory);

    experiment
        ->add_option_function<std::string>("-d,--vnr-dir-name",
                                           [](const std::string &dir) {
                                               ConfigManager::Instance()->setConfig(
                                                   dir, "vineyard", "VirtualNetRequest", "dir");
                                           })
        ->description("Virtual network requests directory name")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "vineyard", "VirtualNetRequest", "dir"));

    experiment
        ->add_option_function<std::string>("-e,--req-file-ext",
                                           [](const std::string &ext) {
                                               ConfigManager::Instance()->setConfig(
                                                   ext, "vineyard", "VirtualNetRequest",
                                                   "reqfileExtension");
                                           })
        ->description("Virtual network request file extension (txt)")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "vineyard", "VirtualNetRequest", "reqfileExtension"));

    // [vineyard.Constants]
    experiment
        ->add_option_function<double>("-m,--revenue-multiplier",
                                      [](double mult) {
                                          ConfigManager::Instance()->setConfig(
                                              mult, "vineyard", "Constants", "revenueMultiplier");
                                      })
        ->description("Revenue calculation multiplier")
        ->default_val(ConfigManager::Instance()->getConfig<double>("vineyard", "Constants",
                                                                   "revenueMultiplier"));

    experiment
        ->add_option_function<double>("-M,--cost-multiplier",
                                      [](double mult) {
                                          ConfigManager::Instance()->setConfig(
                                              mult, "vineyard", "Constants", "costMultiplier");
                                      })
        ->description("Cost calculation multiplier")
        ->default_val(ConfigManager::Instance()->getConfig<double>("vineyard", "Constants",
                                                                   "costMultiplier"));

    vineyard_exp
        ->add_option_function<std::string>("-t,--node-map-type",
                                           [](const std::string &type) {
                                               ConfigManager::Instance()->setConfig(
                                                   type, "vineyard", "Configs", "nodeMappingType");
                                           })
        ->required()
        ->description("Vineyard node mapping type")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>("vineyard", "Configs",
                                                                        "nodeMappingType"))
        ->check(CLI::IsMember({"deterministic", "randomized"}));

    vineyard_exp
        ->add_option_function<double>("--epsilon",
                                      [](double eps) {
                                          ConfigManager::Instance()->setConfig(
                                              eps, "vineyard", "Constants", "epsilon");
                                      })
        ->description("Numerical comparison epsilon")
        ->default_val(
            ConfigManager::Instance()->getConfig<double>("vineyard", "Constants", "epsilon"))
        ->check(CLI::PositiveNumber);

    vineyard_exp
        ->add_flag_function("--set-alpha",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(set > 0, "vineyard",
                                                                     "Configs", "setAlpha");
                            })
        ->description("Enable alpha parameter in Vineyard")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setAlpha"));

    vineyard_exp
        ->add_flag_function("--set-beta",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(set > 0, "vineyard",
                                                                     "Configs", "setBeta");
                            })
        ->description("Enable beta parameter in Vineyard")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setBeta"));

    // [vineyard.glpk]
    vineyard_exp
        ->add_option_function<std::string>("--lp-model",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "LPmodelFile");
                                           })
        ->description("GLPK LP model file")
        ->default_val(
            ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "LPmodelFile"))
        ->check(CLI::ExistingFile);

    vineyard_exp
        ->add_option_function<std::string>("--lp-data",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "LPdataFile");
                                           })
        ->description("GLPK LP data file")
        ->default_val(
            ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "LPdataFile"))
        ->check(CLI::ExistingFile);

    experiment
        ->add_option_function<std::string>("-o,--mcf-model",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "MCFmodelFile");
                                           })
        ->description("GLPK MCF model file")
        ->default_val(
            ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "MCFmodelFile"))
        ->check(CLI::ExistingFile);

    experiment
        ->add_option_function<std::string>("-O,--mcf-data",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "MCFdataFile");
                                           })
        ->description("GLPK MCF data file")
        ->default_val(
            ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "MCFdataFile"))
        ->check(CLI::ExistingFile);

    experiment
        ->add_flag_function("-E,--enable-glpk-terminal",
                            [](int enabled) {
                                ConfigManager::Instance()->setConfig(enabled, "vineyard", "glpk",
                                                                     "terminalEnabled");
                            })
        ->description("Enable GLPK terminal output")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("vineyard", "glpk", "terminalEnabled"));

    // [MCTS.Simulator.Knowledge]
    mcvne_exp
        ->add_option_function<int>("--tree-level",
                                   [](int level) {
                                       ConfigManager::Instance()->setConfig(
                                           level, "MCTS", "Simulator", "Knowledge", "TreeLevel");
                                   })
        ->description("MCTS tree knowledge level (0:PURE, 1:LEGAL, 2:SMART)")
        ->default_val(ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator", "Knowledge",
                                                                "TreeLevel"))
        ->check(CLI::Range(0, 2));

    mcvne_exp
        ->add_option_function<int>("--rollout-level",
                                   [](int level) {
                                       ConfigManager::Instance()->setConfig(
                                           level, "MCTS", "Simulator", "Knowledge",
                                           "RolloutLevel");
                                   })
        ->description("MCTS rollout knowledge level (0:PURE, 1:LEGAL, 2:SMART)")
        ->default_val(ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator", "Knowledge",
                                                                "RolloutLevel"))
        ->check(CLI::Range(0, 2));

    mcvne_exp
        ->add_option_function<int>("--smart-tree-count",
                                   [](int count) {
                                       ConfigManager::Instance()->setConfig(
                                           count, "MCTS", "Simulator", "Knowledge",
                                           "SmartTreeCount");
                                   })
        ->description("MCTS smart tree count")
        ->default_val(ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator", "Knowledge",
                                                                "SmartTreeCount"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<double>("--smart-tree-value",
                                      [](double value) {
                                          ConfigManager::Instance()->setConfig(
                                              value, "MCTS", "Simulator", "Knowledge",
                                              "SmartTreeValue");
                                      })
        ->description("MCTS smart tree value")
        ->default_val(ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator",
                                                                   "Knowledge", "SmartTreeValue"))
        ->check(CLI::PositiveNumber);

    // [MCTS.Simulator]
    mcvne_exp
        ->add_option_function<double>("--discount",
                                      [](double discount) {
                                          ConfigManager::Instance()->setConfig(
                                              discount, "MCTS", "Simulator", "discount");
                                      })
        ->description("MCTS discount factor (0-1]")
        ->default_val(
            ConfigManager::Instance()->getConfig<double>("MCTS", "Simulator", "discount"))
        ->check(CLI::Range(0.0, 1.0));

    mcvne_exp
        ->add_option_function<int>("--reward-range",
                                   [](int range) {
                                       ConfigManager::Instance()->setConfig(
                                           range, "MCTS", "Simulator", "rewardRange");
                                   })
        ->description("MCTS reward range")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("MCTS", "Simulator", "rewardRange"))
        ->check(CLI::PositiveNumber);

    // [MCTS.MCTSParameters]
    mcvne_exp
        ->add_option_function<int>("--max-depth",
                                   [](int depth) {
                                       ConfigManager::Instance()->setConfig(
                                           depth, "MCTS", "MCTSParameters", "MaxDepth");
                                   })
        ->description("MCTS maximum search depth")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("MCTS", "MCTSParameters", "MaxDepth"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<int>("--num-simulations",
                                   [](int num) {
                                       ConfigManager::Instance()->setConfig(
                                           num, "MCTS", "MCTSParameters", "NumSimulations");
                                   })
        ->description("MCTS number of simulations (self-plays) to generate a rollout")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("MCTS", "MCTSParameters", "NumSimulations"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<int>("--expand-count",
                                   [](int count) {
                                       ConfigManager::Instance()->setConfig(
                                           count, "MCTS", "MCTSParameters", "ExpandCount");
                                   })
        ->description("MCTS nodes to expand per step")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("MCTS", "MCTSParameters", "ExpandCount"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_flag_function("--auto-exploration",
                            [](long autoexp) {
                                ConfigManager::Instance()->setConfig(
                                    autoexp > 0, "MCTS", "MCTSParameters", "AutoExploration");
                            })
        ->description("Enable MCTS automatic exploration")
        ->default_val(ConfigManager::Instance()->getConfig<bool>("MCTS", "MCTSParameters",
                                                                 "AutoExploration"));

    mcvne_exp
        ->add_option_function<double>("--exploration-constant",
                                      [](double c) {
                                          ConfigManager::Instance()->setConfig(
                                              c, "MCTS", "MCTSParameters", "ExplorationConstant");
                                      })
        ->description("MCTS exploration constant")
        ->default_val(ConfigManager::Instance()->getConfig<double>("MCTS", "MCTSParameters",
                                                                   "ExplorationConstant"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_flag_function("--rave",
                            [](long use) {
                                ConfigManager::Instance()->setConfig(use > 0, "MCTS",
                                                                     "MCTSParameters", "UseRave");
                            })
        ->description("Enable MCTS RAVE")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("MCTS", "MCTSParameters", "UseRave"));

    mcvne_exp
        ->add_option_function<double>("--rave-discount",
                                      [](double discount) {
                                          ConfigManager::Instance()->setConfig(
                                              discount, "MCTS", "MCTSParameters", "RaveDiscount");
                                      })
        ->description("MCTS RAVE discount factor")
        ->default_val(
            ConfigManager::Instance()->getConfig<double>("MCTS", "MCTSParameters", "RaveDiscount"))
        ->check(CLI::Range(0.0, 1.0));

    mcvne_exp
        ->add_option_function<double>("--rave-constant",
                                      [](double c) {
                                          ConfigManager::Instance()->setConfig(
                                              c, "MCTS", "MCTSParameters", "RaveConstant");
                                      })
        ->description("MCTS RAVE constant")
        ->default_val(
            ConfigManager::Instance()->getConfig<double>("MCTS", "MCTSParameters", "RaveConstant"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_flag_function("--disable-tree",
                            [](long disable) {
                                ConfigManager::Instance()->setConfig(
                                    disable > 0, "MCTS", "MCTSParameters", "DisableTree");
                            })
        ->description("Disable MCTS tree (use rollout only)")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("MCTS", "MCTSParameters", "DisableTree"));

    mcvne_exp
        ->add_flag_function("--single-player-mcts",
                            [](long use) {
                                ConfigManager::Instance()->setConfig(
                                    use > 0, "MCTS", "MCTSParameters", "UseSinglePlayerMCTS");
                            })
        ->description("Enable single-player MCTS")
        ->default_val(ConfigManager::Instance()->getConfig<bool>("MCTS", "MCTSParameters",
                                                                 "UseSinglePlayerMCTS"));

    mcvne_exp
        ->add_option_function<int>("--spmcts-constant",
                                   [](int c) {
                                       ConfigManager::Instance()->setConfig(
                                           c, "MCTS", "MCTSParameters", "SPMCTSConstant");
                                   })
        ->description("Single-player MCTS constant")
        ->default_val(
            ConfigManager::Instance()->getConfig<int>("MCTS", "MCTSParameters", "SPMCTSConstant"))
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<int>("--parallelization",
                                   [](int type) {
                                       ConfigManager::Instance()->setConfig(
                                           type, "MCTS", "MCTSParameters", "ParallelizationType");
                                   })
        ->description("MCTS parallelization type (0:ActionRoot, 1:FullTree)")
        ->default_val(ConfigManager::Instance()->getConfig<int>("MCTS", "MCTSParameters",
                                                                "ParallelizationType"))
        ->check(CLI::Range(0, 1));

    // [MCVNE.VNEMCTSSimulator]
    mcvne_exp
        ->add_flag_function("--set-alpha",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(
                                    set > 0, "MCVNE", "VNEMCTSSimulator", "setAlpha");
                            })
        ->description("Enable MCVNE substrate links weight (alpha)")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("MCVNE", "VNEMCTSSimulator", "setAlpha"));

    mcvne_exp
        ->add_flag_function("--set-beta",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(
                                    set > 0, "MCVNE", "VNEMCTSSimulator", "setBeta");
                            })
        ->description("Enable MCVNE substrate node weight (beta)")
        ->default_val(
            ConfigManager::Instance()->getConfig<bool>("MCVNE", "VNEMCTSSimulator", "setBeta"));

    // [MCVNE.NodeEmbeddingAlgo]
    mcvne_exp
        ->add_option_function<std::string>("--selfplay-link-embedder",
                                           [](const std::string &algo) {
                                               ConfigManager::Instance()->setConfig(
                                                   algo, "MCVNE", "NodeEmbeddingAlgo",
                                                   "LinkEmbedder");
                                           })
        ->description("MCVNE link embedding algorithm for self-plays")
        ->default_val(ConfigManager::Instance()->getConfig<std::string>(
            "MCVNE", "NodeEmbeddingAlgo", "LinkEmbedder"))
        ->check(CLI::IsMember({"MCF", "BFS-SP"}));

    // [GRC]
    grc_exp
        ->add_option_function<double>(
            "--alpha",
            [](double alpha) { ConfigManager::Instance()->setConfig(alpha, "GRC", "alpha"); })
        ->description("GRC computing resources unit price")
        ->default_val(ConfigManager::Instance()->getConfig<double>("GRC", "alpha"))
        ->check(CLI::PositiveNumber);

    grc_exp
        ->add_option_function<double>(
            "--beta",
            [](double beta) { ConfigManager::Instance()->setConfig(beta, "GRC", "beta"); })
        ->description("GRC bandwidth resources unit price")
        ->default_val(ConfigManager::Instance()->getConfig<double>("GRC", "beta"))
        ->check(CLI::PositiveNumber);

    grc_exp
        ->add_option_function<double>(
            "--sigma",
            [](double sigma) { ConfigManager::Instance()->setConfig(sigma, "GRC", "sigma"); })
        ->description("GRC convergence threshold")
        ->default_val(ConfigManager::Instance()->getConfig<double>("GRC", "sigma"))
        ->check(CLI::PositiveNumber);

    grc_exp
        ->add_option_function<double>("--damping-factor",
                                      [](double factor) {
                                          ConfigManager::Instance()->setConfig(factor, "GRC",
                                                                               "dampingFactor");
                                      })
        ->description("GRC damping factor (0-1)")
        ->default_val(ConfigManager::Instance()->getConfig<double>("GRC", "dampingFactor"))
        ->check(CLI::Range(0.0, 1.0));

    CLI11_PARSE(app, prefix_app.remaining_for_passthrough(true));

    return 0;
}

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
#include "CLI11.hpp"

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/color.h"

using namespace vne;
using namespace vne::vineyard;
using namespace vne::utilities;
using namespace vne::nfg;
using namespace std;

namespace po = boost::program_options;

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

        //destroy all singleton classes to start fresh for next simulations
        ConfigManager::Destroy();
        IdGenerator::Destroy();
        RNG::Destroy();

    } else if (algo == "mcvne_sp") {
        vne::experiments::MCVNENodeBFSLinkExp<> exp = vne::experiments::MCVNENodeBFSLinkExp<>();

        std::string str = dbPath + "mcvne_sp_" + vnr_file + ".db";
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

    } else if (algo == "grc_mcf") {
        vne::experiments::GRCNodeMCFLinkExp<> exp = vne::experiments::GRCNodeMCFLinkExp<>();

        std::string str = dbPath + "grc_mcf_" + vnr_file + ".db";
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

    } else if (algo == "grc_sp") {
        vne::experiments::GRCNodeBFSLinkExp<> exp = vne::experiments::GRCNodeBFSLinkExp<>();

        std::string str = dbPath + "grc_sp_" + vnr_file + ".db";
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

    } else if (algo == "vineyard") {
        vne::experiments::VineNodeMCFLinkExp<> exp = vne::experiments::VineNodeMCFLinkExp<>();

        std::string str = dbPath + "vineyard_" + vnr_file + ".db";
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

class ColorFormatter : public CLI::Formatter
{
   private:
    std::string replaceCharWithTwo(const std::string &original, char target, char replacement1,
                                   char replacement2) const
    {
        std::string result;
        result.reserve(original.size() * 2);  // Pre-allocate memory for efficiency
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
    // std::string make_usage(const CLI::App *app, std::string name) const override
    // {
    //     std::string usage = CLI::Formatter::make_usage(app, name);
    //     return fmt::format(fg(fmt::color::deep_pink) | fmt::emphasis::italic, usage);
    // }
    std::string make_option_opts(const CLI::Option *opt) const override
    {
        std::string str = CLI::Formatter::make_option_opts(opt);
        str = replaceCharWithTwo(str, '{', '{', '{');
        str = replaceCharWithTwo(str, '}', '}', '}');

        if (opt->get_required()) {
            return fmt::format(fg(fmt::color::medium_spring_green) | fmt::emphasis::bold, str);
        } else {
            return fmt::format(fg(fmt::color::medium_spring_green), str);
        }
    }

    std::string make_option_name(const CLI::Option *opt, bool positional) const override
    {
        std::string str = CLI::Formatter::make_option_name(opt, positional);
        if (opt->get_required()) {
            return fmt::format(fg(fmt::color::light_sky_blue) | fmt::emphasis::bold, str);
        } else {
            return fmt::format(fg(fmt::color::light_sky_blue), str);
        }
    }

    std::string make_option_desc(const CLI::Option *opt) const override
    {
        std::string str = CLI::Formatter::make_option_desc(opt);
        return fmt::format(fg(fmt::color::khaki) | fmt::emphasis::italic, str);
    }
};

int main(int argc, char **argv)
{
    // std::cout << rang::fg::blue << "testing" << std::endl;
    CLI::App app{"VNE-SIM: A Virtual Network Embedding Simulator", "vnesim"};
    // Create and configure the formatter
    auto fmt = std::make_shared<ColorFormatter>();
    // auto fmt = app.get_formatter();
    fmt->column_width(20);  // Set line width
    fmt->right_column_width(120);
    fmt->label("REQUIRED",
               fmt::format(fg(fmt::color::crimson) | fmt::emphasis::bold, "[REQUIRED]"));
    // Assign the formatter to the app
    app.formatter(fmt);

    std::string link_embedding;

    // Create subcommands
    auto experiment = app.add_subcommand("experiment", "Run experiments");
    experiment->formatter(fmt);
    auto netgen = app.add_subcommand("netgen", "Generate network files");
    netgen->formatter(fmt);

    // Experiment subcommands
    auto vineyard_exp = experiment->add_subcommand("vineyard", "Vineyard experiment");
    vineyard_exp->formatter(fmt);

    auto grc_exp = experiment->add_subcommand("grc", "GRC experiment");
    grc_exp->formatter(fmt);

    grc_exp
        ->add_option("-l,--link-embedding-algo", link_embedding,
                     "Link embedding algo to use: Shortest Path or Multi-Commodity Flow")
        ->required()
        ->check(CLI::IsMember({"sp", "mcf"}));
    auto mcvne_exp = experiment->add_subcommand("mcvne", "MCVNE experiment");
    mcvne_exp->formatter(fmt);

    mcvne_exp
        ->add_option("-l,--link-embedding-algo", link_embedding,
                     "Link embedding algo to use: Shortest Path or Multi-Commodity Flow")
        ->required()
        ->check(CLI::IsMember({"sp", "mcf"}));

    // Netgen subcommands
    auto substrate_gen = netgen->add_subcommand("substrate", "Generate substrate network");
    auto vr_gen = netgen->add_subcommand("vr", "Generate virtual network requests");

    // Subcommand callbacks
    vineyard_exp->callback([]() { run_experiment("vineyard"); });
    grc_exp->callback([&]() {
        if (link_embedding == "sp") {
            run_experiment("grc_sp");
        } else {
            run_experiment("grc_mcf");
        }
    });
    mcvne_exp->callback([&]() {
        if (link_embedding == "sp") {
            run_experiment("mcvne_sp");
        } else {
            run_experiment("mcvne_mcf");
        }
    });

    substrate_gen->callback([]() {
        NetworkFileGenerator nfg;
        nfg.VYSubstrateNetFileGenerator(true);
    });
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
        ->check(CLI::IsMember({"debug", "info", "warning", "error", "fatal"}));

    app.add_option_function<std::string>("-p,--pythonPath",
                                         [](const std::string &path) {
                                             ConfigManager::Instance()->setConfig(
                                                 path, "utilities", "pythonPath");
                                         })
        ->description("Path to python interpreter for FNSS")
        ->check(CLI::ExistingFile);

    // [core]
    app.add_flag_function("-i,--ignoreLocationConstraint",
                          [](long ignore) {
                              ConfigManager::Instance()->setConfig(ignore > 0, "core",
                                                                   "ignoreLocationConstrain");
                          })
        ->description("Ignore location constraints during embedding");

    app.add_option_function<std::string>("-d,--dbPath",
                                         [](const std::string &path) {
                                             ConfigManager::Instance()->setConfig(path, "core",
                                                                                  "dbPath");
                                         })
        ->description("Path to store result databases")
        ->check(CLI::ExistingDirectory);

    app.add_option_function<int>(
           "-s,--seed",
           [](int seed) { ConfigManager::Instance()->setConfig(seed, "core", "rngSeed"); })
        ->description("Random number generator seed (0 for random)")
        ->check(CLI::NonNegativeNumber);

    app.add_flag_function("-S,--parallel-runs-identical-seeds",
                          [](long use) {
                              ConfigManager::Instance()->setConfig(
                                  use > 0, "core", "rngUseSameSeedForParallelRuns");
                          })
        ->description("Use same RNG seed across parallel runs");

    app.add_option_function<std::string>("-r,--rng-algo",
                                         [](const std::string &type) {
                                             ConfigManager::Instance()->setConfig(type, "core",
                                                                                  "rngType");
                                         })
        ->description("GSL RNG algorithm (e.g. gsl_rng_mt19937)");

    // Network generation options
    netgen
        ->add_option_function<std::string>("-H,--handler",
                                           [](const std::string &handler) {
                                               ConfigManager::Instance()->setConfig(
                                                   handler, "NetworkFileGenerator", "Handler");
                                           })
        ->description("Network generator handler (BRITE or FNSS)")
        ->check(CLI::IsMember({"BRITE", "FNSS"}))
        ->required();

    netgen
        ->add_option_function<std::string>("-d,--dest",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "NetworkFileGenerator", "DirToSaveFiles");
                                           })
        ->description("Directory to save generated network files")
        ->check(CLI::ExistingDirectory);

    netgen
        ->add_option_function<std::string>("-b,--brite-seed-file",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "NetworkFileGenerator", "BriteHandler",
                                                   "BriteSeedFile");
                                           })
        ->description("BRITE seed file path")
        ->check(CLI::ExistingFile);

    netgen
        ->add_option_function<int>("-p,--brite-node-placement",
                                   [](int placement) {
                                       ConfigManager::Instance()->setConfig(
                                           placement, "NetworkFileGenerator", "BriteHandler",
                                           "nodePlacement");
                                   })
        ->description("BRITE node placement (1:Random, 2:HeavyTailed)")
        ->check(CLI::Range(1, 2));

    netgen
        ->add_option_function<int>("-n,--brite-num-neighbors",
                                   [](int num) {
                                       ConfigManager::Instance()->setConfig(
                                           num, "NetworkFileGenerator", "BriteHandler",
                                           "numNeighbors");
                                   })
        ->description("BRITE neighbors per node")
        ->check(CLI::PositiveNumber);

    netgen
        ->add_option_function<int>("-i,--brite-inner-grid",
                                   [](int size) {
                                       ConfigManager::Instance()->setConfig(
                                           size, "NetworkFileGenerator", "BriteHandler",
                                           "innerGridSize");
                                   })
        ->description("BRITE inner grid size")
        ->check(CLI::PositiveNumber);

    netgen
        ->add_option_function<int>("-o,--brite-outer-grid",
                                   [](int size) {
                                       ConfigManager::Instance()->setConfig(
                                           size, "NetworkFileGenerator", "BriteHandler",
                                           "outerGridSize");
                                   })
        ->description("BRITE outer grid size")
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
        ->check(CLI::Range(1, 2));

    netgen
        ->add_option_function<double>("-A,--waxman-alpha",
                                      [](double alpha) {
                                          ConfigManager::Instance()->setConfig(
                                              alpha, "NetworkFileGenerator", "BriteHandler",
                                              "RTWaxman", "alpha");
                                      })
        ->description("Waxman alpha parameter (0-1)")
        ->check(CLI::Range(0.0, 1.0));

    netgen
        ->add_option_function<double>("-B,--waxman-beta",
                                      [](double beta) {
                                          ConfigManager::Instance()->setConfig(
                                              beta, "NetworkFileGenerator", "BriteHandler",
                                              "RTWaxman", "beta");
                                      })
        ->description("Waxman beta parameter (0-1)")
        ->check(CLI::Range(0.0, 1.0));

    substrate_gen
        ->add_option_function<std::string>("-t,--topology",
                                           [](const std::string &type) {
                                               ConfigManager::Instance()->setConfig(
                                                   type, "NetworkFileGenerator", "SNTopologyType");
                                           })
        ->description("Substrate network topology type")
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
        ->check(CLI::PositiveNumber);

    vr_gen
        ->add_option_function<std::string>("-t,--topology",
                                           [](const std::string &type) {
                                               ConfigManager::Instance()->setConfig(
                                                   type, "NetworkFileGenerator", "VNTopologyType");
                                           })
        ->description("Virtual network topology type")
        ->check(CLI::IsMember({"Waxman", "Barabasi"}))
        ->required();

    vr_gen
        ->add_option_function<int>("-T,--total-time",
                                   [](int time) {
                                       ConfigManager::Instance()->setConfig(
                                           time, "NetworkFileGenerator", "TotalTime");
                                   })
        ->description("Total simulation time in an arbitrary unit.")
        ->check(CLI::PositiveNumber);

    vr_gen
        ->add_option_function<double>("-r,--link-splitting-rate",
                                      [](double rate) {
                                          ConfigManager::Instance()->setConfig(
                                              rate, "NetworkFileGenerator",
                                              "VNRLinkSplittingRate");
                                      })
        ->description("VNR link splitting probability (0-1)")
        ->check(CLI::Range(0.0, 1.0));

    // Distribution parameters
    auto add_dist_param = [&](CLI::App *sub_cmd, const std::string &name,
                              const std::string &desc) {
        sub_cmd
            ->add_option_function<int>("--" + name + "-dist",
                                       [name](int dist) {
                                           ConfigManager::Instance()->setConfig(
                                               dist, "NetworkFileGenerator", name + "Dist");
                                       })
            ->description(desc + " distribution (0:Uniform, 1:Exponential, 2:Poisson)")
            ->check(CLI::Range(0, 2));

        sub_cmd
            ->add_option_function<double>("--" + name + "-p1",
                                          [name](double param) {
                                              ConfigManager::Instance()->setConfig(
                                                  param, "NetworkFileGenerator",
                                                  name + "DistParam1");
                                          })
            ->description(desc + " distribution parameter 1");

        sub_cmd
            ->add_option_function<double>("--" + name + "-p2",
                                          [name](double param) {
                                              ConfigManager::Instance()->setConfig(
                                                  param, "NetworkFileGenerator",
                                                  name + "DistParam2");
                                          })
            ->description(desc + " distribution parameter 2");

        sub_cmd
            ->add_option_function<double>("--" + name + "-p3",
                                          [name](double param) {
                                              ConfigManager::Instance()->setConfig(
                                                  param, "NetworkFileGenerator",
                                                  name + "DistParam3");
                                          })
            ->description(desc + " distribution parameter 3");
    };

    add_dist_param(substrate_gen, "sn-cpu", "Substrate node CPU distribution");
    add_dist_param(substrate_gen, "sl-bw", "Substrate link bandwidth distribution");
    add_dist_param(substrate_gen, "sl-delay", "Substrate link delay distribution");

    add_dist_param(vr_gen, "vnr-num-nodes", "VNR number of nodes distribution");
    add_dist_param(vr_gen, "vnr-duration", "VNR duration distribution");
    add_dist_param(vr_gen, "vnr-arrival", "VNR arrival time distribution");
    add_dist_param(vr_gen, "vnr-max-distance", "VNR max distance distribution");
    add_dist_param(vr_gen, "vn-cpu", "Virtual node CPU distribution");
    add_dist_param(vr_gen, "vl-bw", "Virtual link bandwidth distribution");
    add_dist_param(vr_gen, "vl-delay", "Virtual link delay distribution");

    // DCNBCube
    substrate_gen
        ->add_option_function<int>("--bcube-n",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNBCube",
                                           "N");
                                   })
        ->description("Hosts per Bcube_0")
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--bcube-k",
                                   [](int k) {
                                       ConfigManager::Instance()->setConfig(
                                           k, "NetworkFileGenerator", "FNSSHandler", "DCNBCube",
                                           "K");
                                   })
        ->description("Bcube level")
        ->check(CLI::PositiveNumber);

    // DCNTwoTier
    substrate_gen
        ->add_option_function<int>("--twotier-n-cores",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_cores");
                                   })
        ->description("Number of core switches")
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--twotier-n-edges",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_edges");
                                   })
        ->description("Number of edges per switch")
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--twotier-n-hosts",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "n_hosts");
                                   })
        ->description("Number of hosts per switch")
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--twotier-bw-multiplier",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNTwoTier",
                                           "coreBWMultiplier");
                                   })
        ->description("Core links bandwidth multiplier")
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
        ->check(CLI::PositiveNumber);

    substrate_gen
        ->add_option_function<int>("--fattree-bw-multiplier",
                                   [](int n) {
                                       ConfigManager::Instance()->setConfig(
                                           n, "NetworkFileGenerator", "FNSSHandler", "DCNFatTree",
                                           "coreBWMultiplier");
                                   })
        ->description("Core links bandwidth multiplier")
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
        ->check(CLI::PositiveNumber);

    // [vineyard.SubstrateNetwork]
    experiment
        ->add_option_function<std::string>("-p,--substrate-path",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "SubstrateNetwork", "path");
                                           })
        ->description("Substrate network files directory")
        ->check(CLI::ExistingDirectory);

    experiment
        ->add_option_function<std::string>("-f,--substreate-filename",
                                           [](const std::string &name) {
                                               ConfigManager::Instance()->setConfig(
                                                   name, "vineyard", "SubstrateNetwork",
                                                   "filename");
                                           })
        ->description("Substrate network filename");

    // [vineyard.VirtualNetRequest]
    experiment
        ->add_option_function<std::string>("-r,--vnr-root-path",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "VirtualNetRequest", "path");
                                           })
        ->description("Virtual network requests root directory")
        ->check(CLI::ExistingDirectory);

    experiment
        ->add_option_function<std::string>("-d,--vnr-dir-name",
                                           [](const std::string &dir) {
                                               ConfigManager::Instance()->setConfig(
                                                   dir, "vineyard", "VirtualNetRequest", "dir");
                                           })
        ->description("Virtual network requests directory name");

    experiment
        ->add_option_function<std::string>("-e,--req-file-ext",
                                           [](const std::string &ext) {
                                               ConfigManager::Instance()->setConfig(
                                                   ext, "vineyard", "VirtualNetRequest",
                                                   "reqfileExtension");
                                           })
        ->description("Virtual network request file extension (txt)");

    // [vineyard.Constants]
    experiment
        ->add_option_function<double>("-m,--revenue-multiplier",
                                      [](double mult) {
                                          ConfigManager::Instance()->setConfig(
                                              mult, "vineyard", "Constants", "revenueMultiplier");
                                      })
        ->description("Revenue calculation multiplier");

    experiment
        ->add_option_function<double>("-M,--cost-multiplier",
                                      [](double mult) {
                                          ConfigManager::Instance()->setConfig(
                                              mult, "vineyard", "Constants", "costMultiplier");
                                      })
        ->description("Cost calculation multiplier");

    // [vineyard.Configs]
    vineyard_exp
        ->add_option_function<std::string>("-t,--node-map-type",
                                           [](const std::string &type) {
                                               ConfigManager::Instance()->setConfig(
                                                   type, "vineyard", "Configs", "nodeMappingType");
                                           })
        ->required()
        ->description("Vineyard node mapping type")
        ->check(CLI::IsMember({"deterministic", "randomized"}));

    vineyard_exp
        ->add_option_function<double>("--epsilon",
                                      [](double eps) {
                                          ConfigManager::Instance()->setConfig(
                                              eps, "vineyard", "Constants", "epsilon");
                                      })
        ->description("Numerical comparison epsilon")
        ->check(CLI::PositiveNumber);

    vineyard_exp
        ->add_flag_function("--set-alpha",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(set > 0, "vineyard",
                                                                     "Configs", "setAlpha");
                            })
        ->description("Enable alpha parameter in Vineyard");

    vineyard_exp
        ->add_flag_function("--set-beta",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(set > 0, "vineyard",
                                                                     "Configs", "setBeta");
                            })
        ->description("Enable beta parameter in Vineyard");

    // [vineyard.glpk]
    vineyard_exp
        ->add_option_function<std::string>("--lp-model",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "LPmodelFile");
                                           })
        ->description("GLPK LP model file")
        ->check(CLI::ExistingFile);

    vineyard_exp
        ->add_option_function<std::string>("--lp-data",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "LPdataFile");
                                           })
        ->description("GLPK LP data file")
        ->check(CLI::ExistingFile);

    experiment
        ->add_option_function<std::string>("-o,--mcf-model",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "MCFmodelFile");
                                           })
        ->description("GLPK MCF model file")
        ->check(CLI::ExistingFile);

    experiment
        ->add_option_function<std::string>("-O,--mcf-data",
                                           [](const std::string &path) {
                                               ConfigManager::Instance()->setConfig(
                                                   path, "vineyard", "glpk", "MCFdataFile");
                                           })
        ->description("GLPK MCF data file")
        ->check(CLI::ExistingFile);

    experiment
        ->add_flag_function("-E,--enable-glpk-terminal",
                            [](long enabled) {
                                ConfigManager::Instance()->setConfig(enabled > 0, "vineyard",
                                                                     "glpk", "terminalEnabled");
                            })
        ->description("Enable GLPK terminal output");

    // [MCTS.Simulator.Knowledge]
    mcvne_exp
        ->add_option_function<int>("--tree-level",
                                   [](int level) {
                                       ConfigManager::Instance()->setConfig(
                                           level, "MCTS", "Simulator", "Knowledge", "TreeLevel");
                                   })
        ->description("MCTS tree knowledge level (0:PURE, 1:LEGAL, 2:SMART)")
        ->check(CLI::Range(0, 2));

    mcvne_exp
        ->add_option_function<int>("--rollout-level",
                                   [](int level) {
                                       ConfigManager::Instance()->setConfig(
                                           level, "MCTS", "Simulator", "Knowledge",
                                           "RolloutLevel");
                                   })
        ->description("MCTS rollout knowledge level (0:PURE, 1:LEGAL, 2:SMART)")
        ->check(CLI::Range(0, 2));

    mcvne_exp
        ->add_option_function<int>("--smart-tree-count",
                                   [](int count) {
                                       ConfigManager::Instance()->setConfig(
                                           count, "MCTS", "Simulator", "Knowledge",
                                           "SmartTreeCount");
                                   })
        ->description("MCTS smart tree count")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<double>("--smart-tree-value",
                                      [](double value) {
                                          ConfigManager::Instance()->setConfig(
                                              value, "MCTS", "Simulator", "Knowledge",
                                              "SmartTreeValue");
                                      })
        ->description("MCTS smart tree value")
        ->check(CLI::PositiveNumber);

    // [MCTS.Simulator]
    mcvne_exp
        ->add_option_function<double>("--discount",
                                      [](double discount) {
                                          ConfigManager::Instance()->setConfig(
                                              discount, "MCTS", "Simulator", "discount");
                                      })
        ->description("MCTS discount factor (0-1]")
        ->check(CLI::Range(0.0, 1.0));

    mcvne_exp
        ->add_option_function<int>("--reward-range",
                                   [](int range) {
                                       ConfigManager::Instance()->setConfig(
                                           range, "MCTS", "Simulator", "rewardRange");
                                   })
        ->description("MCTS reward range")
        ->check(CLI::PositiveNumber);

    // [MCTS.MCTSParameters]
    mcvne_exp
        ->add_option_function<int>("--max-depth",
                                   [](int depth) {
                                       ConfigManager::Instance()->setConfig(
                                           depth, "MCTS", "MCTSParameters", "MaxDepth");
                                   })
        ->description("MCTS maximum search depth")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<int>("--num-simulations",
                                   [](int num) {
                                       ConfigManager::Instance()->setConfig(
                                           num, "MCTS", "MCTSParameters", "NumSimulations");
                                   })
        ->description("MCTS number of simulations (self-plays) to generate a rollout")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<int>("--expand-count",
                                   [](int count) {
                                       ConfigManager::Instance()->setConfig(
                                           count, "MCTS", "MCTSParameters", "ExpandCount");
                                   })
        ->description("MCTS nodes to expand per step")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_flag_function("--auto-exploration",
                            [](long autoexp) {
                                ConfigManager::Instance()->setConfig(
                                    autoexp > 0, "MCTS", "MCTSParameters", "AutoExploration");
                            })
        ->description("Enable MCTS automatic exploration");

    mcvne_exp
        ->add_option_function<double>("--exploration-constant",
                                      [](double c) {
                                          ConfigManager::Instance()->setConfig(
                                              c, "MCTS", "MCTSParameters", "ExplorationConstant");
                                      })
        ->description("MCTS exploration constant")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_flag_function("--rave",
                            [](long use) {
                                ConfigManager::Instance()->setConfig(use > 0, "MCTS",
                                                                     "MCTSParameters", "UseRave");
                            })
        ->description("Enable MCTS RAVE");

    mcvne_exp
        ->add_option_function<double>("--rave-discount",
                                      [](double discount) {
                                          ConfigManager::Instance()->setConfig(
                                              discount, "MCTS", "MCTSParameters", "RaveDiscount");
                                      })
        ->description("MCTS RAVE discount factor")
        ->check(CLI::Range(0.0, 1.0));

    mcvne_exp
        ->add_option_function<double>("--rave-constant",
                                      [](double c) {
                                          ConfigManager::Instance()->setConfig(
                                              c, "MCTS", "MCTSParameters", "RaveConstant");
                                      })
        ->description("MCTS RAVE constant")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_flag_function("--disable-tree",
                            [](long disable) {
                                ConfigManager::Instance()->setConfig(
                                    disable > 0, "MCTS", "MCTSParameters", "DisableTree");
                            })
        ->description("Disable MCTS tree (use rollout only)");

    mcvne_exp
        ->add_flag_function("--single-player-mcts",
                            [](long use) {
                                ConfigManager::Instance()->setConfig(
                                    use > 0, "MCTS", "MCTSParameters", "UseSinglePlayerMCTS");
                            })
        ->description("Enable single-player MCTS");

    mcvne_exp
        ->add_option_function<int>("--spmcts-constant",
                                   [](int c) {
                                       ConfigManager::Instance()->setConfig(
                                           c, "MCTS", "MCTSParameters", "SPMCTSConstant");
                                   })
        ->description("Single-player MCTS constant")
        ->check(CLI::PositiveNumber);

    mcvne_exp
        ->add_option_function<int>("--parallelization",
                                   [](int type) {
                                       ConfigManager::Instance()->setConfig(
                                           type, "MCTS", "MCTSParameters", "ParallelizationType");
                                   })
        ->description("MCTS parallelization type (0:ActionRoot, 1:FullTree)")
        ->check(CLI::Range(0, 1));

    // [MCVNE.VNEMCTSSimulator]
    mcvne_exp
        ->add_flag_function("--set-alpha",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(
                                    set > 0, "MCVNE", "VNEMCTSSimulator", "setAlpha");
                            })
        ->description("Enable MCVNE substrate links weight (alpha)");

    mcvne_exp
        ->add_flag_function("--set-beta",
                            [](long set) {
                                ConfigManager::Instance()->setConfig(
                                    set > 0, "MCVNE", "VNEMCTSSimulator", "setBeta");
                            })
        ->description("Enable MCVNE substrate node weight (beta)");

    // [MCVNE.NodeEmbeddingAlgo]
    mcvne_exp
        ->add_option_function<std::string>("--selfplay-link-embedder",
                                           [](const std::string &algo) {
                                               ConfigManager::Instance()->setConfig(
                                                   algo, "MCVNE", "NodeEmbeddingAlgo",
                                                   "LinkEmbedder");
                                           })
        ->description("MCVNE link embedding algorithm for self-plays")
        ->check(CLI::IsMember({"MCF", "BFS-SP"}));

    // [GRC]
    grc_exp
        ->add_option_function<double>(
            "--alpha",
            [](double alpha) { ConfigManager::Instance()->setConfig(alpha, "GRC", "alpha"); })
        ->description("GRC computing resources unit price")
        ->check(CLI::PositiveNumber);

    grc_exp
        ->add_option_function<double>(
            "--beta",
            [](double beta) { ConfigManager::Instance()->setConfig(beta, "GRC", "beta"); })
        ->description("GRC bandwidth resources unit price")
        ->check(CLI::PositiveNumber);

    grc_exp
        ->add_option_function<double>(
            "--sigma",
            [](double sigma) { ConfigManager::Instance()->setConfig(sigma, "GRC", "sigma"); })
        ->description("GRC convergence threshold")
        ->check(CLI::PositiveNumber);

    grc_exp
        ->add_option_function<double>("--damping-factor",
                                      [](double factor) {
                                          ConfigManager::Instance()->setConfig(factor, "GRC",
                                                                               "dampingFactor");
                                      })
        ->description("GRC damping factor (0-1)")
        ->check(CLI::Range(0.0, 1.0));

    CLI11_PARSE(app, argc, argv);
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << ConfigManager::Instance()->getConfig<int>("core", "rngSeed") << std::endl;

    return 0;
}

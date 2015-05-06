/**
 * @file brite-handler.cc
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
/****************************************************************************/
/*                  Copyright 2001, Trustees of Boston University.          */
/*                               All Rights Reserved.                       */
/*                                                                          */
/* Permission to use, copy, or modify this software and its documentation   */
/* for educational and research purposes only and without fee is hereby     */
/* granted, provided that this copyright notice appear on all copies and    */
/* supporting documentation.  For any other uses of this software, in       */
/* original or modified form, including but not limited to distribution in  */
/* whole or in part, specific prior permission must be obtained from Boston */
/* University.  These programs shall not be used, rewritten, or adapted as  */
/* the basis of a commercial software or hardware product without first     */
/* obtaining appropriate licenses from Boston University.  Boston University*/
/* and the author(s) make no representations about the suitability of this  */
/* software for any purpose.  It is provided "as is" without express or     */
/* implied warranty.                                                        */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*  Author:     Alberto Medina                                              */
/*              Anukool Lakhina                                             */
/*  Title:     BRITE: Boston university Representative Topology gEnerator   */
/*  Revision:  2.0         4/02/2001                                        */
/****************************************************************************/

#include "brite-handler.h"



namespace vne {
    namespace nfg {
        
        std::shared_ptr<BriteHandler> BriteHandler::_instance = nullptr;
        std::shared_ptr<BriteHandler> BriteHandler::Instance ()
        {
            if (_instance==nullptr) {
                _instance = std::shared_ptr<BriteHandler> (new BriteHandler());
            }
            return _instance;
        }
        void BriteHandler::InitSeeds() {
            
            ofstream last_seed_file;
            
            /* Create Parse object */
            Parse p((char*) params.BriteSeedFile.c_str());
            
            /* Parse configuration file */
            p.ParseSeed((char*) "PLACES", Model::s_places);
            p.ParseSeed((char*) "CONNECT", Model::s_connect);
            p.ParseSeed((char*) "EDGE_CONN", Model::s_edgeconn);
            p.ParseSeed((char*) "GROUPING", Model::s_grouping);
            p.ParseSeed((char*) "ASSIGNMENT", Model::s_assignment);
            p.ParseSeed((char*) "BANDWIDTH", Model::s_bandwidth);
            
            BOOST_LOG_TRIVIAL(info) << "Place seed used: "
            << Model::s_places[0] << " "
            << Model::s_places[1] << " "
            << Model::s_places[2] << "\n";
            BOOST_LOG_TRIVIAL(info) << "Connect seed used: "
            << Model::s_connect[0] << " "
            << Model::s_connect[1] << " "
            << Model::s_connect[2] << "\n";
            BOOST_LOG_TRIVIAL(info) << "Edge conn seed used: "
            << Model::s_edgeconn[0] << " "
            << Model::s_edgeconn[1] << " "
            << Model::s_edgeconn[2] << "\n";
            BOOST_LOG_TRIVIAL(info) << "Grouping seed used: "
            << Model::s_grouping[0] << " "
            << Model::s_grouping[1] << " "
            << Model::s_grouping[2] << "\n";
            BOOST_LOG_TRIVIAL(info) << "Assigment seed used: "
            << Model::s_assignment[0] << " "
            << Model::s_assignment[1] << " "
            << Model::s_assignment[2] << "\n";
            BOOST_LOG_TRIVIAL(info) << "Bandwidth seed used: "
            << Model::s_bandwidth[0] << " "
            << Model::s_bandwidth[1] << " "
            << Model::s_bandwidth[2] << "\n" << std::endl;
            
            
            last_seed_file.open("last_seed_file", ios::out);
            
            if (last_seed_file.fail()) {
                cerr << "Cannot open seed files for input/output...\n";
                exit(0);
            }
            
            last_seed_file << "PLACES"
            << " " << Model::s_places[0]
            << " " << Model::s_places[1]
            << " " << Model::s_places[2] << "\n";
            
            last_seed_file << "CONNECT"
            << " " << Model::s_connect[0]
            << " " << Model::s_connect[1]
            << " " << Model::s_connect[2] << "\n";
            
            last_seed_file << "EDGE_CONN"
            << " " << Model::s_edgeconn[0]
            << " " << Model::s_edgeconn[1]
            << " " << Model::s_edgeconn[2] << "\n";
            
            last_seed_file << "GROUPING"
            << " " << Model::s_grouping[0] 
            << " " << Model::s_grouping[1] 
            << " " << Model::s_grouping[2] << "\n";
            
            last_seed_file << "ASSIGNMENT"
            << " " << Model::s_assignment[0] 
            << " " << Model::s_assignment[1] 
            << " " << Model::s_assignment[2] << "\n";
            
            last_seed_file << "BANDWIDTH"
            << " " << Model::s_bandwidth[0] 
            << " " << Model::s_bandwidth[1] 
            << " " << Model::s_bandwidth[2] << "\n";
            
            last_seed_file.close();
            
        }
        
        BriteHandler::Parameters::Parameters() :
        BriteSeedFile (ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.BriteHandler.BriteSeedFile")),
        nodePlacement(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.BriteHandler.nodePlacement")),
        numNeighbors(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.BriteHandler.numNeighbors")),
        innerGridSize(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.BriteHandler.innerGridSize")),
        outerGridSize(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.BriteHandler.outerGridSize")),
        rtWaxman (RTWaxman())
        {
            pt.put ("nodePlacement", nodePlacement);
            pt.put ("numNeighbors", numNeighbors);
            pt.put ("innerGridSize", innerGridSize);
            pt.put ("outerGridSize", outerGridSize);
            pt.put ("nodePlacement", nodePlacement);
            pt.put ("RTWaxman.growthType", rtWaxman.growthType);
            pt.put ("RTWaxman.alpha", rtWaxman.alpha);
            pt.put ("RTWaxman.beta", rtWaxman.beta);
        }
        
        BriteHandler::Parameters::RTWaxman::RTWaxman () :
        growthType(ConfigManager::Instance()->getConfig<int>("NetworkFileGenerator.BriteHandler.RTWaxman.growthType")),
        alpha(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.BriteHandler.RTWaxman.alpha")),
        beta(ConfigManager::Instance()->getConfig<double>("NetworkFileGenerator.BriteHandler.RTWaxman.beta"))
        {
        }
        
        BriteHandler::BriteHandler () :
        params(Parameters())
        {
            InitSeeds();
        }
        const BriteHandler::Parameters&
        BriteHandler::getParams() const
        {
            return params;
        }
        
/*
        void BriteHandler::ParseModel()
        {
            std::shared_ptr<Topology> topology;
            std::shared_ptr<RouterWaxman> rt_wax_model;
            std::shared_ptr<RouterBarabasiAlbert_1> rt_bar_1_model;
            std::shared_ptr<RouterBarabasiAlbert_2> rt_bar_2_model;
            std::shared_ptr<RouterGLP> rt_glp_model;
            std::shared_ptr<ASWaxman> as_wax_model;
            std::shared_ptr<ASBarabasiAlbert_1> as_bar_1_model;
            std::shared_ptr<ASBarabasiAlbert_2> as_bar_2_model;
            std::shared_ptr<ASGLP> as_glp_model;
            std::shared_ptr<TopDownHierModel> td_model;
            std::shared_ptr<BottomUpHierModel> bu_model;
            std::shared_ptr<ImportedBriteTopologyModel> if_brite_model;
            std::shared_ptr<ImportedGTitmTopologyModel> if_gtitm_model;
            std::shared_ptr<ImportedNLANRTopologyModel> if_nlanr_model;
            std::shared_ptr<ImportedInetTopologyModel> if_inet_model;
            
            ModelPar* par;
            
            
            
            // Create Parse object
            Parse p((char*) ConfigManager::Instance()->getConfig<std::string>("NetworkFileGenerator.BriteHandler.BriteConfigFile").c_str());
            
            //  Parse configuration file
            par = p.ParseConfigFile();
            assert(par != NULL);
            
            switch (par->GetModelType()) {
                case RT_WAXMAN:
                    rt_wax_model =  std::shared_ptr<RouterWaxman> (new RouterWaxman((RouterWaxPar*)par));
                    topology = std::shared_ptr<Topology> (new Topology(rt_wax_model.get()));
                    break;
                    
                case RT_BARABASI_1:
                    rt_bar_1_model =
                        std::shared_ptr<RouterBarabasiAlbert_1> (new RouterBarabasiAlbert_1((RouterBarabasiAlbert_1_Par*)par));
                    topology = std::shared_ptr<Topology> (new Topology(rt_bar_1_model.get()));
                    break;
                    
                case RT_BARABASI_2:
                    rt_bar_2_model =
                        std::shared_ptr<RouterBarabasiAlbert_2> (new RouterBarabasiAlbert_2((RouterBarabasiAlbert_2_Par*)par));
                    topology = std::shared_ptr<Topology> (new Topology(rt_bar_2_model.get()));
                    break;
                    
                case RT_GLP:
                    rt_glp_model = std::shared_ptr<RouterGLP> (new RouterGLP((RouterGLPPar*)par));
                    topology = std::shared_ptr<Topology> (new Topology(rt_glp_model.get()));
                    break;
                    
                case AS_WAXMAN:
                    as_wax_model = std::shared_ptr<ASWaxman> (new ASWaxman((ASWaxPar*)par));
                    topology = std::shared_ptr<Topology> (new Topology(rt_glp_model.get()));
                    break;
                    
                case AS_BARABASI_1:
                    as_bar_1_model =
                        std::shared_ptr<ASBarabasiAlbert_1> (new ASBarabasiAlbert_1((ASBarabasiAlbert_1_Par*)par));
                    topology = std::shared_ptr<Topology> (new Topology(as_bar_1_model.get()));
                    break;
                    
                case AS_BARABASI_2:
                    as_bar_2_model =
                        std::shared_ptr<ASBarabasiAlbert_2> (new ASBarabasiAlbert_2((ASBarabasiAlbert_2_Par*)par));
                    topology = std::shared_ptr<Topology> (new Topology(as_bar_2_model.get()));
                    break;
                    
                case AS_GLP:
                    as_glp_model =
                        std::shared_ptr<ASGLP> (new ASGLP((ASGLPPar*)par));
                    topology = std::shared_ptr<Topology> (new Topology(as_glp_model.get()));
                    break;
                    
                case TD_HIER:
                    td_model =
                        std::shared_ptr<TopDownHierModel> (new TopDownHierModel((TopDownPar*)par));
                    topology = std::shared_ptr<Topology> (new Topology(td_model.get()));
                    break;
                    
                case BU_HIER:
                    bu_model =
                        std::shared_ptr<BottomUpHierModel> (new BottomUpHierModel((BottUpPar*)par));
                    topology = std::shared_ptr<Topology> (new Topology(as_glp_model.get()));
                    break;
                    
                case IF_ROUTER:
                case IF_AS:
                    
                    switch (((ImportedFilePar*)par)->GetFormat()) {
                        case ImportedFileModel::IF_BRITE:
                            BOOST_LOG_TRIVIAL(info) << "Importing brite..." << std::endl;
                            if_brite_model =
                                std::shared_ptr<ImportedBriteTopologyModel> (new ImportedBriteTopologyModel((ImportedFilePar*)par));
                            topology = std::shared_ptr<Topology> (new Topology(if_brite_model.get()));
                            break;
                            
                        case ImportedFileModel::IF_GTITM:
                        case ImportedFileModel::IF_GTITM_TS:
                            BOOST_LOG_TRIVIAL(info) << "Importing gtitm..." << std::endl;
                            if_gtitm_model =
                                std::shared_ptr<ImportedGTitmTopologyModel> (new ImportedGTitmTopologyModel((ImportedFilePar*)par));
                            topology = std::shared_ptr<Topology> (new Topology(if_gtitm_model.get()));
                            break;
                            
                        case ImportedFileModel::IF_NLANR:
                            BOOST_LOG_TRIVIAL(info) << "Importing nlanr.." << std::endl;
                            if_nlanr_model =
                                std::shared_ptr<ImportedNLANRTopologyModel> (new ImportedNLANRTopologyModel((ImportedFilePar*)par));
                            topology = std::shared_ptr<Topology> (new Topology(if_nlanr_model.get()));
                            break;
                            
                        case ImportedFileModel::IF_INET:
                            BOOST_LOG_TRIVIAL(info) << "Importing Inet.." << std::endl;
                            if_inet_model =
                                std::shared_ptr<ImportedInetTopologyModel> (new ImportedInetTopologyModel((ImportedFilePar*)par));
                            topology = std::shared_ptr<Topology> (new Topology(if_inet_model.get()));
                            break;
                            
                        case ImportedFileModel::IF_SKITTER:
                        default:
                            BOOST_LOG_TRIVIAL(error) << "Invalid file format for ImportedFileModel..." << std::endl;
                            exit(0);
                    }
                    break;
                    
                default:
                    BOOST_LOG_TRIVIAL(error) << "Parsing error: invalid parameter structure returned..." << std::endl;
                    exit(0);
            }
            
            
            
            // Output topology into file(s)
            if (p.OutputBrite()) {
                cout << "Outputing topology into BRITE's format...\n" << flush;
                //topology->BriteOutput(argv[2]);
            }
            
            
            //delete topology;
            
            //m.OutputSeeds(argv[3]);
        }
*/
    }
}
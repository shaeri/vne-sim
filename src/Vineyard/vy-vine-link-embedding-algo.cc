/**
 * @file vy-vine-link-embedding-algo.cc
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

#include "vy-vine-link-embedding-algo.h"
#include "core/config-manager.h"

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

namespace vne {
    namespace vineyard{
        template<>
        VYVineLinkEmbeddingAlgo<>::VYVineLinkEmbeddingAlgo () :
        LinkEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> > (Link_Embedding_Algo_Types::WITH_PATH_SPLITTING),
        lp_problem(nullptr),
        substrateLinkIdSet(nullptr),
        substrateNodeIdSet(nullptr)
        {
            glp_term_out(ConfigManager::Instance()->getConfig<int>("vineyard", "glpk", "terminalEnabled"));
            
            MCFdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "MCFdataFile");
            MCFmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "MCFmodelFile");
#ifdef ENABLE_MPI
            int my_rank = MPI::COMM_WORLD.Get_rank();
            std::stringstream strm;
            strm << "-" << my_rank;
            MCFdataFile.append(strm.str());
#endif
            setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setBeta");
        }
        template<>
        VYVineLinkEmbeddingAlgo<>::~VYVineLinkEmbeddingAlgo()
        {
            if (lp_problem != NULL)
            {
                glp_delete_prob(lp_problem);
            }
        }
        
        template<>
        inline void VYVineLinkEmbeddingAlgo<>::writeDataFile
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr, const std::map<int,int>* nodeIdMap)
        {
            int substrateNodesNum = substrate_network->getNumNodes();
            
            // coeeficent vector (b)
            std::vector<std::vector<double>> bVec ((substrateNodesNum), std::vector<double>(substrateNodesNum));
            
            FILE *mcfFpDat = fopen(MCFdataFile.c_str(), "w");
            fprintf(mcfFpDat, "data;\n\n");
            
            //Substrate Nodes
            fprintf(mcfFpDat, "set N:=");
            for (int i = 0; i < substrateNodesNum; i++) {
                fprintf(mcfFpDat, " %d", i);
            }
            fprintf(mcfFpDat, ";\n");
            
            // Virtual edges
            fprintf(mcfFpDat, "set F:=");
            
            for (int i = 0; i < vnr->getVN()->getNumLinks(); i++) {
                fprintf(mcfFpDat, " f%d", i);
            }
            fprintf(mcfFpDat, ";\n\n");
            
            fprintf(mcfFpDat, "param b:\n");
            for (int i = 0; i < substrateNodesNum; i++) {
                fprintf(mcfFpDat, "%d ", i);
            }
            fprintf(mcfFpDat, ":=\n");
            for (int i = 0; i < substrateNodesNum; i++)
            {
                fprintf(mcfFpDat, "%d ", i);
                for (int j = 0; j < substrateNodesNum; j++)
                {
                    if (i==j || substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])==nullptr)
                    {
                        fprintf(mcfFpDat, "%.4lf ", 0.0);
                        bVec[i][j] = 0.0;
                    }
                    else
                    {
                        double bw = substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])->getBandwidth();
                        fprintf(mcfFpDat, "%.4lf ", bw);
                        bVec[i][j] = bw;
                    }
                }
                fprintf(mcfFpDat, "\n");
            }
            fprintf(mcfFpDat, ";\n\n");
            
            fprintf(mcfFpDat, "param alpha:\n");
            for (int i = 0; i < substrateNodesNum; i++)
                fprintf(mcfFpDat, "%d ", i);
            fprintf(mcfFpDat, ":=\n");
            
            //alpha parameter
            if (setAlpha == true)
            {
                for (int i = 0; i < substrateNodesNum; i++) {
                    fprintf(mcfFpDat, "%d ", i);
                    for (int j = 0; j < substrateNodesNum; j++) {
                        fprintf(mcfFpDat, "%.4lf ", bVec[i][j]); }
                    fprintf(mcfFpDat, "\n");
                }
            }
            else
            {
                for (int i = 0; i < substrateNodesNum; i++) {
                    fprintf(mcfFpDat, "%d ", i);
                    for (int j = 0; j < substrateNodesNum; j++)
                        fprintf(mcfFpDat, "1 ");
                    fprintf(mcfFpDat, "\n");
                }
            }
            fprintf(mcfFpDat, ";\n\n");
            
            // flow source : fs
            int count = 0;
            fprintf(mcfFpDat, "param fs:=\n");
            
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++)
            {
                int vNodeFromId = vnr->getVN()->getLink(*it)->getNodeFromId();
                int sNodeFromId;
                if(nodeIdMap == nullptr)
                    sNodeFromId = vnr->getNodeMap()->find(vNodeFromId)->second;
                else
                    sNodeFromId = nodeIdMap->find(vNodeFromId)->second;
                auto flowSource = std::distance(substrateNodeIdSet->begin(), substrateNodeIdSet->find(sNodeFromId));
                fprintf(mcfFpDat, "f%d %d\n", count, (int)flowSource);
                count++;
            }
            fprintf(mcfFpDat, ";\n\n");
            
            // flow destination
            count=0;
            fprintf(mcfFpDat, "param fe:=\n");
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                int vNodeToId = vnr->getVN()->getLink(*it)->getNodeToId();
                int sNodeToId;
                if (nodeIdMap == nullptr)
                    sNodeToId = vnr->getNodeMap()->find(vNodeToId)->second;
                else
                    sNodeToId = nodeIdMap->find(vNodeToId)->second;
                
                auto flowDestination = std::distance(substrateNodeIdSet->begin(), substrateNodeIdSet->find(sNodeToId));
                fprintf(mcfFpDat, "f%d %d\n", count, (int)flowDestination);
                count++;
            }
            fprintf(mcfFpDat, ";\n\n");
            
            // flow requirements
            count = 0;
            fprintf(mcfFpDat, "param fd:=\n");
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                fprintf(mcfFpDat, "f%d %.4lf\n", count, vnr->getVN()->getLink(*it)->getBandwidth());
                count++;
            }
            fprintf(mcfFpDat, ";\n\n");
            
            fprintf(mcfFpDat, "end;\n\n");
            
            fclose(mcfFpDat);
        }
        
        template<>
        inline int VYVineLinkEmbeddingAlgo<>::solveLinkMappingLP
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            if (lp_problem == nullptr) {
                lp_problem = glp_create_prob();
            }
            else
                glp_erase_prob(lp_problem);
            int ret;
            glp_smcp param;
            glp_init_smcp(&param);
            //turn on the preprocessor
            param.presolve = GLP_ON;
            
            glp_tran *tran;
            tran = glp_mpl_alloc_wksp();
            ret = glp_mpl_read_model(tran, MCFmodelFile.c_str(), 0);
            if (ret != 0)
            {
                std::cerr << "Error on translating model\n";
                glp_mpl_free_wksp(tran);
                return 1;
            }
            ret = glp_mpl_read_data(tran, MCFdataFile.c_str());
            if (ret != 0)
            {
                fprintf(stderr, "Error on translating data\n");
                glp_mpl_free_wksp(tran);
                return 1;
            }
            ret = glp_mpl_generate(tran, NULL);
            if (ret != 0)
            {
                std::cerr << "Error on generating model\n";
                glp_mpl_free_wksp(tran);
                return 1;
            }
            glp_mpl_build_prob(tran, lp_problem);
            
            glp_simplex(lp_problem, &param);
            
            ret = glp_mpl_postsolve(tran, lp_problem, GLP_SOL);
            if (ret != 0)
            {
                std::cerr <<  "Error on postsolving model\n";
                glp_mpl_free_wksp(tran);
                return 1;
            }
            
            glp_mpl_free_wksp(tran);
            return 0;
        }
        template<>
        inline void VYVineLinkEmbeddingAlgo<>::cleanUp()
        {
            substrateNodeIdSet.reset();
            virtualNodeIdSet.reset();
            substrateLinkIdSet.reset();
            virtualLinkIdSet.reset();
        }
        template<>
        inline Embedding_Result VYVineLinkEmbeddingAlgo<>::embeddLinks (
                                 std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
                                 const std::map<int,int>* nodeIdMap,
                                 std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap)
        {
           int substrateNodesNum = substrate_network->getNumNodes();
            
            substrateNodeIdSet = substrate_network->getNodeIdSet();
            substrateLinkIdSet = substrate_network->getLinkIdSet();
            virtualNodeIdSet = vnr->getVN()->getNodeIdSet();
            virtualLinkIdSet = vnr->getVN()->getLinkIdSet();
            
            allNodeIds  = std::vector<int>(substrateNodesNum);
            std::copy(substrateNodeIdSet->begin(), substrateNodeIdSet->end(), allNodeIds.begin());
            
            writeDataFile (substrate_network, vnr, nodeIdMap);
            if(solveLinkMappingLP (substrate_network, vnr)==1)
            {
                cleanUp();
                return Embedding_Result::ERROR_IN_SOLUTION;
            }
            if (glp_get_status(lp_problem) != GLP_OPT)
            {
                cleanUp();
                return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
            }

            /*
             * TO DO: collect the lp_objective Values
             *         an statistic collector needs to record
             *         glp_get_obj_val
             */
            vnr->linkMappingObjectiveVal = glp_get_obj_val(lp_problem);
            
            int numVariables = glp_get_num_cols(lp_problem);
            int flowId, from, to;
            //this variable is only used as a control variable
            //double lastVirtualLinkMappedBW = 0.0;
            for (int i=1; i <= numVariables; i++)
            {
                //int virtualLinkId = 0;
                double varVal = glp_get_col_prim(lp_problem, i);
                if (varVal > 1E-6)
                {
                    sscanf(glp_get_col_name(lp_problem, i),"f[f%d,%d,%d]", &flowId,&from,&to);
                    //std::cout << "primal: " << glp_get_col_name(lp_problem, i) << " = " << glp_get_col_prim(lp_problem, i) << std::endl;
                    int substrateLinkId = substrate_network->getLinkBetweenNodes(allNodeIds[from], allNodeIds[to])->getId();
                    auto it = virtualLinkIdSet->begin();
                    for (int i = 0; i<flowId; i++)
                        it++;
                    int virtualLinkId = *it;
                    std::shared_ptr<Resources<double>> _res (new Resources<double>(varVal));
                    if (linkMap == nullptr)
                        vnr->addLinkMapping(substrateLinkId, virtualLinkId, _res);
                    else
                        (*linkMap)[virtualLinkId].push_back(std::make_pair(substrateLinkId, _res));
                }
                
            }
            // This if statement is here to break the long if statement as soon as we get
            // all the mappings from the glpk in order to save time
            if ((vnr->getLinkMap()->size() == virtualLinkIdSet->size()
                || (linkMap != nullptr && (linkMap->size() ==  virtualLinkIdSet->size()))))
            {
                //lastVirtualLinkMappedBW += varVal;
                // We first need to make sure that we have read all the values for the last
                // link mapping and we are not breaking the if statement too early.
                // because linkMap size might be equal to number of virtual links but
                // there might still be some remaining enrties to be read (because of
                // flow splitting)
                //if (varVal == vnr->getVN()->getLink(virtualLinkId)->getBandwidth())
                //{
                cleanUp();
                return Embedding_Result::SUCCESSFUL_EMBEDDING;
                //}
            }
            cleanUp();
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
        }
        
        template<>
        Embedding_Result VYVineLinkEmbeddingAlgo<>::embeddVNRLinks
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            return (embeddLinks (substrate_network,vnr));
        }
        
        template<>
        Embedding_Result  VYVineLinkEmbeddingAlgo<>::embeddVNRLinksForIdSets
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
         const std::map<int,int>* nodeIdMap,
         std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap)
        {
            return (embeddLinks (substrate_network,vnr,nodeIdMap,linkMap));
        }
    }
}

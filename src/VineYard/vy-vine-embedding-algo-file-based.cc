/**
 * @file vy-vine-embedding-algo-file-based.cc
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

#include "vy-vine-embedding-algo-file-based.h"

#include "vy-substrate-network-builder.h"
#include "config-manager.h"

#include <float.h>

#define META_EDGE_BW 1000000

namespace vne {
    namespace vineyard{
        template<>
        VYVineEmbeddingAlgoFileBased<>::VYVineEmbeddingAlgoFileBased (NetworkBuilder<SUBSTRATE_TYPE>& _sb) :
        EmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>(_sb),
        lp_problem(glp_create_prob())
        {
            LPdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.LPdataFile");
            LPmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.LPmodelFile");
            MCFdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.MCFdataFile");
            MCFmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.MCFmodelFile");
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard.Configs.setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("vineyard.Configs.setBeta");
            if (ConfigManager::Instance()->getConfig<std::string>("vineyard.Configs.nodeMappingType").compare("randomized")==0)
                nodeMappingType = RANDOMIZED;
            else
                nodeMappingType = DETERMINISTIC;
        }
        template<>
        VYVineEmbeddingAlgoFileBased<>::VYVineEmbeddingAlgoFileBased (std::shared_ptr<SUBSTRATE_TYPE> _sn) :
        EmbeddingAlgorithm<SUBSTRATE_TYPE, VNR_TYPE>(_sn),
        lp_problem(glp_create_prob())
        {
            LPdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.LPdataFile");
            LPmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.LPmodelFile");
            MCFdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.MCFdataFile");
            MCFmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard.glpk.MCFmodelFile");
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard.Configs.setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("vineyard.Configs.setBeta");
            if (ConfigManager::Instance()->getConfig<std::string>("vineyard.Configs.nodeMappingType").compare("randomized")==0)
                nodeMappingType = RANDOMIZED;
            else
                nodeMappingType = DETERMINISTIC;
        }
        template<>
        VYVineEmbeddingAlgoFileBased<>::~VYVineEmbeddingAlgoFileBased()
        {
            if (lp_problem != NULL)
            {
                glp_delete_prob(lp_problem);
            }
        }
        
        template<>
        inline void VYVineEmbeddingAlgoFileBased<>::writeDataFiles(std::shared_ptr<VNR_TYPE> vnr)
        {
            int substrateNodesNum = substrate_network->getNumNodes();
            int virtualNodeNum =vnr->getVN()->getNumNodes();
            
            VYNodesReachabilityCondition reachability_cond;
            
            const std::shared_ptr<const std::set<int>> substrateNodeIdSet = substrate_network->getNodeIdSet();
            const std::shared_ptr<const std::set<int>> substrateLinkIdSet = substrate_network->getLinkIdSet();
            const std::shared_ptr<const std::set<int>> virtualNodeIdSet = vnr->getVN()->getNodeIdSet();
            const std::shared_ptr<const std::set<int>> virtualLinkIdSet = vnr->getVN()->getLinkIdSet();
            
            std::vector <int> allNodeIds (substrateNodesNum+virtualNodeNum);
            auto it = std::copy(substrateNodeIdSet->begin(), substrateNodeIdSet->end(), allNodeIds.begin());
            std::copy(virtualNodeIdSet->begin(), virtualNodeIdSet->end(), it);
            
            // coeeficent vector (b)
            std::vector<std::vector<double>> bVec ((substrateNodesNum+virtualNodeNum), std::vector<double>(substrateNodesNum+virtualNodeNum));
            //std::cout << dataFile << std::endl;
            //std::cout << modelFile << std::endl;
            FILE *fpDat = fopen(LPdataFile.c_str(), "w");
            FILE *mcfFpDat = fopen(MCFdataFile.c_str(), "w");
            
            fprintf(fpDat, "data;\n\n");
            fprintf(mcfFpDat, "data;\n\n");
            
            //Substrate Nodes
            fprintf(fpDat, "set N:=");
            fprintf(mcfFpDat, "set N:=");
            for (int i = 0; i < substrateNodesNum; i++) {
                fprintf(fpDat, " %d", i);
                fprintf(mcfFpDat, " %d", i);
            }
            fprintf(fpDat, ";\n");
            fprintf(mcfFpDat, ";\n");
            
            //Virtual Nodes
            fprintf(fpDat, "set M:=");
            for (int i = 0; i < virtualNodeNum; i++) {
                fprintf(fpDat, " %d", substrateNodesNum + i);
            }
            fprintf(fpDat, ";\n");
            
            // Virtual edges
            fprintf(fpDat, "set F:=");
            fprintf(mcfFpDat, "set F:=");
            
            for (int i = 0; i < vnr->getVN()->getNumLinks(); i++) {
                fprintf(fpDat, " f%d", i);
                fprintf(mcfFpDat, " f%d", i);
            }
            fprintf(fpDat, ";\n\n");
            fprintf(mcfFpDat, ";\n\n");
            
            // cpu capacities
            fprintf(fpDat, "param p:=\n");
            for (int i = 0; i < substrateNodesNum; i++) {
                fprintf(fpDat, "%d %.4lf\n", i, substrate_network->getNode(allNodeIds[i])->getCPU());
            }
            for (int i = 0; i < virtualNodeNum; i++) {
                fprintf(fpDat, "%d %.4lf\n", substrateNodesNum + i, vnr->getVN()->getNode(allNodeIds[substrateNodesNum + i])->getCPU());
            }
            fprintf(fpDat, ";\n\n");
            
            
            fprintf(fpDat, "param b:\n");
            fprintf(mcfFpDat, "param b:\n");
            for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                fprintf(fpDat, "%d ", i);
                if (i<substrateNodesNum)
                   fprintf(mcfFpDat, "%d ", i);
            }
            fprintf(fpDat, ":=\n");
            fprintf(mcfFpDat, ":=\n");
            
            for (int i = 0; i < (substrateNodesNum + virtualNodeNum); i++)
            {
                fprintf(fpDat, "%d ", i);
                if (i<substrateNodesNum)
                   fprintf(mcfFpDat, "%d ", i);
                for (int j = 0; j < (substrateNodesNum + virtualNodeNum); j++)
                {
                    if (i<substrateNodesNum && j<substrateNodesNum)
                    {
                        if (i==j || substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])==nullptr)
                        {
                            fprintf(fpDat, "%4lf ", 0.0);
                            fprintf(mcfFpDat, "%4lf ", 0.0);
                            bVec[i][j] = 0.0;
                        }
                        else
                        {
                            double bw = substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])->getBandwidth();
                            fprintf(fpDat, "%4lf ", bw);
                            fprintf(mcfFpDat, "%4lf ", bw);
                            bVec[i][j] = bw;
                        }
                    }
                    else
                    {
                        if (i>=substrateNodesNum && j<substrateNodesNum)
                        {
                            if (reachability_cond(substrate_network->getNode(allNodeIds[j]), vnr->getVN()->getNode(allNodeIds[i]), vnr->getMaxDistance()))
                            {
                                fprintf(fpDat, "%d ", META_EDGE_BW);
                                bVec[i][j] = META_EDGE_BW;
                            }
                            else
                            {
                                fprintf(fpDat, "%4lf ", 0.0);
                                bVec[i][j] = 0.0;
                            }
                        }
                        else if (i<substrateNodesNum && j>=substrateNodesNum)
                        {
                            if (reachability_cond(substrate_network->getNode(allNodeIds[i]), vnr->getVN()->getNode(allNodeIds[j]), vnr->getMaxDistance()))
                            {
                                fprintf(fpDat, "%d ", META_EDGE_BW);
                                bVec[i][j] = META_EDGE_BW;
                            }
                            else
                            {
                                fprintf(fpDat, "%4lf ", 0.0);
                                bVec[i][j] = 0.0;
                            }
                        }
                        else // if i>n && j>n
                        {
                            fprintf(fpDat, "%4lf ", 0.0);
                            bVec[i][j] = 0.0;
                        }
                    }
                }
                fprintf(fpDat, "\n");
                if (i<substrateNodesNum)
                    fprintf(mcfFpDat, "\n");
            }
            
            fprintf(fpDat, ";\n\n");
            fprintf(mcfFpDat, ";\n\n");
            
            fprintf(fpDat, "param alpha:\n");
            fprintf(mcfFpDat, "param alpha:\n");
            for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                fprintf(fpDat, "%d ", i);
                if (i<substrateNodesNum)
                   fprintf(mcfFpDat, "%d ", i);
            }
            fprintf(fpDat, ":=\n");
            fprintf(mcfFpDat, ":=\n");
            
            //alpha parameter
            if (setAlpha == true)
            {
                for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                    fprintf(fpDat, "%d ", i);
                    if (i<substrateNodesNum)
                        fprintf(mcfFpDat, "%d ", i);
                    for (int j = 0; j < substrateNodesNum + virtualNodeNum; j++) {
                        fprintf(fpDat, "%.4lf ", bVec[i][j]);
                        if (j<substrateNodesNum)
                           fprintf(mcfFpDat, "%.4lf ", bVec[i][j]);
                    }
                    fprintf(fpDat, "\n");
                    if (i<substrateNodesNum)
                        fprintf(mcfFpDat, "\n");
                       
                }
            }
            else
            {
                for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                    fprintf(fpDat, "%d ", i);
                    if (i<substrateNodesNum)
                        fprintf(mcfFpDat, "%d ", i);
                    for (int j = 0; j < substrateNodesNum + virtualNodeNum; j++) {
                        fprintf(fpDat, "1 ");
                        if (j<substrateNodesNum)
                            fprintf(mcfFpDat, "1 ");
                    }
                    fprintf(fpDat, "\n");
                    if (i<substrateNodesNum)
                        fprintf(mcfFpDat, "\n");
                }
            }
            fprintf(fpDat, ";\n\n");
            fprintf(mcfFpDat, ";\n\n");
            
            //beta parameter
            fprintf(fpDat, "param beta:=\n");
            if (setBeta == true) {
                for (int i = 0; i < substrateNodesNum; i++) {
                    fprintf(fpDat, "%d %.4lf\n", i, substrate_network->getNode(allNodeIds[i])->getCPU());
                }
                for (int i = 0; i < virtualNodeNum; i++) {
                    fprintf(fpDat, "%d %.4lf\n", substrateNodesNum + i, vnr->getVN()->getNode(allNodeIds[substrateNodesNum + i])->getCPU());
                }
            }
            else {
                for (int i = 0; i < substrateNodesNum; i++) {
                    fprintf(fpDat, "%d 1\n", i);
                }
                for (int i = 0; i < virtualNodeNum; i++) {
                    fprintf(fpDat, "%d 1\n", substrateNodesNum + i);
                }
            }
            fprintf(fpDat, ";\n\n");
            
            // flow source : fs
            int count = 0;
            fprintf(fpDat, "param fs:=\n");
            fprintf(mcfFpDat, "param fs:=\n");
            
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++)
            {
                auto flowSource = std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(*it)->getNodeFromId()));
                fprintf(fpDat, "f%d %d\n", count, (int)flowSource);
                fprintf(mcfFpDat, "f%d %d\n", count, (int)flowSource);
                count++;
            }
            fprintf(fpDat, ";\n\n");
            fprintf(mcfFpDat, ";\n\n");
            
            // flow destination
            count=0;
            fprintf(fpDat, "param fe:=\n");
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                auto flowDestination =
                std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(*it)->getNodeToId()));
                fprintf(fpDat, "f%d %d\n", count, (int)flowDestination);
                fprintf(mcfFpDat, "f%d %d\n", count, (int)flowDestination);
                count++;
            }
            fprintf(fpDat, ";\n\n");
            fprintf(mcfFpDat, ";\n\n");
            
            // flow requirements
            count = 0;
            fprintf(fpDat, "param fd:=\n");
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                fprintf(fpDat, "f%d %.4lf\n", count, vnr->getVN()->getLink(*it)->getBandwidth());
                fprintf(mcfFpDat, "f%d %.4lf\n", count, vnr->getVN()->getLink(*it)->getBandwidth());
                count++;
            }
            fprintf(fpDat, ";\n\n");
            fprintf(mcfFpDat, ";\n\n");
            
            fprintf(fpDat, "end;\n\n");
            fprintf(mcfFpDat, "end;\n\n");
            
            fclose(fpDat);
            fclose(mcfFpDat);
        }
        
        template<>
        inline int VYVineEmbeddingAlgoFileBased<>::solveNodeMappingLP(std::shared_ptr<VNR_TYPE> vnr)
        {
            glp_erase_prob(lp_problem);
            int ret;
            glp_smcp param;
            glp_init_smcp(&param);
            //turn on the preprocessor
            param.presolve = GLP_ON;
            
            glp_tran *tran;
            tran = glp_mpl_alloc_wksp();
            ret = glp_mpl_read_model(tran, LPmodelFile.c_str(), 0);
            if (ret != 0)
            {
                std::cerr << "Error on translating model\n";
                glp_mpl_free_wksp(tran);
                return 1;
            }
            ret = glp_mpl_read_data(tran, LPdataFile.c_str());
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
        Embedding_Result VYVineEmbeddingAlgoFileBased<>::embeddVNR(std::shared_ptr<VNR_TYPE> vnr)
        {
            writeDataFiles (vnr);
            if(solveNodeMappingLP (vnr)==1)
                return Embedding_Result::ERROR_IN_SOLUTION;
            
            if (glp_get_status(lp_problem) != GLP_OPT)
                return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
            
            /*
             * TO DO: collect the lp_objective Values
             *         an statistic collector needs to record
             *         glp_get_obj_val
             */
            
            int substrateNodesNum = substrate_network->getNumNodes();
            int virtualNodeNum =vnr->getVN()->getNumNodes();
            
            std::vector<std::vector<double>> flowVec (substrateNodesNum + virtualNodeNum, std::vector<double> (substrateNodesNum + virtualNodeNum));
            
            

            //parseResults ();
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
        }
    }
}

/**
 * @file vy-vine-node-embedding-algo.cc
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

#include "vy-vine-node-embedding-algo.h"
#include "core/config-manager.h"
#include "core/rng.h"

namespace vne {
    namespace vineyard{
        template<>
        VYVineNodeEmbeddingAlgo<>::VYVineNodeEmbeddingAlgo () :
        NodeEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> > (),
        lp_problem(nullptr),
        substrateLinkIdSet(nullptr),
        substrateNodeIdSet(nullptr),
        virtualLinkIdSet(nullptr),
        virtualNodeIdSet(nullptr)
        {
            glp_term_out(ConfigManager::Instance()->getConfig<int>("vineyard", "glpk", "terminalEnabled"));
            
            LPdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "LPdataFile");
            LPmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "LPmodelFile");
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setBeta");
            if (ConfigManager::Instance()->getConfig<std::string>("vineyard", "Configs", "nodeMappingType").compare("randomized")==0)
                nodeMappingType = RANDOMIZED;
            else
                nodeMappingType = DETERMINISTIC;
        }
        template<>
        VYVineNodeEmbeddingAlgo<>::~VYVineNodeEmbeddingAlgo()
        {
            if (lp_problem != NULL)
            {
                glp_delete_prob(lp_problem);
            }
        }
        
        template<>
        inline void VYVineNodeEmbeddingAlgo<>::writeDataFile
                            (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            int substrateNodesNum = substrate_network->getNumNodes();
            int virtualNodeNum =vnr->getVN()->getNumNodes();
            
            VYNodesReachabilityCondition reachability_cond;
            
            
            // coeeficent vector (b)
            std::vector<std::vector<double>> bVec ((substrateNodesNum+virtualNodeNum), std::vector<double>(substrateNodesNum+virtualNodeNum));
            //std::cout << dataFile << std::endl;
            //std::cout << modelFile << std::endl;
            FILE *fpDat = fopen(LPdataFile.c_str(), "w");
            
            fprintf(fpDat, "data;\n\n");
            
            //Substrate Nodes
            fprintf(fpDat, "set N:=");
            for (int i = 0; i < substrateNodesNum; i++) {
                fprintf(fpDat, " %d", i);
            }
            fprintf(fpDat, ";\n");
            
            //Virtual Nodes
            fprintf(fpDat, "set M:=");
            for (int i = 0; i < virtualNodeNum; i++) {
                fprintf(fpDat, " %d", substrateNodesNum + i);
            }
            fprintf(fpDat, ";\n");
            
            // Virtual edges
            fprintf(fpDat, "set F:=");
            for (int i = 0; i < vnr->getVN()->getNumLinks(); i++) {
                fprintf(fpDat, " f%d", i);
            }
            fprintf(fpDat, ";\n\n");
            
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
            for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                fprintf(fpDat, "%d ", i);
            }
            fprintf(fpDat, ":=\n");
            
            for (int i = 0; i < (substrateNodesNum + virtualNodeNum); i++)
            {
                fprintf(fpDat, "%d ", i);
                for (int j = 0; j < (substrateNodesNum + virtualNodeNum); j++)
                {
                    if (i<substrateNodesNum && j<substrateNodesNum)
                    {
                        if (i==j || substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])==nullptr)
                        {
                            fprintf(fpDat, "%.4lf ", 0.0);
                            bVec[i][j] = 0.0;
                        }
                        else
                        {
                            double bw = substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])->getBandwidth();
                            fprintf(fpDat, "%.4lf ", bw);
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
                                nodesWithinReach[i].push_back(j);
                            }
                            else
                            {
                                fprintf(fpDat, "%.4lf ", 0.0);
                                bVec[i][j] = 0.0;
                            }
                        }
                        else if (i<substrateNodesNum && j>=substrateNodesNum)
                        {
                            if (reachability_cond(substrate_network->getNode(allNodeIds[i]), vnr->getVN()->getNode(allNodeIds[j]), vnr->getMaxDistance()))
                            {
                                fprintf(fpDat, "%d ", META_EDGE_BW);
                                bVec[i][j] = META_EDGE_BW;
                                nodesWithinReach[j].push_back(i);
                            }
                            else
                            {
                                fprintf(fpDat, "%.4lf ", 0.0);
                                bVec[i][j] = 0.0;
                            }
                        }
                        else // if i>n && j>n
                        {
                            fprintf(fpDat, "%.4lf ", 0.0);
                            bVec[i][j] = 0.0;
                        }
                    }
                }
                fprintf(fpDat, "\n");
            }
            
            fprintf(fpDat, ";\n\n");
            
            fprintf(fpDat, "param alpha:\n");
            for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                fprintf(fpDat, "%d ", i);
            }
            fprintf(fpDat, ":=\n");
            
            //alpha parameter
            if (setAlpha == true)
            {
                for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                    fprintf(fpDat, "%d ", i);
                    for (int j = 0; j < substrateNodesNum + virtualNodeNum; j++) {
                        fprintf(fpDat, "%.4lf ", bVec[i][j]);
                    }
                    fprintf(fpDat, "\n");
                }
            }
            else
            {
                for (int i = 0; i < substrateNodesNum + virtualNodeNum; i++) {
                    fprintf(fpDat, "%d ", i);
                    for (int j = 0; j < substrateNodesNum + virtualNodeNum; j++) {
                        fprintf(fpDat, "1 ");
                    }
                    fprintf(fpDat, "\n");
                }
            }
            fprintf(fpDat, ";\n\n");
            
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
            
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++)
            {
                auto flowSource = std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(*it)->getNodeFromId()));
                fprintf(fpDat, "f%d %d\n", count, substrateNodesNum + (int)flowSource);
                count++;
            }
            fprintf(fpDat, ";\n\n");
            
            // flow destination
            count=0;
            fprintf(fpDat, "param fe:=\n");
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                auto flowDestination =
                std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(*it)->getNodeToId()));
                fprintf(fpDat, "f%d %d\n", count, substrateNodesNum + (int)flowDestination);
                count++;
            }
            fprintf(fpDat, ";\n\n");
            
            // flow requirements
            count = 0;
            fprintf(fpDat, "param fd:=\n");
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                fprintf(fpDat, "f%d %.4lf\n", count, vnr->getVN()->getLink(*it)->getBandwidth());
                count++;
            }
            fprintf(fpDat, ";\n\n");
            
            fprintf(fpDat, "end;\n\n");
            
            fclose(fpDat);
        }
        
        template<>
        inline int VYVineNodeEmbeddingAlgo<>::solveNodeMappingLP
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
            
            glp_simplex(this->lp_problem, &param);
            
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
        inline Embedding_Result VYVineNodeEmbeddingAlgo<>::deterministicNodeMapping
            (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr, std::vector<std::vector<double>>& xVec)
        {
            int substrateNodeNum = substrate_network->getNumNodes();
            for (int i = 0; i<vnr->getVN()->getNumNodes(); i++)
            {
                int maxIndex = -1;
                double maxValue = 0.0;
                for (int j=0; j < nodesWithinReach[substrateNodeNum + i].size(); j++)
                {
                    int current_substrate_node_local_id = nodesWithinReach[substrateNodeNum + i].at(j);
                    if (xVec[substrateNodeNum + i][current_substrate_node_local_id]>=maxValue &&
                        !(substrate_network->getNode(allNodeIds[current_substrate_node_local_id])->touched))
                    {
                        maxValue = xVec[substrateNodeNum + i][current_substrate_node_local_id];
                        maxIndex = current_substrate_node_local_id;
                    }
                }
                if (maxIndex == -1)
                    return Embedding_Result::ERROR_IN_SOLUTION;
                vnr->addNodeMapping(allNodeIds[maxIndex], allNodeIds[substrateNodeNum + i]);
                substrate_network->getNode(allNodeIds[maxIndex])->touched = true;
            }
            if (vnr->getNodeMap()->size() == vnr->getVN()->getNumNodes())
                return Embedding_Result::SUCCESSFUL_EMBEDDING;
            return Embedding_Result::ERROR_IN_SOLUTION;
        }
        
        template<>
        inline Embedding_Result VYVineNodeEmbeddingAlgo<>::randomizedNodeMapping
            (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr, std::vector<std::vector<double>>& xVec)
        {
            
            int substrateNodeNum = substrate_network->getNumNodes();
            for (int i = 0; i<vnr->getVN()->getNumNodes(); i++)
            {
                int randTry = 0;
                int randIndex = -1;
                while (randTry < 16) {
                    double randValue = gsl_rng_uniform (RNG::Instance()->getGeneralRNG());
                    for (int j=0; j < nodesWithinReach[substrateNodeNum + i].size(); j++)
                    {
                        int current_substrate_node_local_id = nodesWithinReach[substrateNodeNum + i].at(j);
                        randValue -= xVec[substrateNodeNum + i][current_substrate_node_local_id];
                        if (randValue <=0.0 &&
                            !(substrate_network->getNode(allNodeIds[current_substrate_node_local_id])->touched))
                        {
                            randIndex = current_substrate_node_local_id;
                            break;
                        }
                    }
                    if (randIndex != -1)
                            break;
                    randTry++;
                }
                randTry = 0;
                if (randIndex == -1) {
                    while (randTry < 16) {
                        int randValue = (int) gsl_rng_uniform_int (RNG::Instance()->getGeneralRNG(),  nodesWithinReach[substrateNodeNum + i].size());
                        int current_substrate_node_local_id = nodesWithinReach[substrateNodeNum + i].at(randValue);
                        if (!(substrate_network->getNode(allNodeIds[current_substrate_node_local_id])->touched))
                        {
                            randIndex = current_substrate_node_local_id;
                            break;
                        }
                        randTry ++;
                    }
                }
                if (randIndex == -1)
                    return Embedding_Result::ERROR_IN_SOLUTION;
                vnr->addNodeMapping(allNodeIds[randIndex], allNodeIds[substrateNodeNum + i]);
                substrate_network->getNode(allNodeIds[randIndex])->touched = true;
                
            }
            if (vnr->getNodeMap()->size() == vnr->getVN()->getNumNodes())
                return Embedding_Result::SUCCESSFUL_EMBEDDING;
            return Embedding_Result::ERROR_IN_SOLUTION;
        }
        
        template<>
        inline void VYVineNodeEmbeddingAlgo<>::cleanUp(std::shared_ptr<SUBSTRATE_TYPE> substrate_network)
        {
            substrateNodeIdSet.reset();
            virtualNodeIdSet.reset();
            substrateLinkIdSet.reset();
            virtualLinkIdSet.reset();
            nodesWithinReach.clear();
            for (int i = 0 ; i < substrate_network->getNumNodes(); i++)
                substrate_network->getNode(allNodeIds[i])->touched = false;
        }
        
        template<>
        Embedding_Result VYVineNodeEmbeddingAlgo<>::embeddVNRNodes
                (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            int substrateNodesNum = substrate_network->getNumNodes();
            int virtualNodeNum =vnr->getVN()->getNumNodes();
            
            substrateNodeIdSet = substrate_network->getNodeIdSet();
            substrateLinkIdSet = substrate_network->getLinkIdSet();
            virtualNodeIdSet = vnr->getVN()->getNodeIdSet();
            virtualLinkIdSet = vnr->getVN()->getLinkIdSet();
            
            allNodeIds  = std::vector<int>(substrateNodesNum + virtualNodeNum);
            auto it = std::copy(substrateNodeIdSet->begin(), substrateNodeIdSet->end(), allNodeIds.begin());
            std::copy(virtualNodeIdSet->begin(), virtualNodeIdSet->end(), it);
            writeDataFile (substrate_network, vnr);
            if(solveNodeMappingLP (substrate_network, vnr)==1)
            {
                cleanUp (substrate_network);
                return Embedding_Result::ERROR_IN_SOLUTION;
            }
            
            if (glp_get_status(lp_problem) != GLP_OPT)
            {
                cleanUp (substrate_network);
                return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
            }
            
            /*
             * TO DO: collect the lp_objective Values
             *         an statistic collector needs to record
             *         glp_get_obj_val
             */
            vnr->nodeMappingObjectiveVal = glp_get_obj_val(lp_problem);
            
            std::vector<std::vector<double>> flowVec (substrateNodesNum + virtualNodeNum, std::vector<double> (substrateNodesNum + virtualNodeNum, 0));
            std::vector<std::vector<double>> xVec (substrateNodesNum + virtualNodeNum, std::vector<double> (substrateNodesNum + virtualNodeNum, 0));
            char varName[50];
            int numVariables = glp_get_num_cols(lp_problem);
            int flowId, from, to;
            float Ep = ConfigManager::Instance()->getConfig<float>("vineyard", "Constants", "epsilon");
            
            for (int i=1; i<= numVariables; i++)
            {
                sscanf(glp_get_col_name(lp_problem, i),"%s",varName);
                double varVal = glp_get_col_prim(lp_problem, i);
                //flow variables
                if (varName[0]=='f')
                {
                    sscanf(varName, "f[f%d,%d,%d]", &flowId,&from,&to);
                    if (varVal>0.00001)
                    BOOST_LOG_TRIVIAL(debug) << from << " " << to << " " <<varVal << std::endl;
                    flowVec[from][to] += varVal;
                    
                }
                //indicator variables
                else
                {
                    sscanf(varName, "x[%d,%d]", &from, &to);
                    xVec[from][to] = varVal * (flowVec[from][to] + flowVec [to][from]);
                    //if (from >= substrateNodesNum && to < substrateNodesNum && varVal > Ep
                    //                            && (flowVec[from][to] + flowVec[to][from])> Ep)
                    //    candidateMappings[allNodeIds[from]].push_back(std::make_pair(allNodeIds[to], varVal));
                }
                //std::cout<< glp_get_col_name(lp_problem, i) << std::endl;
            }
            //normalize xVec
            
            for (int i = 0; i < virtualNodeNum; i++)
            {
                double tot = 0;
                for (int j = 0; j< nodesWithinReach[substrateNodesNum + i].size(); j++)
                {
                    {
                        tot += xVec[substrateNodesNum+i][nodesWithinReach[substrateNodesNum + i].at(j)];
                    }
                }
                if (tot < Ep)
                    tot = 1.0;
                for (int j=0 ; j< nodesWithinReach[substrateNodesNum + i].size(); j++)
                    xVec[substrateNodesNum+i][nodesWithinReach[substrateNodesNum + i].at(j)] /= tot;
            }

            
            if (nodeMappingType==DETERMINISTIC)
            {
                Embedding_Result r = deterministicNodeMapping (substrate_network, vnr, xVec);
                cleanUp(substrate_network);
                return r;
            }
            else
            {
                Embedding_Result r = randomizedNodeMapping (substrate_network, vnr, xVec);
                cleanUp(substrate_network);
                return r;
            }
            cleanUp(substrate_network);
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
        }
    }
}

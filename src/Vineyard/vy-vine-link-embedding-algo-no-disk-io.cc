/**
 * @file vy-vine-link-embedding-algo-no-disk-io.cc
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

#include "vy-vine-link-embedding-algo-no-disk-io.h"
#include "core/config-manager.h"

namespace vne {
    namespace vineyard{
        template<>
        VYVineLinkEmbeddingAlgoNoDiskIO<>::VYVineLinkEmbeddingAlgoNoDiskIO () :
        LinkEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> > (Link_Embedding_Algo_Types::WITH_PATH_SPLITTING),
        lp_problem(nullptr),
        substrateLinkIdSet(nullptr),
        substrateNodeIdSet(nullptr)
        {
            glp_term_out(ConfigManager::Instance()->getConfig<int>("vineyard", "glpk", "terminalEnabled"));
            
            MCFdataFile = ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "MCFdataFile");
            MCFmodelFile = ConfigManager::Instance()->getConfig<std::string>("vineyard", "glpk", "MCFmodelFile");
            
            setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setAlpha");
            setBeta  = ConfigManager::Instance()->getConfig<bool>("vineyard", "Configs", "setBeta");
        }
        template<>
        VYVineLinkEmbeddingAlgoNoDiskIO<>::~VYVineLinkEmbeddingAlgoNoDiskIO()
        {
            if (lp_problem != NULL)
            {
                glp_delete_prob(lp_problem);
            }
        }
        
        template<>
        inline void VYVineLinkEmbeddingAlgoNoDiskIO<>::prepareProblem
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr, const std::map<int,int>* nodeIdMap)
        {
            glp_set_obj_dir(lp_problem, GLP_MIN);
            glp_set_obj_name(lp_problem, "cost");
            
            int substrateNodesNum = substrate_network->getNumNodes();
            int numFlows = vnr->getVN()->getNumLinks();
            //capacity constrains
            int numRows = substrateNodesNum * substrateNodesNum;
            //2 demand satisfaction constrains
            numRows += 2 * numFlows;
            //flow constrans
            numRows += numFlows * (substrateNodesNum - 2);
            
            glp_add_rows(lp_problem, numRows);
            glp_add_cols(lp_problem, substrateNodesNum * substrateNodesNum * numFlows);
            
    
            // coeeficent vector (b)
            std::vector<std::vector<double>> bVec ((substrateNodesNum), std::vector<double>(substrateNodesNum));
            
            // flow source : fs
            int count = 0;
            
            std::vector<int> flowSource (numFlows);
            std::vector<int> flowDest (numFlows);
            std::vector<double> flowDem (numFlows);

            std::vector<int> aj (1000);
            std::vector<double> ar (1000);
            
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++)
            {
                int vNodeFromId = vnr->getVN()->getLink(*it)->getNodeFromId();
                int sNodeFromId;
                if(nodeIdMap == nullptr)
                    sNodeFromId = vnr->getNodeMap()->find(vNodeFromId)->second;
                else
                    sNodeFromId = nodeIdMap->find(vNodeFromId)->second;
                auto source = std::distance(substrateNodeIdSet->begin(), substrateNodeIdSet->find(sNodeFromId));
                flowSource[count] = (int) source;
                count++;
            }
            
            // flow destination
            count=0;
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                int vNodeToId = vnr->getVN()->getLink(*it)->getNodeToId();
                int sNodeToId;
                if (nodeIdMap == nullptr)
                    sNodeToId = vnr->getNodeMap()->find(vNodeToId)->second;
                else
                    sNodeToId = nodeIdMap->find(vNodeToId)->second;
                
                auto dest = std::distance(substrateNodeIdSet->begin(), substrateNodeIdSet->find(sNodeToId));
                flowDest[count] = (int) dest;
                count++;
            }
            
            count = 0;
            for (auto it = virtualLinkIdSet->begin(); it != virtualLinkIdSet->end() ; it++) {
                flowDem[count]  = vnr->getVN()->getLink(*it)->getBandwidth();
                count++;
            }
            
           
            //SET THE ROW NAMES AND BANDS
            // CAPACITY CONSTRAINTS
            int rowNum = 1;
            int colNum = 1;
            for (int i = 0; i < substrateNodesNum; i++)
            {
                for (int j = 0; j < substrateNodesNum; j++)
                {
                    if (i==j || substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])==nullptr)
                    {
                        bVec[i][j] = 0.0;
                    }
                    else
                    {
                        double bw = substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])->getBandwidth();
                        bVec[i][j] = bw;
                    }
                    char buffer [20];
                    sprintf(buffer, "capcon[%d,%d]", i,j);
                    glp_set_row_name(lp_problem, rowNum, buffer);
                    glp_set_row_bnds(lp_problem, rowNum, GLP_UP, 0.0, bVec[i][j]);
                    
                    for (int k = 0; k < numFlows; k++)
                    {
                        //SET COLUMNS
                        sprintf(buffer, "f[f%d,%d,%d]", k,j,i);
                        glp_set_col_name(lp_problem, colNum, buffer);
                        glp_set_col_bnds(lp_problem, colNum, GLP_LO, 0.0, 0.0);
                        double coeff = 1.0;
                        if (setAlpha)
                            coeff = bVec[i][j];
                        glp_set_obj_coef(lp_problem, colNum, coeff/(bVec[i][j]+1E-6));
                        colNum++;
                    }
                    rowNum++;
                }
            }
            
            //DEMAND SATSFACTION 1
            for (int k = 0; k < numFlows; k++)
            {
                char buffer [20];
                sprintf(buffer, "demsat1[f%d]", k);
                glp_set_row_name(lp_problem, rowNum, buffer);
                glp_set_row_bnds(lp_problem, rowNum, GLP_FX, flowDem[k], flowDem[k]);
                rowNum++;
            }
            
            //DEMAND SATSFACTION 2
            for (int k = 0; k < numFlows; k++)
            {
                char buffer [20];
                sprintf(buffer, "demsat2[f%d]", k);
                glp_set_row_name(lp_problem, rowNum, buffer);
                glp_set_row_bnds(lp_problem, rowNum, GLP_FX, -flowDem[k], -flowDem[k]);
                rowNum++;
            }
            
            //FLOW CONSERVATION
            for (int i = 0; i < numFlows; i++)
            {
                for (int j = 0; j < substrateNodesNum; j++)
                {
                    char buffer [20];
                    if( j != flowDest[i] && j != flowSource[i] )
                    {
                        sprintf(buffer, "flocon[f%d,%d]", i,j);
                        glp_set_row_name(lp_problem, rowNum, buffer);
                        glp_set_row_bnds(lp_problem, rowNum, GLP_FX, 0, 0);
                        rowNum++;
                    }
                }
            }
            
        //CAPACITY CONSTRAINTS
        rowNum = 1;
        for (int i = 0; i < substrateNodesNum; i++)
        {
            for (int j = 0; j < substrateNodesNum; j++)
            {
                int colNum = 1;
                int index = 1;
                ar.clear();
                aj.clear();
                for (int k = 0; k < numFlows; k++)
                {
                    for (int l = 0; l < substrateNodesNum; l++)
                    {
                        for (int m = 0; m < substrateNodesNum; m++)
                        {
                            int from, to, flowNum;
                            sscanf(glp_get_col_name(lp_problem, colNum), "f[f%d,%d,%d]", &flowNum, &from, &to);
                            if (flowNum == l && ((from == i && to == j) || (from == j && to == i)))
                            {
                                ar[index] = 1.0;
                                aj[index] = colNum;
                                index++;
                            }
                            colNum++;
                        }
                    }
                }
                //ar.resize(ar.size() - 1);
                //aj.resize(aj.size() - 1);
                glp_set_mat_row(lp_problem, rowNum, index - 1, &aj[0], &ar[0]);
                rowNum++;
            }
        }
        //DEMAND SATISFACTION 1
        for (int j = 0; j < numFlows; j++)
        {
            int colNum = 1;
            int index = 1;
            ar.clear();
            aj.clear();
            for (int k = 0; k < numFlows; k++)
            {
                for (int l = 0; l < substrateNodesNum; l++)
                {
                    for (int m = 0; m < substrateNodesNum; m++)
                    {
                        int from, to, flowNum;
                        sscanf(glp_get_col_name(lp_problem, colNum), "f[f%d,%d,%d]", &flowNum, &from, &to);
                        if (flowNum == j && flowSource[flowNum] == from)
                        {
                            ar[index] = 1.0;
                            aj[index] = colNum;
                            index++;
                        }
                        else if (flowNum == j && flowSource[flowNum] == to)
                        {
                            ar[index] = -1.0;
                            aj[index] = colNum;
                            index++;
                        }
                        colNum++;
                    }
                }
            }
            glp_set_mat_row(lp_problem, rowNum, index - 2, &aj[0], &ar[0]);
            rowNum++;
        }
            
        //DEMAND SATISFACTION 2
        for (int j = 0; j < numFlows; j++)
        {
            int colNum = 1;
            int index = 1;
            ar.clear();
            aj.clear();
            for (int k = 0; k < numFlows; k++)
            {
                for (int l = 0; l < substrateNodesNum; l++)
                {
                    for (int m = 0; m < substrateNodesNum; m++)
                    {
                        aj[colNum] = colNum;
                        int from, to, flowNum;
                        sscanf(glp_get_col_name(lp_problem, colNum), "f[f%d,%d,%d]", &flowNum, &from, &to);
                        if (flowNum == j && flowDest[flowNum] == from)
                        {
                            ar[index] = 1.0;
                            aj[index] = colNum;
                            index++;
                        }
                        else if (flowNum == j && flowDest[flowNum] == to)
                        {
                            ar[index] = -1.0;
                            aj[index] = colNum;
                            index++;
                        }
                        colNum++;
                    }
                }
            }
            glp_set_mat_row(lp_problem, rowNum, index -2, &aj[0], &ar[0]);
            rowNum++;
        }
            //FLOW CONSERVATION
            for (int i = 0; i < numFlows; i++)
            {
                for (int j = 0; j < substrateNodesNum; j++)
                {
                    if (j == flowSource[i] || j== flowDest[i])
                        continue;
                    int colNum = 1;
                    int index = 1;
                    ar.clear();
                    aj.clear();
                    for (int k = 0; k < numFlows; k++)
                    {
                        for (int l = 0; l < substrateNodesNum; l++)
                        {
                            for (int m = 0; m < substrateNodesNum; m++)
                            {
                                aj[colNum] = colNum;
                                int from, to, flowNum;
                                sscanf(glp_get_col_name(lp_problem, colNum), "f[f%d,%d,%d]", &flowNum, &from, &to);
                                if (from == j)
                                {
                                    ar[index] = 1.0;
                                    aj[index] = colNum;
                                    index++;
                                }
                                else if (to == j)
                                {
                                    ar[index] = 1.0;
                                    aj[index] = colNum;
                                    index++;
                                }
                                colNum++;
                            }
                        }
                    }
                
                    glp_set_mat_row(lp_problem, rowNum, index - 2, &aj[0], &ar[0]);
                    rowNum++;
                }
            }
            glp_write_mps(lp_problem, GLP_MPS_FILE, NULL, "/Users/Soroosh/Desktop/memorybased.mps");
            
        }
        
        template<>
        inline int VYVineLinkEmbeddingAlgoNoDiskIO<>::solveLinkMappingLP
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr, const std::map<int,int>* nodeIdMap)
        {
            if (lp_problem == nullptr) {
                lp_problem = glp_create_prob();
            }
            else
                glp_erase_prob(lp_problem);

            glp_smcp param;
            glp_init_smcp(&param);
            //turn on the preprocessor
            param.presolve = GLP_ON;
            
            /*
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
            */
            
            prepareProblem(substrate_network, vnr, nodeIdMap);
            
            glp_simplex(lp_problem, &param);
            
            //ret = glp_mpl_postsolve(tran, lp_problem, GLP_SOL);
            /*if (ret != 0)
            {
                std::cerr <<  "Error on postsolving model\n";
                glp_mpl_free_wksp(tran);
                return 1;
            }
            
            glp_mpl_free_wksp(tran);
            */
            return 0;
        }
        template<>
        inline void VYVineLinkEmbeddingAlgoNoDiskIO<>::cleanUp()
        {
            substrateNodeIdSet.reset();
            virtualNodeIdSet.reset();
            substrateLinkIdSet.reset();
            virtualLinkIdSet.reset();
        }
        template<>
        inline Embedding_Result VYVineLinkEmbeddingAlgoNoDiskIO<>::embeddLinks (
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
            
            //writeDataFile (substrate_network, vnr, nodeIdMap);
            if(solveLinkMappingLP (substrate_network, vnr, nodeIdMap)==1)
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
        Embedding_Result VYVineLinkEmbeddingAlgoNoDiskIO<>::embeddVNRLinks
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
        {
            return (embeddLinks (substrate_network,vnr));
        }
        
        template<>
        Embedding_Result  VYVineLinkEmbeddingAlgoNoDiskIO<>::embeddVNRLinksForIdSets
        (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr,
         const std::map<int,int>* nodeIdMap,
         std::map<int,std::list<std::pair<int, std::shared_ptr<Resources<double>>>>>* linkMap)
        {
            return (embeddLinks (substrate_network,vnr,nodeIdMap,linkMap));
        }
    }
}

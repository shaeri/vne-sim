/**
 * @file vy-vine-embedding-algo.cc
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
#if 0
#include "vy-vine-embedding-algo.h"
#include "vy-substrate-network-builder.h"
#include "config-manager.h"

#include <float.h>

namespace vne {
    namespace vineyard{
       template<>
        VYVineEmbeddingAlgo<>::VYVineEmbeddingAlgo (VYSubstrateNetworkBuilder<>& _sb) :
            EmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> >(_sb),
                lp_problem(glp_create_prob())
        {
        }
        template<>
        VYVineEmbeddingAlgo<>::VYVineEmbeddingAlgo (std::shared_ptr<Network<VYSubstrateNode<>, VYSubstrateLink<>>> _sn) :
        EmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<> >, VYVirtualNetRequest<> >(_sn),
        lp_problem(glp_create_prob())
        {
        }
        template<>
        VYVineEmbeddingAlgo<>::~VYVineEmbeddingAlgo()
        {
            if (lp_problem != NULL)
            {
                glp_delete_prob(lp_problem);
            }
        }
        
        template<>
        inline void VYVineEmbeddingAlgo<>::setGLPKProblemObject(std::shared_ptr<VNR_TYPE> vnr)
        {
            glp_erase_prob(lp_problem);
            /***********************************************************************************
             * n = number of substrate nodes
             * m = number of virtual nodes
             * l = number of substarte links
             * k = number of virtual links
             *
             * -------------------------------------------------------------------------------
             * Variables:
             *
             * flow variable f[i u v] >= 0 : k * (m+n) * (m+n)
             * indicator var x[u v] >= 0   : (m+n) * (m+n)  
             *   if we are solving IP version then x[u v] must be binary {0,1}
             * --------------------------------------------------------------------------------
             * Constraints:
             *
             * Capacity constraint:                 (m+n) * (m+n)
             * Demant constraints:                  2 * k
             * Conservation of flow constraints:    k * (m+n-2)
             * CPU constraints:                     m * n
             * Meta constraints:                    3 * (m+n) * (m+n) + m + n
             * Binary constraints:                  (m+n) * (m+n)
             ----------------------------------------------------------------------------------
               total:       (k+6)(m+n)^2 + (k+1)(m+n) + m*n
             *************************************************************************************/
            //problem size variables
            int n = substrate_network->getNumNodes();
            int m = vnr->getVN()->getNumNodes();
            int k = vnr->getVN()->getNumLinks();
            int number_of_constraints = (5*(m+n)*(m+n)) + k + k  + (k*(m+n-2)) + (m*n) + m + n;
            int number_of_variables = k*(m+n)*(m+n) + (m+n)*(m+n);
            //int number_of_variables = k*n*n + n*n;
            //id sets
            const std::shared_ptr<const std::set<int>> substrateNodeIdSet = substrate_network->getNodeIdSet();
            const std::shared_ptr<const std::set<int>> substrateLinkIdSet = substrate_network->getLinkIdSet();
            const std::shared_ptr<const std::set<int>> virtualNodeIdSet = vnr->getVN()->getNodeIdSet();
            const std::shared_ptr<const std::set<int>> virtualLinkIdSet = vnr->getVN()->getLinkIdSet();
            
            std::vector <int> allNodeIds (m+n+1);
            auto it = std::copy(substrateNodeIdSet->begin(), substrateNodeIdSet->end(), ++allNodeIds.begin());
            std::copy(virtualNodeIdSet->begin(), virtualNodeIdSet->end(), it);
            
            for (int i=1; i< allNodeIds.size(); i++)
                std::cout<< allNodeIds[i];
            std::cout << std::endl;
            //configs
            bool setAlpha = ConfigManager::Instance()->getConfig<bool>("vineyard.Constants.setAlpha");
            bool setBeta = ConfigManager::Instance()->getConfig<bool>("vineyard.Constants.setBeta");
            
            //rechability checking object
            VYNodesReachabilityCondition reachability_cond;
            
            //Three temporary vectors, which are used to define the LP problem coefficient matrix
            int ai [(number_of_variables) * (number_of_constraints)+1];
            int aj [((number_of_variables) * (number_of_constraints)+1)] ;
            double ar [((number_of_variables) * (number_of_constraints)+1)];
            
            // coeeficent vector (b)
            std::vector<std::vector<double>> bVec ((m+n+1), std::vector<double>(m+n+1));
            
            // index vectors
            std::vector<std::vector<std::vector<int>>> flowVarIndex (k+1, std::vector<std::vector<int>> (m+n+1, std::vector<int> (m+n+1)));
            std::vector<std::vector<int>> indicatorVarIndex (m+n+1, std::vector<int> (m+n+1));
            
            //Set the problem type: Minimization
            glp_set_obj_dir(lp_problem, GLP_MIN);
            
            glp_add_rows(lp_problem, number_of_constraints);
            glp_add_cols(lp_problem, number_of_variables);
            
            glp_set_prob_name(lp_problem, "LP Problem");
            
            
            /********************************************************************************************************************
             ********************************************************************************************************************
                                                        SET MATRIX ROW BANDS
             *******************************************************************************************************************
             ********************************************************************************************************************/
            int rowNumber = 0;
            int colNumber = 0;
            char buff[50];
            
            //first set the column names:
            // f [f, u, v] x [u,v]
            //first k*(m+n)*(m+n) variables are the flow varibles f[f,n,m]
            for (int h=1; h<=k; h++)
            {
                for (int i=1; i<=n; i++)
                {
                    for (int j=1; j<=n; j++)
                    {
                        colNumber += 1;
                        sprintf(buff, "f[f=%d n=%d m=%d]", h,i,j);
                        glp_set_col_name(lp_problem, colNumber, buff);
                        flowVarIndex[h][i][j] = colNumber;
                        //on the last run of the outer loop write the x[u,v] variables
                        if (h==k)
                        {
                            sprintf(buff, "x[n=%d m=%d]", i,j);
                            glp_set_col_name(lp_problem, colNumber+(n*n), buff);
                            indicatorVarIndex[i][j] = colNumber+(n*n);
                        }
                    }
                }
            }
           
            
            
            // capacity conditions bands: (m+n)*(m+n) rows
            // sum{i in F} (f[i, u, v] + f[i, v, u]) <= b[u,v]

            // Meta constraint bands : 3 * (m+n)*(m+n)
            // metcon I {u in NM, v in NM}: sum{i in F} f[i, u, v] <= b[u, v] * x[u, v] ==> f[i, u, v] - b[u,v] * x[u, v] <= 0;
            // metcon II {u in NM, v in NM}: sum{i in F} f[i, u, v] <= b[u, v] * x[v, u] ==> f[i, u, v] - b[u,v] * x[v, u] <= 0;
            // metcon III {u in NM, v in NM}: x[u, v] <= b[u,v];
            
            // Indicator variable constraint bands: (m+n)*(m+n) rows
            // bincon{u in NM, v in NM}: x[u, v] = x[v, u] ==> x[u,v]-x[v,u] = 0;

            for (int i = 1; i <= (m+n); i++)
            {
                for (int j = 1; j <= (m+n); j++)
                {
                    rowNumber += 1;
                    sprintf(buff, "Capacity Constraint %d", (i-1)*(m+n)+j);
                    glp_set_row_name(lp_problem, rowNumber , buff);
                    
                    sprintf(buff, "Meta Constraint I %d", (i-1)*(m+n)+j);
                    glp_set_row_name(lp_problem, rowNumber + (m+n)*(m+n) , buff);
                    
                    sprintf(buff, "Meta Constraint II %d", (i-1)*(m+n)+j);
                    glp_set_row_name(lp_problem, rowNumber + 2 * (m+n)*(m+n), buff);
                    
                    sprintf(buff, "Meta Constraint III %d", (i-1)*(m+n)+j);
                    glp_set_row_name(lp_problem, rowNumber + 3 * (m+n)*(m+n), buff);
                    
                    sprintf(buff, "Indicator Constraint %d", (i-1)*(m+n)+j);
                    glp_set_row_name(lp_problem, rowNumber + 4 * (m+n)*(m+n), buff);
                    glp_set_row_bnds(lp_problem, rowNumber + 4 * (m+n)*(m+n), GLP_DB, 0.0, 0.0);
                    
                    
                    if (i<=n && j<=n)
                    {
                        if (i==j || substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])==nullptr)
                        {
                            glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, 0.0);
                            
                            glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                            glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                            glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                            
                            bVec[i][j] = 0.0;
                        }
                        else
                        {
                            double bw = substrate_network->getLinkBetweenNodes(allNodeIds[i], allNodeIds[j])->getBandwidth();
                            
                            glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, bw);
                            
                            glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, bw);
                            glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, bw);
                            glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, bw);
                            
                            bVec[i][j] = bw;
                        }
                    }
                    else
                    {
                        if (i>n && j<=n)
                        {
                            if (reachability_cond(substrate_network->getNode(allNodeIds[j]), vnr->getVN()->getNode(allNodeIds[i]), vnr->getMaxDistance()))
                            {
                                glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, DBL_MAX);
                                
                                glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, DBL_MAX);
                                glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, DBL_MAX);
                                glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, DBL_MAX);
                                
                                bVec[i][j] = DBL_MAX;
                            }
                            else
                            {
                                glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, 0.0);
                                
                                glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                                glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                                glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                                
                                bVec[i][j] = 0.0;
                            }
                        }
                        else if (i<=n && j>n)
                        {
                            if (reachability_cond(substrate_network->getNode(allNodeIds[i]), vnr->getVN()->getNode(allNodeIds[j]), vnr->getMaxDistance()))
                            {
                                glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, DBL_MAX);
                                
                                glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, DBL_MAX);
                                glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, DBL_MAX);
                                glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, DBL_MAX);
                                
                                bVec[i][j] = DBL_MAX;
                            }
                            else
                            {
                                glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, 0.0);
                                
                                glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                                glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                                glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                                
                                bVec[i][j] = 0.0;
                            }
                        }
                        else // if i>n && j>n
                        {
                            glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, 0.0);
                            
                            glp_set_row_bnds(lp_problem, rowNumber + (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                            glp_set_row_bnds(lp_problem, rowNumber + 2 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                            glp_set_row_bnds(lp_problem, rowNumber + 3 * (m+n)*(m+n), GLP_MAX, 0.0, 0.0);
                            
                            bVec[i][j] = 0.0;
                        }
                    }
                    //set columns:
                    for (int h=1; h<=k; h++)
                    {
                        //Capacity constraint
                        ar[rowNumber * flowVarIndex[h][i][j]] = ar[rowNumber * flowVarIndex[h][j][i]] = 1;
                        
                        //Meta condition I
                        ar[(rowNumber + (m+n)*(m+n))*flowVarIndex[h][i][j]] = 1;
                        ar[(rowNumber + (m+n)*(m+n))*indicatorVarIndex[i][j]] = -bVec[i][j];
                        
                        //Meta condition II
                        ar[(rowNumber + 2*(m+n)*(m+n))*flowVarIndex[h][i][j]] = 1;
                        ar[(rowNumber + 2*(m+n)*(m+n))*indicatorVarIndex[j][i]] = -bVec[i][j];
                    }
                    //Metacondition III
                    ar[(rowNumber + 3*(m+n)*(m+n))*indicatorVarIndex[i][j]] = 1;
                    
                    //Indicator variable
                    ar[(rowNumber + 4*(m+n)*(m+n))*indicatorVarIndex[i][j]] = 1;
                    ar[(rowNumber + 4*(m+n)*(m+n))*indicatorVarIndex[j][i]] = -1;
                }
            }
            //adjust the rowNumber
            rowNumber *= 5;
            
            // meta condition row bands: m rows
            // metcon3{m in M}: sum{w in N} x[m, w] = 1;
            for (int i=1; i<=m; i++)
            {
                rowNumber += 1;
                sprintf(buff, "Meta Constraint IV %d", i);
                glp_set_row_name(lp_problem, rowNumber , buff);
                glp_set_row_bnds(lp_problem, rowNumber, GLP_FX, 1.0 , 0.0);
                
                // set columns : Meta Condition IV
                for (int j=1;j<=n;j++)
                    ar[rowNumber*indicatorVarIndex[n+i][j]] = 1;
            }
            // meta condition row bands: m rows
            // metcon4{w in N}: sum{m in M} x[m, w] <= 1;
            for (int i=1; i<=n; i++)
            {
                rowNumber += 1;
                sprintf(buff, "Meta Constraint V %d", i);
                glp_set_row_name(lp_problem, rowNumber , buff);
                glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0 , 1.0);
                
                // set columns : Meta condition V
                for (int j=1;j<=m;j++)
                    ar[rowNumber*indicatorVarIndex[n+j][i]] = 1;
            }
            // demand satisfaction conditions bands: k rows
            // demsat1{i in F}: sum{w in NM} f[i, fs[i], w] - sum{w in NM} f[i, w, fs[i]] = fd[i];
            // demsat2{i in F}: sum{w in NM} f[i, fe[i], w] - sum{w in NM} f[i, w, fe[i]] = -fd[i];
            for (int i=1; i<=k; i++)
            {
                double flow_bw = (double) vnr->getVN()->getLink(allNodeIds[n+i])->getBandwidth();
                
                rowNumber += 1;
                sprintf(buff, "Demand Constraint I %d", i);
                glp_set_row_name(lp_problem, rowNumber , buff);
                glp_set_row_bnds(lp_problem, rowNumber, GLP_FX, flow_bw, 0.0);
                
                sprintf(buff, "Demand Constraint II %d", i);
                glp_set_row_name(lp_problem, rowNumber+k , buff);
                glp_set_row_bnds(lp_problem, rowNumber+k, GLP_FX, -flow_bw, 0.0);
                
                //set Columns : demand condition I
                auto flowSource = n+std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(allNodeIds[n+i])->getNodeFromId()));
                auto flowDestination = n+std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(allNodeIds[n+i])->getNodeToId()));
                for (int j=1; j<=m+n; j++)
                {
                    ar[rowNumber*flowVarIndex[i][flowSource][j]] = 1;
                    ar[rowNumber*flowVarIndex[i][j][flowSource]] = -1;
                    
                    ar[(rowNumber+k)*flowVarIndex[i][flowDestination][j]] = 1;
                    ar[(rowNumber+k)*flowVarIndex[i][j][flowDestination]] = -1;
                }
                
            }
            //adjust the rowNumber
            rowNumber+=k;
            // flow conservation condition bands: k * (m+n-2) rows
            // flocon{i in F, u in NM diff {fs[i], fe[i]}}: sum{w in NM} f[i, u, w] - sum{w in NM} f[i, w, u] = 0
            int constraing_counter = 0;
            for (int i=1; i<= k; i++)
            {
                for (int j=1; j<= (m+n); j++)
                {
                    auto flowSource = n+std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(allNodeIds[n+i])->getNodeFromId()));
                    auto flowDestination = n+std::distance(virtualNodeIdSet->begin(), virtualNodeIdSet->find(vnr->getVN()->getLink(allNodeIds[n+i])->getNodeToId()));
                    
                    if (j == flowSource || j == flowDestination)
                    {
                        continue;
                    }
                    else
                    {
                        rowNumber += 1;
                        constraing_counter +=1;
                        sprintf(buff, "Flow Conservation Constraint %d", constraing_counter);
                        glp_set_row_name(lp_problem, rowNumber , buff);
                        glp_set_row_bnds(lp_problem, rowNumber, GLP_FX, 0.0, 0.0);
                        
                        //write columns : flow conservation
                        for (int l = 1; l<= m+n; l++)
                        {
                            ar[rowNumber*flowVarIndex[i][j][l]] = 1;
                            ar[rowNumber*flowVarIndex[i][l][j]] = -1;
                        }
                    }
                }
            }
            
            // CPU constraint bands:  m * n rows
            // cpucon{m in M, w in N}: p[w] >= x[m, w] * p[m]; ==> x[m,w]<= p[w]/p[m]
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j<=m; j++)
                {
                    rowNumber += 1;
                    sprintf(buff, "CPU Constraint %d", (i-1)*m + j);
                    glp_set_row_name(lp_problem, rowNumber , buff);
                    glp_set_row_bnds(lp_problem, rowNumber, GLP_DB, 0.0, substrate_network->getNode(allNodeIds[i])->getCPU()/vnr->getVN()->getNode(allNodeIds[j])->getCPU());
                    
                    //set Columns : CPU constraint
                    ar[rowNumber*indicatorVarIndex[n+j][i]] = 1;
                }
            }
            //set ai and ar matrices
            for (int i=1; i<=number_of_constraints; i++)
            {
                for (int j=1; j<=number_of_variables; j++)
                {
                    ai[((i-1)*number_of_variables)+j] = i;
                    aj[((i-1)*number_of_variables)+j] = j;
                }
            }
            //load the glp matrix
            glp_load_matrix(lp_problem, (number_of_constraints)*(number_of_variables), ai, aj, ar);
            
            //set the objective function
            colNumber = 0;
            /*
            for (int h=1; h<=k; h++)
            {
                for (int i=1; i<=n+m; i++)
                {
                    for (int j=1; j<=n+m; j++)
                    {
                        colNumber++;
                        glp_set_obj_coef(lp_problem, colNumber, 1/bVec[i][j])
                        (if h==k)
                        {
                            glp_set_obj_coef(lp_problem, colNumber+(m+n)*(m+n), )
                        }
                    }
                }
            }
             */
            
        }
        template<>
        Embedding_Result VYVineEmbeddingAlgo<>::embeddVNR(std::shared_ptr<VNR_TYPE> vnr)
        {
            setGLPKProblemObject (vnr);
            glp_write_mps(lp_problem, GLP_MPS_FILE, NULL, "/Users/Soroosh/normal_output.txt");
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_NODE_RESOURCES;
        }
    }
}
#endif
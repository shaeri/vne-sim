/**
 * @file grc-node-embedding-algo.cc
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

#include "grc-node-embedding-algo.h"
#include "core/config-manager.h"

namespace vne
{
namespace grc
{

    template <>
    GRCNodeEmbeddingAlgo<>::GRCNodeEmbeddingAlgo()
        : NodeEmbeddingAlgorithm<Network<VYSubstrateNode<>, VYSubstrateLink<>>,
                                 VYVirtualNetRequest<>>(),
          alpha(ConfigManager::Instance()->getConfig<double>("GRC", "alpha")),
          beta(ConfigManager::Instance()->getConfig<double>("GRC", "beta")),
          sigma(ConfigManager::Instance()->getConfig<float>("GRC", "sigma")),
          dampingFactor(ConfigManager::Instance()->getConfig<float>("GRC", "dampingFactor")){};
    //template<>
    //bool GRCNodeEmbeddingAlgo<>::compare_pair (const std::pair<int,double>& first, const std::pair<int,double>& second)
    //{
    //    return ( first.second > second.second );
    //};

    template <>
    GRCNodeEmbeddingAlgo<>::~GRCNodeEmbeddingAlgo(){};

    template <>
    double GRCNodeEmbeddingAlgo<>::calculateNormOfDifference(std::map<int, double> &Mat1,
                                                             std::map<int, double> &Mat2)
    {
        assert(Mat1.size() == Mat2.size());
        double norm = 0.0;
        for (auto it = Mat1.begin(); it != Mat1.end(); it++) {
            assert(Mat2.find(it->first) != Mat2.end());
            norm += it->second - Mat2[it->first];
        }
        return norm;
    };

    template <>
    void GRCNodeEmbeddingAlgo<>::scalarVecMultiplication(float &scalar,
                                                         std::map<int, double> &cMat)
    {
        for (auto it = cMat.begin(); it != cMat.end(); it++) {
            it->second *= scalar;
        }
    };

    template <>
    std::map<int, double> GRCNodeEmbeddingAlgo<>::matrixMultiplication(
        std::map<int, std::map<int, double>> &mMat, std::map<int, double> &rVec)
    {
        std::map<int, double> output;
        for (auto it1 = mMat.begin(); it1 != mMat.end(); it1++) {
            double row_sum = 0.0;
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                assert(it1->second.size() == rVec.size());
                row_sum += it2->second * rVec[it2->first];
            }
            output[it1->first] = row_sum;
        }
        assert(output.size() == rVec.size());
        return output;
    };

    template <>
    std::list<std::pair<int, double>> GRCNodeEmbeddingAlgo<>::calculateSubstrateGRCVector(
        std::shared_ptr<SUBSTRATE_TYPE> substrate_network)
    {
        std::shared_ptr<std::set<int>> substrateIdSet = substrate_network->getNodeIdSet();

        std::map<int, double> cMat;
        std::map<int, std::map<int, double>> mMat;

        double sumSubstrateNodeCPU = 0.0;
        for (auto it = substrateIdSet->begin(); it != substrateIdSet->end(); it++) {
            sumSubstrateNodeCPU += substrate_network->getNode(*it)->getCPU();
        }
        for (auto it = substrateIdSet->begin(); it != substrateIdSet->end(); it++) {
            cMat[*it] = substrate_network->getNode(*it)->getCPU() / sumSubstrateNodeCPU;
        }
        for (auto it1 = substrateIdSet->begin(); it1 != substrateIdSet->end(); it1++) {
            for (auto it2 = substrateIdSet->begin(); it2 != substrateIdSet->end(); it2++) {
                std::shared_ptr<VYSubstrateLink<>> linkBetweenSNs =
                    substrate_network->getLinkBetweenNodes(*it1, *it2);
                if (linkBetweenSNs == nullptr) {
                    mMat[*it1][*it2] = 0.0;
                } else {
                    std::shared_ptr<const std::vector<std::shared_ptr<VYSubstrateLink<>>>>
                        linksConnectedToSN = substrate_network->getLinksForNodeId(*it2);
                    double sumBW = 0.0;
                    for (int i = 0; i < linksConnectedToSN->size(); i++) {
                        sumBW += linksConnectedToSN->at(i)->getBandwidth();
                    }
                    mMat[*it1][*it2] = linkBetweenSNs->getBandwidth() / sumBW;
                }
            }
        }

        double delta = Infinity;
        std::map<int, double> current_r = cMat;
        std::map<int, double> next_r;
        float scalar = 1 - dampingFactor;
        scalarVecMultiplication(scalar, cMat);

        while (delta >= sigma) {
            std::map<int, double> mrMatrix = matrixMultiplication(mMat, current_r);
            scalarVecMultiplication(dampingFactor, mrMatrix);
            assert(mrMatrix.size() == cMat.size());
            for (auto it = cMat.begin(); it != cMat.end(); it++) {
                next_r[it->first] = it->second + mrMatrix[it->first];
            }
            delta = calculateNormOfDifference(next_r, current_r);
            current_r = next_r;
        }

        std::list<std::pair<int, double>> output;
        for (auto it = current_r.begin(); it != current_r.end(); it++) {
            output.push_back(std::make_pair(it->first, it->second));
        }
        output.sort(sort_order());
        return output;
    };

    template <>
    std::list<std::pair<int, double>> GRCNodeEmbeddingAlgo<>::calculateVNRGRCVector(
        std::shared_ptr<VNR_TYPE> vnr)
    {
        std::shared_ptr<std::set<int>> vnIdSet = vnr->getVN()->getNodeIdSet();

        std::map<int, double> cMat;
        std::map<int, std::map<int, double>> mMat;

        double sumVirtualNodeCPU = 0.0;
        for (auto it = vnIdSet->begin(); it != vnIdSet->end(); it++) {
            sumVirtualNodeCPU += vnr->getVN()->getNode(*it)->getCPU();
        }
        for (auto it = vnIdSet->begin(); it != vnIdSet->end(); it++) {
            cMat[*it] = vnr->getVN()->getNode(*it)->getCPU() / sumVirtualNodeCPU;
        }
        for (auto it1 = vnIdSet->begin(); it1 != vnIdSet->end(); it1++) {
            for (auto it2 = vnIdSet->begin(); it2 != vnIdSet->end(); it2++) {
                std::shared_ptr<VYVirtualLink<>> linkBetweenVNs =
                    vnr->getVN()->getLinkBetweenNodes(*it1, *it2);

                if (linkBetweenVNs == nullptr) {
                    mMat[*it1][*it2] = 0.0;
                } else {
                    std::shared_ptr<const std::vector<std::shared_ptr<VYVirtualLink<>>>>
                        linksConnectedToVN = vnr->getVN()->getLinksForNodeId(*it2);
                    double sumBW = 0.0;
                    for (int i = 0; i < linksConnectedToVN->size(); i++) {
                        sumBW += linksConnectedToVN->at(i)->getBandwidth();
                    }
                    mMat[*it1][*it2] = linkBetweenVNs->getBandwidth() / sumBW;
                }
            }
        }

        double delta = Infinity;
        std::map<int, double> current_r = cMat;
        std::map<int, double> next_r;
        float scalar = 1 - dampingFactor;
        scalarVecMultiplication(scalar, cMat);

        while (delta >= sigma) {
            std::map<int, double> mrMatrix = matrixMultiplication(mMat, current_r);
            scalarVecMultiplication(dampingFactor, mrMatrix);
            assert(mrMatrix.size() == cMat.size());
            for (auto it = cMat.begin(); it != cMat.end(); it++) {
                next_r[it->first] = it->second + mrMatrix[it->first];
            }
            delta = calculateNormOfDifference(next_r, current_r);
            current_r = next_r;
        }

        std::list<std::pair<int, double>> output;
        for (auto it = current_r.begin(); it != current_r.end(); it++) {
            output.push_back(std::make_pair(it->first, it->second));
        }
        output.sort(sort_order());
        return output;
    };

    template <>
    Embedding_Result GRCNodeEmbeddingAlgo<>::embeddVNRNodes(
        std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr)
    {
        std::list<std::pair<int, double>> substrateRmat = calculateSubstrateGRCVector(
            substrate_network);
        std::list<std::pair<int, double>> virtualRmat = calculateVNRGRCVector(vnr);
        for (auto itv = virtualRmat.begin(); itv != virtualRmat.end(); itv++) {
            for (auto its = substrateRmat.begin(); its != substrateRmat.end(); its++) {
                // first check the capacity condition
                if (substrate_network->getNode(its->first)->getCPU() >=
                    vnr->getVN()->getNode(itv->first)->getCPU()) {
                    //if Location Constrain need not to be considered add the mapping
                    if (IgnoreLocationConstrain()) {
                        vnr->addNodeMapping(its->first, itv->first);
                        its = substrateRmat.erase(its);
                        break;
                    }
                    // Else check make sure the node is within the required range
                    else {
                        if (vnr->getVN()
                                ->getNode(itv->first)
                                ->getCoordinates()
                                .distanceFrom(
                                    substrate_network->getNode(its->first)->getCoordinates()) <=
                            vnr->getMaxDistance()) {
                            vnr->addNodeMapping(its->first, itv->first);
                            its = substrateRmat.erase(its);
                            break;
                        }
                    }
                }
            }
        }
        if (vnr->getNodeMap()->size() != vnr->getVN()->getNumNodes())
            return Embedding_Result::NOT_ENOUGH_SUBSTRATE_RESOURCES;
        return Embedding_Result::SUCCESSFUL_EMBEDDING;
    };
}  // namespace grc
}  // namespace vne

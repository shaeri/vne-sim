/**
 * @file grc-node-embedding-algo.h
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
 *
 *      This is an implementation of the GRC-VNE (global resource capacity) algorithm.
 *      This algorithm is a node ranking based algorithm:
 *      
 *       [1] L. Gong, Y. Wen, Z. Zhu, and T. Lee, “Toward profit-seeking virtual network
 *           embedding algorithm via global resource capacity.,” INFOCOM, pp. 1--9, 2014.
 *
 **/

#ifndef GRC_GRC_NODE_EMBEDDING_ALGO_
#define GRC_GRC_NODE_EMBEDDING_ALGO_

#include "core/node-embedding-algorithm.h"

#include "Vineyard/vy-substrate-node.h"
#include "Vineyard/vy-substrate-link.h"

#include "Vineyard/vy-virtual-net-request.h"

using namespace vne::vineyard;

namespace vne {
    namespace grc {
        template<typename = Network<VYSubstrateNode<>,VYSubstrateLink<>> ,
        typename = VYVirtualNetRequest<>>
        class GRCNodeEmbeddingAlgo : public NodeEmbeddingAlgorithm
        <Network<VYSubstrateNode<>,VYSubstrateLink<>>, VYVirtualNetRequest<>>
        {
        public:
            GRCNodeEmbeddingAlgo ();
            ~GRCNodeEmbeddingAlgo ();
            virtual Embedding_Result  embeddVNRNodes (std::shared_ptr<SUBSTRATE_TYPE> substrate_network, std::shared_ptr<VNR_TYPE> vnr);
        private:
            float sigma;
            float dampingFactor;
            double alpha;
            double beta;
            //bool compare_pair (const std::pair<int,double>& first, const std::pair<int,double>& second);
            inline std::map<int, double> matrixMultiplication (std::map<int, std::map<int, double>>& mMat, std::map<int, double>& rVec);
            inline void scalarVecMultiplication (float& scalar, std::map<int, double>& cMat);
            inline double calculateNormOfDifference (std::map<int, double>& Mat1, std::map<int, double>& Mat2);
            inline std::list<std::pair<int, double>> calculateSubstrateGRCVector (std::shared_ptr<SUBSTRATE_TYPE> substrate_network);
            inline std::list<std::pair<int, double>> calculateVNRGRCVector (std::shared_ptr<VNR_TYPE> vnr);
            
            struct sort_order
            {
                bool operator()(const std::pair<int,double>& first, const std::pair<int,double>& second) const
                {
                    return ( first.second > second.second );
                }
            };
            
        };
    }
}
#endif /* defined(GRC_GRC_NODE_EMBEDDING_ALGO_) */

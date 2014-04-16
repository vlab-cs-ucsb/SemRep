/*
 * ImageComputer.hpp
 *
 * Copyright (C) 2013-2014 University of California Santa Barbara.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335,
 * USA.
 *
 * Authors: Abdulbaki Aydin, Muath Alkhalaf
 */

#ifndef IMAGECOMPUTER_HPP_
#define IMAGECOMPUTER_HPP_

#include "StringAnalysisTypes.hpp"
#include "exceptions/StrangerStringAnalysisException.hpp"
#include "StrangerAutomaton.hpp"
#include "depgraph/DepGraph.hpp"

class ImageComputer {
public:
	ImageComputer();
	virtual ~ImageComputer();
    static void staticInit();


    /****************************************************************************************************/
    /********* VALIDATION FUNCTION CALCULATION METHODS ************************************************/
    /****************************************************************************************************/

    AnalysisResult doBackwardAnalysis_ValidationCase(DepGraph& origDepGraph, DepGraph& depGraph, StrangerAutomaton* initialAuto);
    void doPreImageComputation_ValidationCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult);
    StrangerAutomaton* makePreImageForOpChild_ValidationCase(DepGraph& depGraph, DepGraphOpNode* opNode,
    			 DepGraphNode* childNode,AnalysisResult& bwAnalysisResult);
    void doPreImageComputationForSCC_ValidationCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult);

    /****************************************************************************************************/
    /*********** SINGLE INPUT POST-IMAGE COMPUTATION METHODS **********************************************/
    /****************************************************************************************************/

    void doForwardAnalysis_SingleInput(DepGraph& origDepGraph,  DepGraphUninitNode* inputNode, AnalysisResult& analysisResult);
    void doForwardAnalysis_SingleInput(DepGraph& origDepGraph,  DepGraph& inputDepGraph, AnalysisResult& analysisResult);
    void doPostImageComputation_SingleInput(DepGraph& origDepGraph,  DepGraph& inputDepGraph, DepGraphNode* node, AnalysisResult& analysisResult);

    /****************************************************************************************************/
    /*********** GENERAL PRE-IMAGE COMPUTATION METHODS *********************************************/
    /****************************************************************************************************/

    AnalysisResult doBackwardAnalysis_GeneralCase(DepGraph& origDepGraph, DepGraph& inputDepGraph, StrangerAutomaton* initialAuto, const AnalysisResult& fwAnalysisResult);
    void doPreImageComputation_GeneralCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult);
    StrangerAutomaton* makePreImageForOpChild_GeneralCase(DepGraph& depGraph, DepGraphOpNode* opNode, DepGraphNode* childNode,AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult);
    void doPreImageComputationForSCC_GeneralCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult);
    /****************************************************************************************************/
    /*********** GENERAL POST-IMAGE COMPUTATION METHODS ************************************************************************/
    /****************************************************************************************************/

    void doForwardAnalysis_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult);
    void doPostImageComputation_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult);
    StrangerAutomaton* makePostImageForOp_GeneralCase(DepGraph& depGraph, DepGraphOpNode* opNode, AnalysisResult& analysisResult);
    void doPostImageComputationForSCC_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult);



    static PerfInfo* perfInfo;

    bool handle_concats = false;

private:
    static bool initialized;

    static StrangerAutomaton* uninit_node_default_initialization;
    NodesList f_unmodeled;
    std::string getLiteralOrConstantValue(DepGraphNode* node);
    bool isLiteralOrConstant(DepGraphNode* node, NodesList successors);
    /**
     *
     * TODO pattern for __vlab_restrict and other replace operations handled differently. There are some cases not handled yet for this reason where a pattern variable flows into both functions.
     */
    StrangerAutomaton* getLiteralorConstantNodeAuto(DepGraphNode* node, bool is_vlab_restrict);

};


#endif /* IMAGECOMPUTER_HPP_ */

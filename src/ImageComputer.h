/*
 * ImageComputer.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef IMAGECOMPUTER_H_
#define IMAGECOMPUTER_H_

#include "StringAnalysisTypes.h"
#include "StrangerStringAnalysisException.h"
#include "DepGraph.h"
#include "StrangerAutomaton.hpp"
#include "PerfInfo.h"
#include "StringBuilder.h"
#include <string>
#include <map>
#include <vector>
#include <stack>




class ImageComputer {
public:
	ImageComputer();
	virtual ~ImageComputer();
    static void staticInit();


    /****************************************************************************************************/
    /********* VALIDATION PATCH EXTRACTION PHASE METHODS ************************************************/
    /****************************************************************************************************/

    AnalysisResult doBackwardAnalysis_ValidationPhase(DepGraph& origDepGraph,
                                                                DepGraph& inputDepGraph,
                                                                 NodesList& sortedNodes);
    void doBackwardNodeComputation_ValidationPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph,
                                   AnalysisResult& bwAnalysisResult, DepGraphNode* node);

    StrangerAutomaton* makeBackwardAutoForOpChild_ValidationPhase(DepGraph& depGraph, DepGraphOpNode* opNode,
    			 DepGraphNode* childNode,AnalysisResult& bwAnalysisResult);


    /****************************************************************************************************/
    /*********** REGULAR FORWARD IMAGE COMPUTATION METHODS **********************************************/
    /****************************************************************************************************/

    void doForwardAnalysis_RegularPhase(DepGraph& origDepGraph,  DepGraph& inputDepGraph, NodesList& sortedNodes, AnalysisResult& analysisResult);

    void doForwardNodeComputation_RegularPhase(DepGraph& origDepGraph,  DepGraph& inputDepGraph, DepGraphNode* node, AnalysisResult& analysisResult);

    StrangerAutomaton* makeForwardAutoForOp_RegularPhase(DepGraph& depGraph, DepGraphOpNode* opNode, AnalysisResult& analysisResult);


    /****************************************************************************************************/
    /*********** REGULAR BACKWARD IMAGE COMPUTATION METHODS *********************************************/
    /****************************************************************************************************/

    AnalysisResult doBackwardAnalysis_RegularPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph, NodesList& sortedNodes, StrangerAutomaton* initialAuto, const AnalysisResult& fwAnalysisResult);

    void doBackwardNodeComputation_RegularPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph, DepGraphNode* node,
                                       AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult);

    StrangerAutomaton* makeBackwardAutoForOpChild_RegularPhase(DepGraph& depGraph, DepGraphOpNode* opNode,
			 DepGraphNode* childNode,AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult);


    /****************************************************************************************************/
    /*********** OLD METHODS *********************************************/
    /****************************************************************************************************/

    AnalysisResult computeFwImage(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                                  /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                                  bool multiTrack);
    void doForwardAnalysis(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                           /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                           bool multiTrack, AnalysisResult& analysisResult);
    void doNodeComputation(DepGraph& acyclicWorkGraph, DepGraph& origDepGraph,
                           AnalysisResult& analysisResult, DepGraphNode* node,
                           AnalysisResult& inputValuesMap, bool multiTrack);
    StrangerAutomaton* makeAutoForOp(DepGraphOpNode* opNode, AnalysisResult& analysisResult, DepGraph& depGraph, bool multiTrack);
    
//********************************************************************************************************************

    AnalysisResult computeBwImage(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                                  /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                                  StrangerAutomaton* intersectionAuto,
                                  DepGraphNode* inputNode,
                                  AnalysisResult& fwAnalysisResult);

    AnalysisResult doBackwardAnalysis(DepGraph& origDepGraph, DepGraph& inputDepGraph,
                                      /*SccNodes& sccNodes,*/
                                      NodesList& sortedNodes,
                                      StrangerAutomaton* intersectionAuto,
                                      const AnalysisResult& fwAnalysisResult);
    void doBackwardNodeComputation(DepGraph& inputDepGraph, DepGraph& origDepGraph,
                                   AnalysisResult& bwAnalysisResult,
                                   const AnalysisResult& fwAnalysisResult,
                                   DepGraphNode* node,
                                   bool multiTrack);

    StrangerAutomaton* makeBackwardAutoForOpChild(DepGraphOpNode* opNode,
                                                                        DepGraphNode* childNode,
                                                  AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult, DepGraph& depGraph, boolean fixPoint);

    static PerfInfo* perfInfo;
    void calculateNodeAutomaton(DepGraph& origDepGraph, AnalysisResult& analysisResult, DepGraphNode* node);
    void doPostImageComputation(DepGraph& origDepGraph, AnalysisResult& analysisResult, DepGraphNode* node);
private:
    static int numOfProcessedNodes;
    static bool initialized;
    static int debugLevel;
    static int autoDebugLevel;
    static StrangerAutomaton* uninit_node_default_initialization;
    NodesList f_unmodeled;
    std::string getLiteralValue(DepGraphNode* node);
    bool isLiteral(DepGraphNode* node, NodesList successors);

    /**
     * If a successor does not have a corresponding automaton yet, calculate it doing a
     * post order tree-traversal starting from that node
     *
     */
//    void calculateNodeAutomaton(DepGraph& origDepGraph, AnalysisResult& AnalysisResult, DepGraphNode* node);
};

#endif /* FORWARDIMAGECOMPUTER_H_ */

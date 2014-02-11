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
#include <set>
#include <queue>




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

    /****************************************************************************************************/
    /*********** GENERAL POST-IMAGE COMPUTATION METHODS ************************************************************************/
    /****************************************************************************************************/

    void doForwardAnalysis_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult);
    void doPostImageComputation_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult);
    StrangerAutomaton* makePostImageForOp_GeneralCase(DepGraph& depGraph, DepGraphOpNode* opNode, AnalysisResult& analysisResult);

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

#endif /* FORWARDIMAGECOMPUTER_H_ */

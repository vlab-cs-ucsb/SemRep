///*
// * ForwardImageComputer.cpp
// *
// *  Created on: Aug 19, 2013
// *      Author: muath
// */

#include "ForwardImageComputer.h"

using namespace std;

ForwardImageComputer::ForwardImageComputer() {
	if (!initialized){
        throw StrangerStringAnalysisException("Forward Image Computer has not been initialized.");
    }

}

ForwardImageComputer::~ForwardImageComputer() {
	// TODO Auto-generated destructor stub
}

int ForwardImageComputer::numOfProcessedNodes = 0;
bool ForwardImageComputer::initialized = false;
/**
 * 1- prints the dot format for result automatons
 * 2- prints the MONA internal states for result automatons
 */
int ForwardImageComputer::autoDebugLevel = 1;
int ForwardImageComputer::debugLevel = 1;
PerfInfo ForwardImageComputer::perfInfo;

//static
void ForwardImageComputer::staticInit() {
    initialized =  true;
    StrangerAutomaton::perfInfo = ForwardImageComputer::perfInfo;
    StrangerAutomaton::staticInit();
}

//static
void ForwardImageComputer::debug(string s, int dlevel) {
    if (debugLevel >= dlevel) {
        cout << s;
    }
}

void ForwardImageComputer::debugAuto(StrangerAutomaton* dfa, int dlevel, int printlevel){
    if (autoDebugLevel >= dlevel) {
        if (autoDebugLevel >= dlevel) {
            debug("", 0);
            dfa->printAutomatonVitals();
            debug("", 0);
        }
        debug("", 0);
        dfa->toDot();
        debug("", 0);
    }
}

void ForwardImageComputer::debugMemoryUsage(int dlevel){
    if (debugLevel >= dlevel)
        PerfInfo::printMemoryUsage();
}

/*******************************************************************************************************************************/
/********* VALIDATION PATCH EXTRACTION PHASE METHODS ***************************************************************************/
/*******************************************************************************************************************************/

/**
 *
 * Initial Pre-Image computation for validation check
 * 1- Start from first __vlab_restrict function
 * 2- Union negation of all restrict functions, (in case third parameter is true, you do not need to negate since it is already negated)
 * 3- If there are some other ops after first restrict, do pre-image computation for them and intersect result.
 */
AnalysisResult ForwardImageComputer::doBackwardAnalysis_ValidationPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph, NodesList& sortedNodes) {

	long start_validation_phase = perfInfo.currentTimeMillis();
	AnalysisResult bwValidationPatchResult;

	// initially all nodes are bottom, we are computing the rejected sets.
	for (DepGraphNode* node: origDepGraph.getNodes()){
		bwValidationPatchResult[node->getID()] = StrangerAutomaton::makePhi(node->getID());
	}

	NodesListConstReverseIterator it;
	for (it = sortedNodes.rbegin(); it != sortedNodes.rend(); it++ ) {
		DepGraphNode* node = (DepGraphNode*)(*it);
		if (dynamic_cast< DepGraphNormalNode*>(node) || dynamic_cast< DepGraphUninitNode*>(node) || dynamic_cast< DepGraphOpNode*>(node)) {

			if (dynamic_cast< DepGraphOpNode*>(node) ) {
				DepGraphOpNode* op = dynamic_cast< DepGraphOpNode*>(node);
				// if the operation is __vlab_restrict, handle first restrict function, special case here
				if (op->getName().find("__vlab_restrict") != string::npos) {
					break;
				}
			}

		} else {
			throw new StrangerStringAnalysisException("SNH: cannot handle node type");
		}
	}

	if (it != sortedNodes.rend()) { // if it is not NULL it can't be the last node, now continue analysis with the rest of the nodes
		while ( ++it != sortedNodes.rend()) {
			DepGraphNode* node = (DepGraphNode*)(*it);
			doBackwardNodeComputation_ValidationPhase(origDepGraph, inputDepGraph, bwValidationPatchResult, node);
		}
	} else {
		// basically server accepts anystring
		cout << "\tVALIDATION STEP: no __vlab_restrict function, do not have any validation!" << endl;
	}
	long stop_validation_phase = perfInfo.currentTimeMillis();
	perfInfo.validation_phase_time = stop_validation_phase - start_validation_phase;
	return bwValidationPatchResult;
}


/**
 * Do the computation for a node
 * Get the predecessors, and compute the pre-image based on those
 */
void ForwardImageComputer::doBackwardNodeComputation_ValidationPhase(DepGraph& origDepGraph, DepGraph& inputDepGraph,
                               AnalysisResult& bwAnalysisResult, DepGraphNode* node) {

	debug(stringbuilder() << "doInitialBackwardNodeComputaion for node: " << node->getID(), 2);
	NodesList predecessors = origDepGraph.getPredecessors(node);
	NodesList successors = origDepGraph.getSuccessors(node);
	StrangerAutomaton *newAuto = NULL, *tempAuto = NULL;
	DepGraphNormalNode* normalNode = NULL;

	if (dynamic_cast< DepGraphNormalNode*>(node) || dynamic_cast< DepGraphUninitNode*>(node) || dynamic_cast< DepGraphOpNode*>(node)) {
		if (predecessors.empty()) {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: node " << node->getID() << " does not have predecessors. ");
		} else if (successors.empty() && (normalNode = dynamic_cast< DepGraphNormalNode*>(node))) {
			TacPlace* place = normalNode->getPlace();
			if (dynamic_cast<Literal*>(place)) { // assuming we have literals other than uninit node
				newAuto = StrangerAutomaton::makeString(place->toString(), node->getID());
			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "(Unhandled node type with 0 successors) SNH: " << place << ", " << normalNode->getFileName() << "," <<
						normalNode->getOrigLineNo());
			}
		} else {

			for (NodesListIterator it = predecessors.begin(); it != predecessors.end(); ++it) {
				DepGraphNode* pred = *it;
				StrangerAutomaton* predAuto = NULL;
				if (pred == node) {
					// ignore simple self loop (check correctness)
					continue;
				} else if (dynamic_cast< DepGraphNormalNode*>(pred)) {
					predAuto = bwAnalysisResult.find( pred->getID() )->second;
				} else if (dynamic_cast< DepGraphOpNode*>(pred)) {
					predAuto = this->makeBackwardAutoForOpChild_ValidationPhase(origDepGraph, dynamic_cast< DepGraphOpNode*>(pred), node, bwAnalysisResult);
				}

				if (predAuto == NULL) {
					throw StrangerStringAnalysisException("SNH: Predecessor Auto is Null: doBackwardNodeComputation_ValidationPhase()");
				}

				if (newAuto == NULL) {
					newAuto = predAuto->clone(node->getID());
				} else {
					tempAuto = newAuto;
					newAuto = newAuto->union_(predAuto, node->getID());
					delete tempAuto;
					delete predAuto;
				}
			}

		}

	} else {
		throw new StrangerStringAnalysisException("SNH: cannot handle node type: doBackwardNodeComputation_ValidationPhase()");
	}

	if (newAuto == NULL) {
		throw new StrangerStringAnalysisException("SNH: pre-image is NULL: doBackwardNodeComputation_ValidationPhase()");
	}

	bwAnalysisResult[node->getID()] = newAuto;
	debug("--------------------------", 2);

}


/**
 * Do pre-image computation for validation patch
 */
StrangerAutomaton* ForwardImageComputer::makeBackwardAutoForOpChild_ValidationPhase(DepGraph& depGraph, DepGraphOpNode* opNode,
			 DepGraphNode* childNode, AnalysisResult& bwAnalysisResult) {

//	cout << "operation function is called" << endl;
	StrangerAutomaton* retMe = NULL;
	NodesList successors = depGraph.getSuccessors(opNode);
	StrangerAutomaton* opAuto = bwAnalysisResult[opNode->getID()];
	string opName = opNode->getName();

	if (!opNode->isBuiltin()) {
		// __vlab_restrict
        if (opName.find("__vlab_restrict") != string::npos) {

			if (successors.size() != 3) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict invalid number of arguments: "
						"makeBackwardAutoForOpChild_ValidationPhase()");
			}

//			cout << "\t~~~~~>>> __vlab_restrict validation phase " << endl;
			DepGraphNode* subjectNode = successors[1];
			DepGraphNode* patternNode = successors[0];
			DepGraphNode* complementNode = successors[2];

			if (childNode->equals(subjectNode)) {
				DepGraphNormalNode* pNode = dynamic_cast<DepGraphNormalNode*>(patternNode);
				if (pNode == NULL) {
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find pattern node: "
							"makeBackwardAutoForOpChild_ValidationPhase()");
				}

				Literal* patternLit = dynamic_cast<Literal*>(pNode->getPlace());
				if (patternLit == NULL) {
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find literal as pattern node: "
							"makeBackwardAutoForOpChild_ValidationPhase()");
				}
				string regString = patternLit->getLiteralValue();

				if (regString.find_first_of('/') == 0 &&
						regString.find_last_of('/') == (regString.length() -1) ) {
					regString = regString.substr( 1,regString.length()-2);
				}

				if(regString.find_first_of('^') == 0 &&
						regString.find_last_of('$') == (regString.length() -1)){
					regString = "/" + regString.substr( 1,regString.length()-2) + "/";
				}
				else {
					regString = "/.*(" + regString + ").*/";
				}


				StrangerAutomaton* regx = StrangerAutomaton::regExToAuto(regString, true, patternNode->getID());

				DepGraphNormalNode* cNode = dynamic_cast<DepGraphNormalNode*>(complementNode);
				if (cNode == NULL) {
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find complement node: "
							"makeBackwardAutoForOpChild_ValidationPhase()");
				}

				Literal* complementLit = dynamic_cast<Literal*>(cNode->getPlace());
				if (complementLit == NULL) {
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find literal as complement node: "
							"makeBackwardAutoForOpChild_ValidationPhase()");
				}

				// Union __vlab_restricts considering complement parameter
				string complementString = complementLit->getLiteralValue();
				if (complementString.find("false") != string::npos || complementString.find("FALSE") != string::npos) {
					StrangerAutomaton* complementAuto = regx->complement(patternNode->getID());

					retMe = opAuto->union_(complementAuto, childNode->getID());
					delete complementAuto;
				} else {
					retMe = opAuto->union_(regx, childNode->getID());
				}

				delete regx;
//				return retMe;

			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of __vlab_restrict (" << opNode->getID() << ") is not in backward path,\ncheck implementation: "
						"makeBackwardAutoForOpChild_ValidationPhase()");
			}
        } else {
        	throw StrangerStringAnalysisException(stringbuilder() << "SNH: function " << opName << " is not __vlab_restrict.\n"
        			"makeBackwardAutoForOpChild_ValidationPhase()");
        }

	} else if (opName == ".") {
		// CONCAT
		throw StrangerStringAnalysisException( "concats are not handled until we really need: "
				"makeBackwardAutoForOpChild_ValidationPhase()");
	} else if (opName == "addslashes") {
		// only has one parameter ==>  string addslashes  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = StrangerAutomaton::addslashes(sigmaStar, opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = StrangerAutomaton::pre_addslashes(intersection, childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;
//		return retMe;
	} else if (opName == "trim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = sigmaStar->trimSpaces(opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = intersection->preTrimSpaces(childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;
//		return retMe;
	} else if (opName == "htmlspecialchars") {
		//TODO handle all params, currently we only consider first param
		if (childNode->equals(successors[0])) {
			StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
			StrangerAutomaton* forward = StrangerAutomaton::htmlSpecialChars(sigmaStar, opNode->getID());
			StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
			retMe = StrangerAutomaton::preHtmlSpecialChars(intersection, childNode->getID());
			delete sigmaStar;
			delete forward;
			delete intersection;
		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of htmlspecialchars (" << opNode->getID() << ") is not in backward path,\ncheck implementation: "
					"makeBackwardAutoForOpChild_ValidationPhase()");
		}

	} else if (opName == "strip_tags") {
		cout << endl << "!!!!! strip_tags is not implemented yet" << endl;
		retMe = bwAnalysisResult.find( childNode->getID() )->second;
	} else if (opName == "md5") {
		cout << endl << "!!!!! md5 cannot be implemented, continues with previous node results" << endl;
		retMe = bwAnalysisResult.find( childNode->getID() )->second;
	} else {
		throw StrangerStringAnalysisException( "Not implemented yet for validation phase: " + opName);
	}

	return retMe;

}

/*******************************************************************************************************************************/
/*********** SANITIZATION PATCH EXTRACTION METHODS *****************************************************************************/
/*******************************************************************************************************************************/

/**
 * TODO implemented for simple dep graphs, extend it for more general cases as needed
 * 1 - Do forward analysis from an input to sink
 */
void ForwardImageComputer::doForwardAnalysis_RegularPhase(
		DepGraph& origDepGraph, DepGraph& inputDepGraph, NodesList& sortedNodes, AnalysisResult& analysisResult) {

//	for (DepGraphNode* node: origDepGraph.getNodes()){
//		if (analysisResult.find(node->getID()) == analysisResult.end()) {
//			analysisResult[node->getID()] = StrangerAutomaton::makePhi(node->getID());
//		}
//	}

	for (NodesListIterator it = sortedNodes.begin(); it != sortedNodes.end(); it++) {
		DepGraphNode* node = *it;
		if (analysisResult.find(node->getID()) == analysisResult.end()) {
			analysisResult[node->getID()] = StrangerAutomaton::makePhi(node->getID());
		}
		doForwardNodeComputation_RegularPhase(origDepGraph, inputDepGraph, node, analysisResult);
	}
	return;
}

void ForwardImageComputer::doForwardNodeComputation_RegularPhase(
		DepGraph& origDepGraph, DepGraph& inputDepGraph, DepGraphNode* node, AnalysisResult& analysisResult) {

	if(node->getNonprocessedParents() == -1){
		node->setNonprocessedParents(origDepGraph.getPredecessors(node).size());
	}

	NodesList successors = origDepGraph.getSuccessors(node);

	StrangerAutomaton* newAuto = NULL;
    DepGraphNormalNode* normalnode;
    DepGraphOpNode* opNode;
    DepGraphUninitNode* uninitNode;

    if ((normalnode = dynamic_cast<DepGraphNormalNode*>(node)) != NULL) {
    	if (successors.empty()) {
    		TacPlace* place = normalnode->getPlace();
    		Literal* lit = dynamic_cast<Literal*>(place);
    		if (lit == NULL) {
    			throw StrangerStringAnalysisException(stringbuilder() << "SNH: " << normalnode->getID() << ","
    										<< "cannot handle, expecting literal node,\nor add this type to the implementation: "
    												"doForwardNodeComputation_RegularPhase()");
    		}
    		newAuto = StrangerAutomaton::makeString(lit->getLiteralValue(), node->getID());
    	} else {
    		// an interior node, union of all its successors
    		for (NodesListIterator it = successors.begin(); it != successors.end(); it++) {
    			DepGraphNode* succNode = *it;
    			if (succNode->getID() == node->getID() ) {
    				// avoid simple loops
    				continue;
    			}

    			AnalysisResultIterator rit = analysisResult.find(succNode->getID());
    			if (rit == analysisResult.end()) {
    				// if this is the case, either use the originial graph instead of input relevant graph
    				// or update the case here, compute all paths that passes through this node.
    				cout << endl << "!!! Not all successors of a normal node(" << node->getID() << ") is computed, not computed node("<< succNode->getID() <<") !!!:\n"
    						"doForwardNodeComputation_RegularPhase()" << endl;
    				continue;
//    				throw StrangerStringAnalysisException("Successor of normal Node is not computed yet!\nUpdate implementation: "
//    						"doForwardNodeComputation_RegularPhase()");

    			}
    			StrangerAutomaton *succAuto = rit->second;
    			if (newAuto == NULL) {
    				newAuto = succAuto->clone(node->getID());
    			} else {
    				StrangerAutomaton* temp = newAuto;
    				newAuto = newAuto->union_(succAuto, node->getID());
    				delete temp;
    			}
    		}
    	}

    } else if ((opNode = dynamic_cast<DepGraphOpNode*>(node)) != NULL) {
		newAuto = makeForwardAutoForOp_RegularPhase(origDepGraph, opNode, analysisResult);
    } else if ((uninitNode = dynamic_cast<DepGraphUninitNode*>(node)) != NULL) {
    	// input nodes should already have been initialized
    	if (analysisResult.find(node->getID()) == analysisResult.end()){
    		throw StrangerStringAnalysisException("An Uninitialized input node found without initial automaton!\n"
    				"doForwardNodeComputation_RegularPhase()");
    	}
    	newAuto = analysisResult[node->getID()];
    } else {
    	throw StrangerStringAnalysisException("Cannot handle node type!\ncheck or update code: "
    			"doForwardNodeComputation_RegularPhase()");
    }

    if (newAuto == NULL) {
    	throw StrangerStringAnalysisException("Forward automaton cannot be computed!\ncheck or update code: "
    			"doForwardNodeComputation_RegularPhase()");
    }

    analysisResult[node->getID()] = newAuto;
}

/**
 * Forward image computation for operation nodes
 */
StrangerAutomaton* ForwardImageComputer::makeForwardAutoForOp_RegularPhase(
		DepGraph& depGraph, DepGraphOpNode* opNode, AnalysisResult& analysisResult) {

	NodesList successors = depGraph.getSuccessors(opNode);
	StrangerAutomaton* retMe = NULL;
	string opName = opNode->getName();

	if (!opNode->isBuiltin()) {
		// __vlab_restrict
		if (opName.find("__vlab_restrict") != string::npos) {
			if (successors.size() != 3) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict invalid number of arguments: "
						"makeForwardAutoForOp_RegularPhase()");
			}

			DepGraphNode* subjectNode = successors[1];
			DepGraphNode* patternNode = successors[0];
			DepGraphNode* complementNode = successors[2];


			AnalysisResultIterator rit = analysisResult.find(subjectNode->getID());
			if (rit == analysisResult.end()) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find subject auto: "
										"makeForwardAutoForOp_RegularPhase()");
			}

			StrangerAutomaton* subjectAuto = rit->second;

			DepGraphNormalNode* pNode = dynamic_cast<DepGraphNormalNode*>(patternNode);
			if (pNode == NULL) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find pattern node: "
						"makeForwardAutoForOp_RegularPhase()");
			}

			Literal* patternLit = dynamic_cast<Literal*>(pNode->getPlace());
			if (patternLit == NULL) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find literal as pattern node: "
						"makeForwardAutoForOp_RegularPhase()");
			}
			string regString = patternLit->getLiteralValue();

			if (regString.find_first_of('/') == 0 &&
					regString.find_last_of('/') == (regString.length() -1) ) {
				regString = regString.substr( 1,regString.length()-2);
			}

			if(regString.find_first_of('^') == 0 &&
					regString.find_last_of('$') == (regString.length() -1)){
				regString = "/" + regString.substr( 1,regString.length()-2) + "/";
			}
			else {
				regString = "/.*(" + regString + ").*/";
			}


			StrangerAutomaton* regx = StrangerAutomaton::regExToAuto(regString, true, patternNode->getID());

			DepGraphNormalNode* cNode = dynamic_cast<DepGraphNormalNode*>(complementNode);
			if (cNode == NULL) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find complement node: "
						"makeForwardAutoForOp_RegularPhase()");
			}

			Literal* complementLit = dynamic_cast<Literal*>(cNode->getPlace());
			if (complementLit == NULL) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict cannot find literal as complement node: "
						"makeForwardAutoForOp_RegularPhase()");
			}

			// Union __vlab_restricts considering complement parameter
			string complementString = complementLit->getLiteralValue();
			if (complementString.find("false") != string::npos || complementString.find("FALSE") != string::npos) {
				retMe = subjectAuto->intersect(regx, opNode->getID());
			} else {
				StrangerAutomaton* complementAuto = regx->complement(patternNode->getID());
				retMe = subjectAuto->intersect(complementAuto, opNode->getID());
				delete complementAuto;
			}
			delete regx;
//			return retMe;

		} else {
        	throw StrangerStringAnalysisException(stringbuilder() << "SNH: function " << opName << " is not __vlab_restrict.\n"
        			"makeForwardAutoForOp_RegularPhase()");
        }
	} else if (opName == ".") {
		// We will stop at concatenation if the concat is for query string in sink
		// Simply forward the node that has a calculated value
		for (NodesListIterator it = successors.begin(); it != successors.end(); it++){
			DepGraphNode* succNode = *it;
			AnalysisResultIterator rit = analysisResult.find(succNode->getID());
			if (rit != analysisResult.end()) {
				StrangerAutomaton* succAuto = rit->second;
				if (retMe == NULL) {
					retMe = succAuto->clone(opNode->getID());
				} else {
					StrangerAutomaton* temp = retMe;
					retMe = retMe->concatenate(succAuto, opNode->getID());
					delete temp;
				}
			}
		}
		if (retMe == NULL) {
			throw StrangerStringAnalysisException("Check successors of concatenation: "
									"makeForwardAutoForOp_RegularPhase()");
		}

	} else if (opName == "preg_replace") {
		throw StrangerStringAnalysisException("Implement preg_replace: "
				"makeForwardAutoForOp_RegularPhase()");
	} else if (opName == "ereg_replace") {
		throw StrangerStringAnalysisException("ereg_replace is not supported yet: "
						"makeForwardAutoForOp_RegularPhase()");
	} else if (opName == "str_replace") {
		throw StrangerStringAnalysisException("Implement str_replace: "
								"makeForwardAutoForOp_RegularPhase()");
		 //Note: subjectAuto should be a string to use the replace function
		 //from StrangerLibrary. Otherwise we need a method to return the
		 //set of strings that an automaton represents (in PHP replace
		 //should always have only one string.

		if (successors.size() < 3) {
			throw new StrangerStringAnalysisException("SNH");
		}
		StrangerAutomaton* subjectAuto = analysisResult[successors[2]->getID()];
		StrangerAutomaton* searchAuto = analysisResult[successors[0]->getID()];
		string replaceStr = analysisResult[successors[1]->getID()]->getStr();

		StrangerAutomaton* replacement = StrangerAutomaton::str_replace(
				searchAuto, replaceStr, subjectAuto, opNode->getID());
		retMe = replacement;
//		return retMe;
	} else if (opName == "addslashes") {
		if (successors.size() != 1) {
			throw new StrangerStringAnalysisException("addslashes should have one child: "
					"makeForwardAutoForOp_RegularPhase()");
		}
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

		StrangerAutomaton* slashesAuto = StrangerAutomaton::addslashes(
				paramAuto, opNode->getID());
		retMe = slashesAuto;
//		return retMe;
	} else if (opName == "stripslashes") {
		throw new StrangerStringAnalysisException("stripslashes is not working: "
										"makeForwardAutoForOp_RegularPhase()");
		 //this is not a precise model but rather an overapproximation
//		if (successors.size() != 1) {
//			throw new StrangerStringAnalysisException("stripslashes should have one child: "
//								"makeForwardAutoForOp_RegularPhase()");
//		}
//		 //only has one parameter
//		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
//		StrangerAutomaton* slashesAuto = StrangerAutomaton::stripslashes(
//				paramAuto, opNode->getID());
//		retMe = slashesAuto;
//		return retMe;
	} else if (opName == "mysql_escape_string") {
		if (successors.size() < 1 || successors.size() > 2) {
			throw new StrangerStringAnalysisException("mysql_escape_string wrong number of arguments: "
					"makeForwardAutoForOp_RegularPhase()");
		}
        //we only care about the first parameter
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

		StrangerAutomaton* mysqlEscapeAuto = StrangerAutomaton::mysql_escape_string(
				paramAuto, opNode->getID());
		retMe = mysqlEscapeAuto;

	} else if (opName == "mysql_real_escape_string") {
		if (successors.size() < 1 || successors.size() > 2) {
			throw new StrangerStringAnalysisException("mysql_real_escape_string wrong number of arguments: "
					"makeForwardAutoForOp_RegularPhase()");
		}
        //we only care about the first parameter
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

		StrangerAutomaton* mysqlEscapeAuto = StrangerAutomaton::mysql_real_escape_string(
				paramAuto, opNode->getID());
		retMe = mysqlEscapeAuto;

	} else if (opName == "htmlspecialchars") {
		//TODO handle all params, currently we only consider first param
		if (analysisResult.find(successors[0]->getID()) != analysisResult.end()) {
			//we only care about the first parameter
			StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

			StrangerAutomaton* htmlSpecAuto = StrangerAutomaton::htmlSpecialChars(paramAuto, opNode->getID());
			retMe = htmlSpecAuto;

		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: successors[0] of htmlspecialchars (" << opNode->getID() << ") is not calculated,\ncheck implementation: "
					"makeBackwardAutoForOpChild_ValidationPhase()");
		}

	} else if (opName == "nl2br"){

		if (successors.size() < 1 || successors.size() > 2) {
			throw new StrangerStringAnalysisException("nl2br wrong number of arguments: "
					"makeForwardAutoForOp_RegularPhase()");
		}
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		StrangerAutomaton* nl2brAuto = StrangerAutomaton::nl2br(
				paramAuto, opNode->getID());
		retMe = nl2brAuto;
//		return retMe;
	} else if (opName == "trim" || opName == "strtoupper" || opName == "strtolower") {
        if (successors.size() != 1) {
			throw new StrangerStringAnalysisException(stringbuilder() << opName << " has more than one successor in depgraph.\n"
					"makeForwardAutoForOp_RegularPhase()" );
		}
		 //TODO: quickly check if this correct
		 //each is treated as a function and it does not do anything
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

        if (opName == "trim")
            retMe = paramAuto->trimSpaces(opNode->getID());
        else if (opName == "strtoupper") {
        	retMe = paramAuto->toUpperCase(opNode->getID());
        }
        else if (opName == "strtolower") {
        	retMe = paramAuto->toLowerCase(opNode->getID());
        }

//		return retMe;
	} else if (opName == "md5") {
		StrangerAutomaton* succAuto = analysisResult[successors[0]->getID()];
		retMe = succAuto->clone(opNode->getID());
	} else {
		cout << "!!! Warning: Unmodeled builtin general function : " << opName;
		f_unmodeled.push_back(opNode);

		 //conservative decision for operations that have not been
		 //modeled yet: .*
		retMe = StrangerAutomaton::makeAnyString(opNode->getID());
	}
	return retMe;
}

/*******************************************************************************************************************************/
/*********** REGULAR BACKWARD IMAGE COMPUTATION METHODS *************************************************************************/
/*******************************************************************************************************************************/

/**
 * Giving an initial auto for root, using the results from previous forward analysis,
 * do a backward analysis.
 *
 */
AnalysisResult ForwardImageComputer::doBackwardAnalysis_RegularPhase(
		DepGraph& origDepGraph, DepGraph& inputDepGraph, NodesList& sortedNodes, StrangerAutomaton* initialAuto, const AnalysisResult& fwAnalysisResult) {

	AnalysisResult bwAnalysisResult;
	// initially all nodes are bottom
	for (DepGraphNode* node: origDepGraph.getSortedNodes()){
		bwAnalysisResult[node->getID()] = StrangerAutomaton::makePhi(node->getID());
	}

	// initialize root node
	bwAnalysisResult[inputDepGraph.getRoot()->getID()] = initialAuto;

	// skipping root do backward analysis for the rest
	for (NodesListConstReverseIterator it = sortedNodes.rbegin() + 1; it != sortedNodes.rend(); it++ ) {
		DepGraphNode* node = (DepGraphNode*)(*it);

		doBackwardNodeComputation_RegularPhase(origDepGraph, inputDepGraph, node, bwAnalysisResult, fwAnalysisResult);

	}

	return bwAnalysisResult;
}

/**
 *
 */
void ForwardImageComputer::doBackwardNodeComputation_RegularPhase(
		DepGraph& origDepGraph, DepGraph& inputDepGraph, DepGraphNode* node,
		AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult) {

	debug(stringbuilder() << "doRegularBackwardNodeComputaion for node: " << node->getID(), 2);
	NodesList predecessors = origDepGraph.getPredecessors(node);
	NodesList successors = origDepGraph.getSuccessors(node);
	DepGraphNormalNode* normalNode = NULL;
	StrangerAutomaton *newAuto = NULL, *tempAuto = NULL;

	if (dynamic_cast< DepGraphNormalNode*>(node) || dynamic_cast< DepGraphUninitNode*>(node) || dynamic_cast< DepGraphOpNode*>(node)) {
		if (predecessors.empty()) {
			throw StrangerStringAnalysisException(stringbuilder() << "Node(" << node->getID() << ") does not have predecessors.\n"
					"doBackwardNodeComputation_RegularPhase()");
		} else if (successors.empty() && (normalNode = dynamic_cast< DepGraphNormalNode*>(node))) {
			TacPlace* place = normalNode->getPlace();
			if (dynamic_cast<Literal*>(place)) { // assuming we have literals other than uninit node
				newAuto = StrangerAutomaton::makeString(place->toString(), node->getID());
			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "(Unhandled node type with 0 successors) SNH: " << place << ", " << normalNode->getFileName() << "," <<
						normalNode->getOrigLineNo() << "\ndoBackwardNodeComputation_RegularPhase()");
			}
		} else {
			// the automa is union of all prodecessors and interstect with forward analysis result
			StrangerAutomaton* forwardAuto = fwAnalysisResult.find(node->getID())->second;
			for (NodesListIterator it = predecessors.begin(); it != predecessors.end(); ++it) {
				DepGraphNode* pred = *it;
				StrangerAutomaton* predAuto = NULL;
				if (pred == node) {
					// ignore simple self loop (check correctness)
					continue;
				} else if (dynamic_cast< DepGraphNormalNode*>(pred)) {
					predAuto = bwAnalysisResult.find( pred->getID() )->second;
				} else if (dynamic_cast< DepGraphOpNode*>(pred)) {
					predAuto = this->makeBackwardAutoForOpChild_RegularPhase(origDepGraph,dynamic_cast< DepGraphOpNode*>(pred), node,
													bwAnalysisResult, fwAnalysisResult);
				}

				if (predAuto == NULL) {
					continue;
				}

				if (newAuto == NULL) {
					newAuto = predAuto->clone(node->getID());
				} else {
					tempAuto = newAuto;
					newAuto = newAuto->union_(predAuto, node->getID());
					delete tempAuto;
					delete predAuto;
				}
			}

			if (newAuto == NULL) {
				throw StrangerStringAnalysisException("Cannot calculate backward auto, fix me\nndoBackwardNodeComputation_RegularPhase()");
			}
			tempAuto = newAuto;
			newAuto = newAuto->intersect(forwardAuto, node->getID());
			delete tempAuto;
		}

	} else {
		throw new StrangerStringAnalysisException("SNH: cannot handle node type:\ndoBackwardNodeComputation_RegularPhase()");
	}

	if (newAuto == NULL) {
		throw new StrangerStringAnalysisException("SNH: pre-image is NULL:\ndoBackwardNodeComputation_RegularPhase()");
	}

	bwAnalysisResult[node->getID()] = newAuto;
	debug("--------------------------", 2);
}

/**
 *
 */
StrangerAutomaton* ForwardImageComputer::makeBackwardAutoForOpChild_RegularPhase(
		DepGraph& depGraph, DepGraphOpNode* opNode, DepGraphNode* childNode,
		AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult) {

	StrangerAutomaton* retMe = NULL;
	NodesList successors = depGraph.getSuccessors(opNode);
	StrangerAutomaton* opAuto = bwAnalysisResult[opNode->getID()];
	string opName = opNode->getName();


	if (!opNode->isBuiltin()) {
		// __vlab_restrict
		if (opName.find("__vlab_restrict") != string::npos) {

			if (successors.size() != 3) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: __vlab_restrict invalid number of arguments: "
						"makeBackwardAutoForOpChild_RegularPhase()");
			}

			DepGraphNode* subjectNode = successors[1];
			DepGraphNode* patternNode = successors[0];
			DepGraphNode* complementNode = successors[2];

			if (childNode->equals(subjectNode)){
				retMe = opAuto->clone(childNode->getID());
			} else if (childNode->equals(patternNode) || childNode->equals(complementNode)) {
				throw StrangerStringAnalysisException(stringbuilder() << "child node (" << childNode->getID() << ") of __vlab_restrict (" << opNode->getID() << ") should not be handled,\ncheck implementation: "
						"makeBackwardAutoForOpChild_RegularPhase()");
			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of __vlab_restrict (" << opNode->getID() << ") is not in backward path,\ncheck implementation: "
						"makeBackwardAutoForOpChild_RegularPhase()");
			}
		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: function " << opName << " is not __vlab_restrict.\n"
					"makeBackwardAutoForOpChild_RegularPhase()");
		}

	} else if (opName == ".") {
		// CONCAT, forward analysis does not do concat actually
		if (successors.size() < 2)
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: less than two successors for concat node " << opNode->getID() << ""
					"\nmakeBackwardAutoForOpChild_RegularPhase()");

		StrangerAutomaton* concatAuto = opAuto;

		DepGraphNode* leftSibling = successors[0];
		DepGraphNode* rightSibling = successors[1];

		AnalysisResultConstIterator leftIt = fwAnalysisResult.find(leftSibling->getID());
		AnalysisResultConstIterator rightIt = fwAnalysisResult.find(rightSibling->getID());

		if (childNode->equals(leftSibling)){
			if (leftIt == fwAnalysisResult.end()) {
				throw StrangerStringAnalysisException(stringbuilder() << "Should not visit left node(" << leftSibling->getID() << ") in concat"
						"\nmakeBackwardAutoForOpChild_RegularPhase()");
			} else if (rightIt == fwAnalysisResult.end()) {
				// we can just clone the previous auto, in that case actual concat operation is not done during forward analysis
				retMe = concatAuto->clone(childNode->getID());
			} else {
				if (isLiteral(rightSibling, depGraph.getSuccessors(rightSibling))){
					string value = getLiteralValue(rightSibling);
					retMe = concatAuto->leftPreConcatConst(value, childNode->getID());
				} else {
					StrangerAutomaton* rightSiblingAuto = fwAnalysisResult.find(rightSibling->getID())->second;
					retMe = concatAuto->leftPreConcat(rightSiblingAuto, childNode->getID());
				}
			}

		} else if (childNode->equals(rightSibling)){

			if (rightIt == fwAnalysisResult.end()) {
				throw StrangerStringAnalysisException(stringbuilder() << "Should not visit right node(" << leftSibling->getID() << ") in concat"
						"\nmakeBackwardAutoForOpChild_RegularPhase()");
			} else if (leftIt == fwAnalysisResult.end()) {
				// we can just clone the previous auto, in that case actual concat operation is not done during forward analysis
				retMe = concatAuto->clone(childNode->getID());
			} else {
				if (isLiteral(leftSibling, depGraph.getSuccessors(leftSibling))){
					string value = getLiteralValue(leftSibling);
					retMe = concatAuto->rightPreConcatConst(value, childNode->getID());
				} else {
					StrangerAutomaton* leftSiblingAuto = fwAnalysisResult.find(leftSibling->getID())->second;
					retMe = concatAuto->rightPreConcat(leftSiblingAuto, childNode->getID());
				}
			}

		}else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: child (" << childNode->getID() << ") of concat (" << opNode->getID() << ") is not equal to any of the two successors."
					"\nmakeBackwardAutoForOpChild_RegularPhase()");
		}

	} else if (opName == "addslashes") {
		// only has one parameter ==>  string addslashes  ( string $str  )
		retMe = StrangerAutomaton::pre_addslashes(opAuto,childNode->getID());

//		retMe = opAuto->clone(childNode->getID());
	} else if (opName == "trim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		retMe = opAuto->preTrimSpaces(childNode->getID());

	} else if (opName == "htmlspecialchars") {
		//TODO handle all params, currently we only consider first param
		if (childNode->equals(successors[0])) {
			retMe = StrangerAutomaton::preHtmlSpecialChars(opAuto, childNode->getID());
		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of htmlspecialchars (" << opNode->getID() << ") is not in backward path,\ncheck implementation: "
					"makeBackwardAutoForOpChild_ValidationPhase()");
		}
	} else if (opName == "mysql_escape_string") {
		// has one parameter
		retMe = StrangerAutomaton::pre_mysql_escape_string(opAuto, childNode->getID());
	} else if (opName == "md5") {
		cout << endl << "!!!!! md5 cannot be implemented, continues with previous node results" << endl;
		retMe = bwAnalysisResult.find( childNode->getID() )->second;
	} else {
		throw StrangerStringAnalysisException( "Not implemented yet for regular validation phase: " + opName);
	}

	return retMe;


}

/*******************************************************************************************************************************/
/*********** REGULAR FORWARD IMAGE COMPUTATION METHODS *************************************************************************/
/*******************************************************************************************************************************/

AnalysisResult ForwardImageComputer::computeFwImage(
                DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                bool multiTrack){

    AnalysisResult analysisResult;
		// used to remove the auto from deco after processing all its parents
		numOfProcessedNodes = 0;

		perfInfo.addGraphInfo(origDepGraph.getRoot()->dotName()
				, origDepGraph.getNumOfNodes(), origDepGraph.getNumOfEdges());

    StrangerAutomaton::debugToFile(stringbuilder() << "//*******************************************************************************\n");
    StrangerAutomaton::debugToFile(stringbuilder() << "// starting a new SINK analysis: " << origDepGraph.getRoot()->dotName());
    StrangerAutomaton::debugToFile(stringbuilder() << "// *******************************************************************************\n");

		cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
		cout << "Starting analysis for SINK: " + origDepGraph.getRoot()->dotName();
		cout << "-----------------------------------------------------------------------------------------------------------------\n" << endl;

		// dump the dependency graph
		// graphcount is needed to provide uniqueness to names of dot graphs
		string fileName = acyclicWorkGraph.getRoot()->dotFileName();//MyOptions.entryFile.getName();
		string graphNameBase = stringbuilder() << "stranger_" << fileName;
		origDepGraph.dumpDot(stringbuilder() << graphNameBase);
//			acyclicWorkGraph.dumpDot(graphNameBase + "_workgraph",
//					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
//			acyclicWorkGraph.dumpSortedDot(graphNameBase + "_sorted_workgraph",
//					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
//			minGraph.dumpDot(graphNameBase + "_min_graph_",
//					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);

		/*******************************************************************/
		//do the actual forward analysis
        doForwardAnalysis(
				origDepGraph, acyclicWorkGraph, /*sccNodes,*/ inputValuesMap, multiTrack, analysisResult);
		/*******************************************************************/
    
    return analysisResult;
	}
	//*****************************************************************************/





//
void ForwardImageComputer::doForwardAnalysis(
                DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                bool multiTrack, AnalysisResult& analysisResult){

		// reset the timer
		long startForward = perfInfo.currentTimeMillis();

		// Start decorating nodes. Note that we decorate nodes in
		// topological order
    
    DepGraphNode* node;
    NodesList sortedNodes = acyclicWorkGraph.getSortedNodes();
		for (NodesListIterator it = sortedNodes.begin();
             it != sortedNodes.end();
             ++it) {
            node = *it;
            if (analysisResult.find(node->getID()) == analysisResult.end())
                analysisResult[node->getID()] = StrangerAutomaton::makePhi(node->getID());
			// if an SCC node then do the fix point computation
//			if (node instanceof DepGraphSccNode) {
//				DepGraphSccNode sccNode = (DepGraphSccNode) node;
//				doFixPointComputationForSCC(acyclicWorkGraph, origDepGraph, forwardDeco,
//						sccNode, sccNodes, multiTrack);
//			} else {// a regular node in the graph
				doNodeComputation(acyclicWorkGraph, origDepGraph, analysisResult, node, inputValuesMap, multiTrack);
				//debugAuto(forwardDeco.get(node));
				//perfInfo.printInfo();
//			}
			//debugMemoryUsage();
			//debug("--------------------------", 2);
		}

		long stopForward = perfInfo.currentTimeMillis();
		if (!multiTrack)
			perfInfo.forwardTime = (stopForward - startForward);
		else
			perfInfo.multiTime = (stopForward - startForward);
	}
//
//	//******************************************************************************************/
AnalysisResult ForwardImageComputer::computeBwImage(DepGraph& origDepGraph, DepGraph& acyclicWorkGraph,
                           /*SccNodes& sccNodes,*/ AnalysisResult& inputValuesMap,
                           StrangerAutomaton* intersectionAuto, DepGraphNode* inputNode,
                                                    AnalysisResult& fwAnalysisResult){
    AnalysisResult bwAnalysisResult;

		cout << "\n***  Stranger Sanit Backward Analysis BEGIN ***\n" << endl;

		string graphNameBase;
    long startBackward = perfInfo.currentTimeMillis();


		DepGraph inputDepGraph = acyclicWorkGraph.getInputRelevantGraph(inputNode);

		 int inputDepGraphsCount = 1;
		 graphNameBase = stringbuilder() << "stranger_" << origDepGraph.getRoot()->getFileName() << inputDepGraphsCount;
		 inputDepGraph.dumpDot(stringbuilder() << graphNameBase << "_inputdepgraph");
	//						 inputDepGraph.dumpSortedDot(graphNameBase + "_sorted_inputdepgraph",
	//									MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);

    NodesList sortedNodes = inputDepGraph.getSortedNodes();
		 bwAnalysisResult = doBackwardAnalysis(origDepGraph, inputDepGraph, /*sccNodes,*/ sortedNodes, intersectionAuto, fwAnalysisResult);
		 string dotName;
    if (dynamic_cast<DepGraphUninitNode*>(inputNode) != NULL){
        const DepGraphNode* temp = *(origDepGraph.getPredecessors(inputNode).begin());
        dotName = temp->dotName();
    }
		 else
			 dotName = inputNode->dotName();
		 cout << "\nBackward analysis automaton result for input node ==> " << dotName << "  ID=" << inputNode->getID() << " :" << endl;
		 cout << "----------------------------" << endl;
		 //backwardDeco.get(node).printAutomaton();
		 debugAuto(bwAnalysisResult[inputNode->getID()], 0, autoDebugLevel);
		 //outputDotAuto(backwardDeco.get(node));
		 //backwardDeco.get(node).printAutomatonVitals();
		 cout << "----------------------------" << endl;

		 long stopBackward = perfInfo.currentTimeMillis();
		 perfInfo.backwardTime += stopBackward - startBackward;

		 cout << "\n***  Stranger Sanit Backward Analysis End ***\n\n" << endl;

		 return bwAnalysisResult;
	}

//	//******************************************************************************************/
//	private void backwardAnalysisMultiInput(int graphcount, DepGraph origDepGraph,
//			DepGraph acyclicWorkGraph,
//			Map<DepGraphSccNode, List<DepGraphNode>> sccNodes,
//			Map<DepGraphNode, StrangerAutomaton> forwardDeco,
//			StrangerAutomaton intersectionAuto, Map<DepGraphNode, StrangerAutomaton> inputValuesMap) {
//
//			String fileName;
//			String graphNameBase;
//
//			 long startBackward = System.currentTimeMillis();
//
//
//
//
//
//			List<DepGraphNode> inputNodes = new ArrayList<DepGraphNode>();
//			inputNodes.addAll(inputValuesMap.keySet());
//			List<DepGraph> inputDepGraphs = acyclicWorkGraph.getInputRelevantGraphs(inputNodes );
//
//			 int inputDepGraphsCount = 1;
//			 for (DepGraph inputDepGraph: inputDepGraphs){
//				 fileName = MyOptions.entryFile.getName();
//				 graphNameBase = name + "stranger_" + fileName + "_" + graphcount + "_" + inputDepGraphsCount;
//				 inputDepGraph.dumpDot(graphNameBase + "_inputdepgraph",
//							MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
////						 inputDepGraph.dumpSortedDot(graphNameBase + "_sorted_inputdepgraph",
////									MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
//
//				 Map<DepGraphNode, StrangerAutomaton> backwardDeco = new HashMap<DepGraphNode, StrangerAutomaton>();
//				 backwardDeco = doBackwardAnalysis(origDepGraph, inputDepGraph, sccNodes, inputDepGraph.getSortedNodes(), intersectionAuto, forwardDeco);
//				 for (DepGraphNode node : backwardDeco.keySet()){
//					 if (inputNodes.contains(node)){
//						 String dotName = null;
//						 if (node instanceof DepGraphUninitNode)
//							 dotName = origDepGraph.getPredecessors(node).iterator().next().dotName();
//						 else
//							 dotName = node.dotName();
//						 cout << "\nBackward analysis automaton result for input node ==> " + dotName + "  ID=" + node.getID() + " :" << endl;
//						 cout << "----------------------------" << endl;
//						 debugAuto(backwardDeco.get(node), 1, 4);
//						 cout << "----------------------------" << endl;
//					 }
//				 }
//				 inputDepGraphsCount++;
//			 }
//
//			 long stopBackward = System.currentTimeMillis();
//			 perfInfo.backwardTime = stopBackward - startBackward;
//
//	}
//	//************************************************************************/
//	public List<Integer> detectVulns(DepClient depClient) {
//
//		/**************************************************/
//		VulnInfo vulnInfo = doTaintAnalysis(depClient);
//		/**************************************************/
//
//		List<DepGraph> vulnDepGraphs = vulnInfo.getDepGraphs();
//
//
//		cout << endl;
//		cout << "*****************" << endl;
//		System.out.println(name.toUpperCase()
//				+ " Stranger Sanit Analysis BEGIN" << endl;
//		cout << "*****************" << endl;
//		cout << endl;
//
//		// dump the automaton that represents the undesired stuff
//		// TODO: it is good to have this for stranger
//		// this.dumpDotAuto(this.undesir, "undesired_" + name,
//		// MyOptions.graphPath);
//
//		// info for dynamic analysis
//		int graphcount = 0;
//
//		int vulnCount = 0;
//
//		for (DepGraph origDepGraph : vulnDepGraphs) {
//			try {
//				long start = System.currentTimeMillis();
//
//
//				f_unmodeled = new HashSet<DepGraphOpNode>();
//				graphcount++;
//				DepGraph acyclicWorkGraph = new DepGraph(origDepGraph);
//				// get a mapping between each SCC nodes and nodes in its cycle
//				Map<DepGraphSccNode, List<DepGraphNode>> sccNodes = acyclicWorkGraph
//				.sort(origDepGraph);
//				// This is to open a file where we have a ctrace for our analysis
//				prepare_ctrace(acyclicWorkGraph, name);
//
//				// input nodes are nodes that contribute untrusted input to the program:
//				// 1- predecessors of tainted uninit nodes (input comes from HTTP GET or POST)
//				// 2- nodes for untrusted input functions. Input functions are functions used
//				// 	  to get user input such as DB function mysql_fetch_array.
//				//    We only add the function if it is not trusted
//				List<DepGraphNode> inputNodes = getHTTPInputNodes(origDepGraph, acyclicWorkGraph);
//				getUntrustedInputFuncsNodes(acyclicWorkGraph, inputNodes);
//
//
//				// even if there is no input we may need to run the analysis
//				// This is in case we have unmodeled functions which cased
//				// the sink to be tainted (although there is no dangerous input)
//				if (inputNodes.isEmpty())
//					cout << "\ninputNodes is empty in string analysis phase (no input)!!\n" << endl;
//
//
//				// initially we will take the most general case which is annotating the input with
//				// sigma*. Later one when starting backward analysis this should change
//				Map<DepGraphNode, StrangerAutomaton> inputValuesMap = annotateInputWithSigmaStar(inputNodes);
//
//
//				/**************************************************/
//				Map<DepGraphNode, StrangerAutomaton> forwardDeco = forwardAnalysis(
//						graphcount, origDepGraph, acyclicWorkGraph, sccNodes, inputValuesMap, false);
//				/**************************************************/
//
//				long startIntersection = System.currentTimeMillis();
//				// intersect root automaton with the undesired stuff;
//				// if the intersection is empty, it means that we are safe!
//				StrangerAutomaton rootAuto = forwardDeco.get(acyclicWorkGraph.getRoot());
//				rootAuto.toDot();
//				if (rootAuto.checkIntersection(this.undesir, rootAuto.getID(),
//						this.undesir.getID())) {
//
//					cout << "\n!!!    A vulnerability has been found in SINK:     !!!" << endl;
//					System.out.println(acyclicWorkGraph.getRoot().toString()+"\n" << endl;
//
//					StrangerAutomaton intersectionAuto = rootAuto.intersect(this.undesir, -1);
//
//					long stopIntersection = System.currentTimeMillis();
//					perfInfo.forwardTime += (stopIntersection - startIntersection);
//
//					debug("\nIntersection result: \n-------------------------", 2);
//					debugAuto(intersectionAuto, 2, 4);
//					debug("", 2);
//					printIntersectionPerfInfo(intersectionAuto);
//
//					vulnCount++;
//
//					/**************************************************/
//					if (MyOptions.optionBackward){
//						if (inputNodes.isEmpty()){
//							// we may have vulnerability with no input when we have unmodeled
//							// function causing the vulnerability (the only source for Sigma*)
//							cout << "\nNo input\n" << endl;
//						}
//						//						if (inputNodes.size() != 1){
//						//							cout << "\nMultiple inputs\n" << endl;
//						//							backwardAnalysisMultiInput(graphcount, origDepGraph, acyclicWorkGraph, sccNodes, forwardDeco, intersectionAuto, inputValuesMap);
//						//						}
//						else if (inputNodes.size() != 1){
//							cout << "\nMultiple inputs: "+inputNodes.size()+"\n" << endl;
//							multiTrackFWAnalysis(graphcount, origDepGraph, acyclicWorkGraph, sccNodes, intersectionAuto, inputValuesMap);
//						}
//						else {
//							cout << "\nSingle input\n" << endl;
//							// if we have one input then do regular backward analysis
//							if (inputValuesMap.keySet().size() == 1)
//								backwardAnalysisSingleInput(graphcount, origDepGraph, acyclicWorkGraph,
//										sccNodes, forwardDeco, intersectionAuto, inputValuesMap.keySet().iterator().next());
//
//							//							else {
//							//								// current node is the current input node we are addigning input value to it
//							//								LinkedList<DepGraphNode> workQ = new LinkedList<DepGraphNode>();
//							//								// initialize the work queue and values for inputs
//							//								// nodes => epsilon
//							//								for (DepGraphNode node: inputValuesMap.keySet()){
//							//									inputValuesMap.put(node, StrangerAutomaton::makeEmptyString(-1));
//							//									workQ.addLast(node);
//							//
//							//								}
//							//								while (!workQ.isEmpty()){
//							//									DepGraphNode currentNode = workQ.removeFirst();
//							//									inputValuesMap.put(currentNode, StrangerAutomaton::makeAnyString(-1));
//							//									forwardDeco = doForwardAnalysis(origDepGraph, acyclicWorkGraph, sccNodes, inputValuesMap, false);
//							//									rootAuto = forwardDeco.get(acyclicWorkGraph.getRoot());
//							//									if (rootAuto.checkIntersection(this.undesir, rootAuto.getID(),
//							//											this.undesir.getID())) {
//							//										intersectionAuto = rootAuto.intersect(this.undesir, -1);
//							//										StrangerAutomaton currentResult = backwardAnalysisSingleInput(graphcount, origDepGraph, acyclicWorkGraph, sccNodes, forwardDeco, intersectionAuto, currentNode);
//							//										if (StrangerAutomaton::makeAnyString(-1).checkEquivalence(currentResult, -1, -1))
//							//											throw new StrangerStringAnalysisException("Can not calculate backward analysis for node "+ currentNode.dotName());
//							//										else
//							//											inputValuesMap.put(currentNode, currentResult.complement(-1));
//							//									}
//							//
//							//								}
//						}
//					}
//
//
//
//				} else {
//					// no vulnerabilities
//					cout << "\n(: No vulnerabilities. SINK is secure :)\n" << endl;
//				}
//
//
//				/***************************************/
//				/** get performance info for this sink **/
//				long stop = System.currentTimeMillis();
//				perfInfo.sinkRunningTime = stop - start;
//				printPerfInfo();
//				// we gather perf into per depgraph (per sink)
//				resetPerfInfo();
//				/***************************************/
//
//				for (StrangerAutomaton auto: forwardDeco.values()){
//					auto.Free();
//				}
//
//				// close the ctrace that we use for debugging
//				close_ctrace();
//
//			} catch (StrangerStringAnalysisException e) {
//				e.printStackTrace();
//				// close the ctrace that we use for debugging and mention that there is an error
//				close_ctrace_with_error(e.toString());
//			}
//
//
//		}
//
//
//		// initial sink count and final graph count may differ (e.g., if some
//		// sinks
//		// are not reachable)
//		cout << "----------------------------" << endl;
//		if (MyOptions.optionV) {
//			cout << "Total Graph Count: " + graphcount);
//		}
//		cout << "Total Vuln Count: " + vulnCount);
//		cout << "----------------------------" << endl;
//
//
//		cout << endl;
//		cout << "*****************" << endl;
//		System.out.println(name.toUpperCase()
//				+" Stranger Sanit Analysis END" << endl;
//		cout << "*****************" << endl;
//		cout << endl;
//
//		// only to output c trace file. One file per all sink places(depgraphs)
//		//StrangerAutomaton::closeCtraceFile();
//
//		return new LinkedList<Integer>();
//	}
//
//	// **************************************************************************************
//
//	private void multiTrackFWAnalysis(int graphcount, DepGraph origDepGraph,
//			DepGraph acyclicWorkGraph,
//			Map<DepGraphSccNode, List<DepGraphNode>> sccNodes,
//			StrangerAutomaton intersectionAuto,
//			Map<DepGraphNode, StrangerAutomaton> inputValuesMap) {
//		// initialize number of tracks to number of inputs
//		StrangerMTrackAutomaton.initializeIndices(inputValuesMap.size());
//		int trackNum = 0;
//		for (DepGraphNode node: inputValuesMap.keySet()){
//			inputValuesMap.put(node, StrangerMTrackAutomaton.makeInputMTAuto(trackNum++, -1));
//		}
//
//		// used to remove the auto from deco after processing all its parents
//		numOfProcessedNodes = 0;
//
//		perfInfo.addGraphInfo(origDepGraph.getRoot().dotName()
//				, origDepGraph.getNumOfNodes(), origDepGraph.getNumOfEdges());
//
//		cout << "\n\n*** " +name.toUpperCase()
//				+ " Stranger Sanit Multitrack Analysis Begin ***\n\n" << endl;
//
//		// dump the dependency graph
//		// graphcount is needed to provide uniqueness to names of dot graphs
//		String fileName = acyclicWorkGraph.getRoot().dotFileName();//MyOptions.entryFile.getName();
//		String graphNameBase = name + "stranger_" + graphcount + "_" + fileName;
//		origDepGraph.dumpDot(graphNameBase + "_depgraph", MyOptions.graphPath,
//				origDepGraph.getUninitNodes(), this.dci);
////			acyclicWorkGraph.dumpDot(graphNameBase + "_workgraph",
////					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
////			acyclicWorkGraph.dumpSortedDot(graphNameBase + "_sorted_workgraph",
////					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
////			minGraph.dumpDot(graphNameBase + "_min_graph_",
////					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
//
//		/*******************************************************************/
//		//do the actual forward analysis
//		Map<DepGraphNode, StrangerAutomaton> forwardDeco = doForwardAnalysis(
//				origDepGraph, acyclicWorkGraph, sccNodes, inputValuesMap, true);
//		/*******************************************************************/
//
//		long startForward = System.currentTimeMillis();
//		StrangerMTrackAutomaton attackPatternMT = StrangerMTrackAutomaton.singleToMTAuto(this.undesir, -1);
////		cout << "debugging attack pattern." << endl;
////		cout << "*************************" << endl;
////		cout << "Single input attack pattern" << endl;
////		this.undesir.toDot();
////		cout << "Multiple input attack pattern" << endl;
////		attackPatternMT.toDot();
////		cout << "*************************" << endl;
//
//		StrangerMTrackAutomaton rootAuto = (StrangerMTrackAutomaton) forwardDeco.get(acyclicWorkGraph.getRoot());
//		//rootAuto.toDot();
//		StrangerMTrackAutomaton intersectionMultiAuto = rootAuto.intersect(attackPatternMT, -1);
//		//intersectionMultiAuto.toDot();
//		intersectionMultiAuto = intersectionMultiAuto.removeLastTrack(-1);
//		long stopForward = System.currentTimeMillis();
//		perfInfo.multiTime += (stopForward - startForward);
//		debugAuto(intersectionMultiAuto, 0, autoDebugLevel);
//
//		int num_of_tracks = inputValuesMap.size();
//		for (int i = 0; i < num_of_tracks; i++){
//			cout << "\nMatch auto for input "+i);
//			intersectionMultiAuto.getTrack(i, num_of_tracks, -1).toDot();
//		}
//
//		cout << "\n\n*** " +name.toUpperCase()
//				+ " Stranger Sanit Multitrack Analysis End ***\n\n" << endl;
//
//	}
//
//	// **************************************************************************************
//
//	private void close_ctrace_with_error(String exception) {
//		StrangerAutomaton::debugToFile("\n//Sorry, an exception happened: "+exception + "\n" << endl;
//		StrangerAutomaton::closeCtraceFile();
//
//
//	}
//
//	// **************************************************************************************
//
//	private void close_ctrace() {
//		StrangerAutomaton::closeCtraceFile();
//
//	}
//
//	// **************************************************************************************
//
//	private void prepare_ctrace(DepGraph acyclicWorkGraph, String graphcount) {
//		String fileName = acyclicWorkGraph.getRoot().dotFileName();//MyOptions.entryFile.getName();
//		String ctraceNameBase = MyOptions.graphPath + "/" + name + "stranger_" + graphcount + "_" + fileName + ".exec_trace.c";
//		StrangerAutomaton::openCtraceFile(ctraceNameBase);
//	}
//	// **************************************************************************************
//	private void resetPerfInfo() {
//		perfInfo.reset();
//
//	}
//	// **************************************************************************************
//
//	private void printIntersectionPerfInfo(StrangerAutomaton intersectionAuto) {
//		cout << "====================================" << endl;
//
//		if (intersectionAuto != null){
//			System.out.print("vitals for intersection" << endl;
//			intersectionAuto.printAutomatonVitals();
//		}
//		cout << "======================================" << endl;
//
//	}
//	// **************************************************************************************
//
//	private void printPerfInfo() {
//		cout << "\n============================================================================"+"\n" << endl;
//		cout << "                         Performance information"+"\n" << endl;
//		cout << "============================================================================"+"\n" << endl;
//		System.out.print("vitals for attack pattern" << endl;
//		this.undesir.printAutomatonVitals();
//		debug(perfInfo.getInfo(), 1);
//		debugMemoryUsage(1);
//		cout << "============================================================================"+"\n" << endl;
//	}
//
//// **************************************************************************************
//	// Adds untrusted input functions that can be used to get untrusted user input to inputNodes
//	// For example: if database is untrusted then return functions that read DB
//	// Note: functions' names for input functions are hard coded in DepClientInfo class
//private void getUntrustedInputFuncsNodes(DepGraph acyclicWorkGraph,
//			List<DepGraphNode> inputNodes) {
//		ArrayList<String> untrustedInputFuncsNames = new ArrayList<String>();
//		// for input functions get the untrusted ones
//		for (String funcName: this.dci.getFunctionModels().getInputFuncs()){
//			// if functions is modeled by user to be untrusted
//			if(isEvil(funcName))
//				untrustedInputFuncsNames.add(funcName);
//		}
//		Set<DepGraphOpNode> untrustedInputFuncsNodes = acyclicWorkGraph.getFuncsNodes(untrustedInputFuncsNames);
//		if (!untrustedInputFuncsNodes.isEmpty())
//			throw new StrangerStringAnalysisException("Input is comming from DB" << endl;
//		for (DepGraphNode node: untrustedInputFuncsNodes){
//			inputNodes.add(node);
//		}
//
//	}
//// **************************************************************************************
//
//private Map<DepGraphNode, StrangerAutomaton> annotateInputWithSigmaStar(
//			List<DepGraphNode> inputNodes) {
//		Map<DepGraphNode, StrangerAutomaton> inputValuesMap = new HashMap<DepGraphNode, StrangerAutomaton>();
//		for (DepGraphNode inputNode: inputNodes){
//			inputValuesMap.put(inputNode, StrangerAutomaton::makeAnyString(-1));
//		}
//		return inputValuesMap;
//	}
//
////******************************************************************
//
//public List<Integer> detectVulns2(DepClient depClient) {
///*
//
//		cout << endl;
//		cout << "*****************" << endl;
//		System.out.println(name.toUpperCase()
//				+ " Taint Analysis BEGIN" << endl;
//		cout << "*****************" << endl;
//		cout << endl;
//
//		// let the basic taint analysis (at.ac.tuwien.infosys.www.pixy.XSSAnalysis & SQLAnalysis) do the preliminary work
//		VulnInfo vulnInfo = depClient.detectAlternative();
//		List<DepGraph> vulnDepGraphs = vulnInfo.getDepGraphs();
//		List<DepGraph> minDepGraphs = vulnInfo.getDepGraphsMin();
//
//		// stats
//		int scanned = vulnInfo.getInitialGraphCount();
//		int reported_by_basic = vulnDepGraphs.size();
//
//		cout << endl;
//		System.out.println(name.toUpperCase()
//				+ " Taint Analysis RESULT:" << endl;
//		cout << "--------------------------" << endl;
//
//		cout << "Number of analyzed sinks: " + scanned);
//		cout << "Number of tainted sinks: " + reported_by_basic);
//
//		long start = System.currentTimeMillis();
//
//		cout << endl;
//		cout << "*****************" << endl;
//		System.out.println(name.toUpperCase()
//				+ " Stranger Sanit Analysis BEGIN" << endl;
//		cout << "*****************" << endl;
//		cout << endl;
//
//		// dump the automaton that represents the undesired stuff
//		// TODO: it is good to have this for stranger
//		// this.dumpDotAuto(this.undesir, "undesired_" + name,
//		// MyOptions.graphPath);
//
//		// info for dynamic analysis
//		StringBuilder dynInfo = new StringBuilder();
//
//		int dynpathcount = 0;
//
//		int graphcount = 0;
//
//		int vulnCount = 0;
//		Iterator<DepGraph> minIter = minDepGraphs.iterator();
//		for (DepGraph origDepGraph : vulnDepGraphs) {
//
//			graphcount++;
//			long startForward = System.currentTimeMillis();
//			f_unmodeled = new HashSet<DepGraphOpNode>();
//			// used to remove the auto from deco after processing all its parents
//			numOfProcessedNodes = 0;
//
//			perfInfo.addGraphInfo(origDepGraph.getRoot().dotName()
//					, origDepGraph.getNumOfNodes(), origDepGraph.getNumOfEdges());
//
//			StrangerAutomaton::debugToFile("// *******************************************************************************" << endl;
//			StrangerAutomaton::debugToFile("// starting a new SINK analysis: " + origDepGraph.getRoot().dotName());
//			StrangerAutomaton::debugToFile("// *******************************************************************************" << endl;
//
//			cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
//			cout << "Starting analysis for SINK: " + origDepGraph.getRoot().dotName());
//			cout << "-----------------------------------------------------------------------------------------------------------------\n" << endl;
//
//
//
//			DepGraph minGraph = minIter.next();
//			DepGraph acyclicWorkGraph = new DepGraph(origDepGraph);
//
//			// graphcount is needed to provide uniqueness to names of dot graphs
//			String fileName = acyclicWorkGraph.getRoot().dotFileName();//MyOptions.entryFile.getName();
//			String graphNameBase = name + "stranger_" + graphcount + "_" + fileName;
//
//			// get a mapping between each SCC nodes and nodes in its cycle
//			Map<DepGraphSccNode, List<DepGraphNode>> sccNodes = acyclicWorkGraph
//					.sort(origDepGraph);
//
//			// in any case, dump the vulnerable depgraphs
//			origDepGraph.dumpDot(graphNameBase + "_depgraph", MyOptions.graphPath,
//					origDepGraph.getUninitNodes(), this.dci);
////			acyclicWorkGraph.dumpDot(graphNameBase + "_workgraph",
////					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
////			acyclicWorkGraph.dumpSortedDot(graphNameBase + "_sorted_workgraph",
////					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
////			minGraph.dumpDot(graphNameBase + "_min_graph_",
////					MyOptions.graphPath, origDepGraph.getUninitNodes(), this.dci);
//
//			CfgNode cfgNode = origDepGraph.getRoot().getCfgNode();
//
//
//			Map<DepGraphNode, StrangerAutomaton> forwardDeco = new HashMap<DepGraphNode, StrangerAutomaton>();
//
//			// Start decorating nodes. Note that we decorate nodes in
//			// topological order
//			for (DepGraphNode node : acyclicWorkGraph.getSortedNodes()) {
//				// if an SCC node then do the fix point computation
//				if (node instanceof DepGraphSccNode) {
//					DepGraphSccNode sccNode = (DepGraphSccNode) node;
//					doFixPointComputationForSCC(acyclicWorkGraph, origDepGraph, forwardDeco,
//							sccNode, sccNodes);
//				} else {// a regular node in the graph
//					doNodeComputation(acyclicWorkGraph, origDepGraph, forwardDeco, node);
//					//debugAuto(forwardDeco.get(node));
//					//perfInfo.printInfo();
//				}
//				//debugMemoryUsage();
//				//debug("--------------------------", 2);
//			}
//
//			long stopForward = System.currentTimeMillis();
//			perfInfo.forwardTime += (stopForward - startForward);
//
//			long startIntersection = System.currentTimeMillis();
//			// intersect root automaton with the undesired stuff;
//			// if the intersection is empty, it means that we are safe!
//			StrangerAutomaton rootAuto = forwardDeco.get(acyclicWorkGraph.getRoot());
//			if (rootAuto.checkIntersection(this.undesir, rootAuto.getID(),
//					this.undesir.getID())) {
//
//				cout << "\n!!!    A vulnerability has been found in SINK:     !!!" << endl;
//				System.out.println(acyclicWorkGraph.getRoot().toString()+"\n" << endl;
//
//				StrangerAutomaton intersectionAuto = rootAuto.intersect(this.undesir, -1);
//				debug("\nIntersection result: \n-------------------------", 2);
//				System.out.print("vitals for attack pattern" << endl;
//				this.undesir.printAutomatonVitals();
//				cout << endl;
//				System.out.print("vitals for intersection" << endl;
//				intersectionAuto.printAutomatonVitals();
//				//debugAuto(intersectionAuto, 1, 4);
//				//intersectionAuto.printAutomatonVitals();
//				debug("", 2);
//				perfInfo.printInfo();
//
//				long stopIntersection = System.currentTimeMillis();
//				perfInfo.forwardTime += (stopIntersection - startIntersection);
//				//outputDotAuto(rootAuto);
//
//
//				 vulnCount++;
//
//				 doBackwardAnalysis(graphcount, origDepGraph, acyclicWorkGraph,
//						sccNodes, forwardDeco, intersectionAuto);
//
//			} else {
//				// no vulnerabilities
//				cout << "\n(: No vulnerabilities. SINK is secure :)\n" << endl;
//			}
//
//
//		}
//		// }
//
//		// initial sink count and final graph count may differ (e.g., if some
//		// sinks
//		// are not reachable)
//		cout << "----------------------------" << endl;
//		if (MyOptions.optionV) {
//			cout << "Total Graph Count: " + graphcount);
//		}
//		cout << "Total Vuln Count: " + vulnCount);
//		cout << "----------------------------" << endl;
//
//
//		cout << endl;
//		cout << "*****************" << endl;
//		System.out.println(name.toUpperCase()
//				+" Stranger Sanit Analysis END" << endl;
//		cout << "*****************" << endl;
//		cout << endl;
//
//		long stop = System.currentTimeMillis();
//		perfInfo.totalRunningTime = stop - start;
//		perfInfo.printInfo();
//		// only to output c trace file. One file per all sink places(depgraphs)
//		StrangerAutomaton::closeFiles();
//	*/
//		return new LinkedList<Integer>();
//	}
//
//// **************************************************************************************
//
//	private void outputDotAuto(StrangerAutomaton auto) {
//		if (true)
//			auto.toDot();
//
//	}
//// **************************************************************************************
//
//	protected List<DepGraphNode> getHTTPInputNodes(DepGraph origDepGraph,
//			DepGraph acyclicWorkGraph) {
//		List <DepGraphNode> inputNodes = new ArrayList<DepGraphNode>();
//		Set<DepGraphUninitNode> uninitNodes = acyclicWorkGraph.getUninitNodes();
//		for (DepGraphUninitNode uninitNode: uninitNodes){
//		 	// retrieve predecessor
//			Set<DepGraphNode> preds = acyclicWorkGraph.getPredecessors(uninitNode);
//			if (preds.size() != 1) {
//				// if more than one do not belong to an SCC
//				throw new StrangerStringAnalysisException("SNH");
//			}
//			// we should only have one predecessor for uninit node
//			DepGraphNode pre = preds.iterator().next();
//
//			if (pre instanceof DepGraphNormalNode) {
//				DepGraphNormalNode preNormal = (DepGraphNormalNode) pre;
//				switch (this.initiallyTainted(preNormal.getPlace())) {
//				case ALWAYS:// always tainted
//				case IFRG:// tainted if registered globals is enabled (optionG
//							// == true)
//					inputNodes.add(uninitNode);
//					break;
//				case NEVER:// never tainted (always untainted and harmless)
//					break;
//				default:
//					throw new StrangerStringAnalysisException("SNH");
//				}
//				// if predecessor is an SCC
//			} else if (pre instanceof DepGraphSccNode) {
//				// this case can really happen (e.g.: dcpportal:
//				// advertiser.php, forums.php);
//
//				// take a look at the "real" predecessors (i.e.,
//				// take a look "into"
//				// the SCC node): if there is exactly one
//				// predecessor, namely a
//				// DepGraphNormalNode, and if the contained
//				// place is initially untainted,
//				// there is no danger from here; else: we will
//				// have to set it to tainted
//				Set<DepGraphNode> origPreds = origDepGraph
//						.getPredecessors(uninitNode);
//				if (origPreds.size() == 1) {
//					DepGraphNode origPre = origPreds.iterator().next();
//					if (origPre instanceof DepGraphNormalNode) {
//						DepGraphNormalNode origPreNormal = (DepGraphNormalNode) origPre;
//
//						switch (this.initiallyTainted(origPreNormal.getPlace())) {
//						case ALWAYS:// always tainted
//						case IFRG:// tainted if registered globals is enabled
//									// (optionG == true)
//							inputNodes.add(uninitNode);
//							break;
//						case NEVER:// always untainted
//							break;
//						default:
//							throw new StrangerStringAnalysisException("SNH");
//						}
//
//					} else {// predecessor is DepGraphOpNode
//						throw new StrangerStringAnalysisException("Not Handled" << endl;
//					}
//				} else { // multiple predecessors for uninit node all in an SCC
//					throw new StrangerStringAnalysisException("Not Handled" << endl;
//				}
//			}
//		 }
//		// this is only important when doing backward analysis
//		//inputNodes.addAll(f_unmodeled);
//		return inputNodes;
//	}
//
//// **************************************************************************************
//
void ForwardImageComputer::doNodeComputation(
                    DepGraph& acyclicWorkGraph, DepGraph& origDepGraph,
                    AnalysisResult& analysisResult, DepGraphNode* node,
                    AnalysisResult& inputValuesMap, bool multiTrack)
{
                            
	// always use nodes from original dep graph as current dep graph
	// does not have the nodes inside scc so successors would always
	// be null for these nodes.
	debug(stringbuilder() << "doNodeComputaion for node: "  << node->getID(), 2);

	numOfProcessedNodes++;

	if (node->getNonprocessedParents() == -1)
		node->setNonprocessedParents((int)origDepGraph.getPredecessors(node).size());

	NodesList successors = origDepGraph.getSuccessors(node);
	StrangerAutomaton* newAuto = NULL;
    DepGraphNormalNode* normalnode;
    DepGraphOpNode* opNode;
    DepGraphUninitNode* uninitNode;
    StrangerAutomaton* temp;
		// Note that node can not be SCC as we deal with scc in fixed point
		// computaion
    if ((normalnode = dynamic_cast<DepGraphNormalNode*>(node)) != NULL) {
			if (successors.empty()) {
				// this should be a string leaf node
				TacPlace* place = normalnode->getPlace();
				if (dynamic_cast<Literal*>(place) != NULL) {
					newAuto = StrangerAutomaton::makeString(place->toString(),
							node->getID());
//					if (multiTrack)
//						newAuto = StrangerMTrackAutomaton.makeConstMTAuto(newAuto, node->getID());
				} else {
					// this case should not happen any longer
					// (now that
					// we have "uninit" nodes, see below)
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: " << place->toString() << ", "
							<< normalnode->getFileName() << ","
							<< normalnode->getOrigLineNo());
				}
			} else {
				// this is an interior node, not a leaf node;
				// the automaton for this node is the union of
				// all the
				// successor automatas
                NodesListIterator succIt;
                 DepGraphNode* succ;
				for (succIt = successors.begin(); succIt != successors.end(); ++succIt) {
                    succ = *succIt;
					if (succ->getID() == node->getID()) {
						// TODO: check if this is true
						// a simple loop, can be ignored
						continue;
					}
                    AnalysisResultIterator it = analysisResult.find(succ->getID());
					StrangerAutomaton* succAuto;
					if ( it == analysisResult.end()) {
						throw StrangerStringAnalysisException("SNH");
					}
                    succAuto = it->second;
					if (newAuto == NULL) {
						// clone necessary to be safe here
						// if we want to optimize then we must check if succ
						// node has only
						// one predecessor. If so then do not clone just copy
						// reference
						// otherwise clone.
						// newAuto = succAuto;
						// debug(node.getID() + " = " + succAuto.getID());
						// newAuto.setID(node.getID()); // for debugging only
						newAuto = succAuto->clone(node->getID());
					} else {
                        temp = newAuto;
						newAuto = newAuto->union_(succAuto, node->getID());
                        delete temp;
					}
				}
			}

		} else if ((opNode = dynamic_cast<DepGraphOpNode*>(node)) != NULL) {
			newAuto = makeAutoForOp(opNode, analysisResult,
					origDepGraph, multiTrack);

		} else if ((uninitNode = dynamic_cast<DepGraphUninitNode*>(node)) != NULL) {
			// for input nodes we take the value from the input init values map.
			// the first time the input values map will have Sigma*.
			// after that it will contain the resutl of previous bw analysis
			// starting with epsilon
			if (inputValuesMap.find(node->getID()) != inputValuesMap.end()){
				newAuto = inputValuesMap[node->getID()];
			} else {
				// retrieve predecessor
				NodesList preds = acyclicWorkGraph.getPredecessors(node);
				if (preds.size() != 1) {
					// if more than one do not belong to an SCC
					throw StrangerStringAnalysisException("SNH");
				}
				// we should only have one predecessor for uninit node
				 DepGraphNode* pre = *(preds.begin());
                 DepGraphNormalNode* preNormal;
				if ((preNormal = dynamic_cast< DepGraphNormalNode*>(pre)) != NULL) {
					newAuto = StrangerAutomaton::makeAnyString(node->getID());
					// if predecessor is an SCC
				}
//                    else if (pre instanceof DepGraphSccNode) {
//					// this case can really happen (e.g.: dcpportal:
//					// advertiser.php, forums.php);
//
//					// take a look at the "real" predecessors (i.e.,
//					// take a look "into"
//					// the SCC node): if there is exactly one
//					// predecessor, namely a
//					// DepGraphNormalNode, and if the contained
//					// place is initially untainted,
//					// there is no danger from here; else: we will
//					// have to set it to tainted
//					Set<DepGraphNode> origPreds = origDepGraph
//							.getPredecessors(node);
//					if (origPreds.size() == 1) {
//						DepGraphNode origPre = origPreds.iterator().next();
//						if (origPre instanceof DepGraphNormalNode) {
//							DepGraphNormalNode origPreNormal = (DepGraphNormalNode) origPre;
//
//							switch (this.initiallyTainted(origPreNormal.getPlace())) {
//							case ALWAYS:// always tainted
//							case IFRG:// tainted if registered globals is enabled
//										// (optionG == true)
//								newAuto = StrangerAutomaton::makeAnyString(node
//										.getID());
//								break;
//							case NEVER:// always untainted
//								// TODO: is it under approximation here or OK
//								newAuto = StrangerAutomaton::makeEmptyString(node
//										.getID());
//								break;
//							default:
//								throw new StrangerStringAnalysisException("SNH");
//							}
//
//						} else {// predecessor is DepGraphOpNode
//							// TODO: is it under approximation here or OK
//							newAuto = StrangerAutomaton::makeEmptyString(node
//									.getID());
//						}
//					} else { // multiple predecessors for uninit node all in an SCC
//						// TODO: we may have under approximation here.
//						newAuto = StrangerAutomaton::makeEmptyString(node->getID());
//					}
//
//				} else { // predecessor of uninit is not SCC Node or NormalNode
//					throw new StrangerStringAnalysisException("SNH: " + pre.getClass());
//				}
			}

		} else { // node is not NormalNode or UninitNode or OpNode
			throw StrangerStringAnalysisException("SNH");
		}

		if (newAuto == NULL) {
			throw StrangerStringAnalysisException("SNH");
		}

		analysisResult[node->getID()] =  newAuto;

		// if not scc node then
//		if (node.getSccID() == -1)
//			// set me as processed for all children and remove auto of a child
//			// if all parents are processed. auto is not needed anymore
//			for (DepGraphNode successor: successors){
//				successor.setNonprocessedParents(successor.getNonprocessedParents() -1);
//				if (successor.getNonprocessedParents() == 0)
//					deco.remove(successor);
//			}

		debug("--------------------------",2 );
}

//	// ********************************************************************************
//
//	private void doFixPointComputationForSCC(DepGraph acyclicWorkGraph,
//			DepGraph origDepGraph, Map<DepGraphNode, StrangerAutomaton> forwardDeco,
//			DepGraphSccNode sccNode,
//			Map<DepGraphSccNode, List<DepGraphNode>> sccNodes, boolean multiTrack) {
//		debug("========================================================================", 2);
//
//		int pw = MyOptions.precise_widening;
//		int cw = MyOptions.coarse_widening;
//
//		debug("doFixPointComputaion for scc: " + sccNode.getSccID()
//				+ " -- start", 2);
//		debug("widening values: precise=" + pw + ",  coarse=" + cw, 2);
//		debug("========================================================================", 2);
//
//		StrangerAutomaton::debugToFile("//doFixPointComputaion for scc: " + sccNode.getSccID() + " -- start" << endl;
//
//		List<DepGraphNode> currentSccNodes = sccNodes.get(sccNode);
//		int currentSccId = sccNode.getSccID();
//
//		// for each node we keep number of times it was visited by the algorithms
//		Map<DepGraphNode, Integer> numOfVisits = new HashMap<DepGraphNode, Integer>();
//
//		// first thing is to have all of the nodes initialized to phi
//		for (DepGraphNode node : currentSccNodes) {
//			forwardDeco.put(node, StrangerAutomaton::makePhi(node.getID()));
//			numOfVisits.put(node, 0);
//		}
//
//		// a work list to do fixpoint computation
//		LinkedList<DepGraphNode> workList = new LinkedList<DepGraphNode>();
//		// first items in the work list are the preds of the scc
//		List<DepGraphNode> sccSuccs = acyclicWorkGraph.getSuccessors(sccNode);
//		for (DepGraphNode sccSucc: sccSuccs){
//			workList.addLast(sccSucc);
//		}
//
//		int iteration = 0;
//		do {
//			DepGraphNode currentNode = workList.removeFirst();
//			debug("*******  iteration "+ (iteration) +"*****************", 2);
//			// current value has changed sol
//			// we calculate the value for successors
//			for (DepGraphNode pred: origDepGraph.getPredecessors(currentNode)){
//				// if succ node is not part of the scc then do not
//				// compute any thing
//				if (pred.getSccID() != currentSccId)
//					continue;
//
//				// save the old auto
//				StrangerAutomaton oldAuto = forwardDeco.get(pred);//.clone(pred.getID());
//
//				// do a regular computation
//				StrangerAutomaton tmpAuto = null;
//
//				if (pred instanceof DepGraphNormalNode){
//					tmpAuto = forwardDeco.get(currentNode);//.clone(pred.getID());
//				}
//				else if (pred instanceof DepGraphOpNode){
//					List<DepGraphNode> successors = origDepGraph.getSuccessors(pred);
//					StrangerAutomaton subjectAuto = forwardDeco.get(successors.get(2));
//					StrangerAutomaton searchAuto = forwardDeco.get(successors[0]->getID()];
//					String replaceStr = forwardDeco.get(successors.get(1)).getStr();
//					if (subjectAuto.isEmpty() || searchAuto.isEmpty() || replaceStr == null)
//						continue;
//					tmpAuto = makeAutoForOp((DepGraphOpNode)pred, forwardDeco, origDepGraph, multiTrack);
//				}
//				else // node is uninit or scc which can not be part of an scc
//					throw new StrangerStringAnalysisException("SNH");
//				if (tmpAuto == null)
//					throw new StrangerStringAnalysisException("SNH");
//
//				// do widening of old and new value union old then compare
//				// with old value.
//				// if different then set changed to true
//				StrangerAutomaton newAuto = tmpAuto.union(oldAuto, pred.getID());
//				int visited = numOfVisits.get(pred).intValue() + 1;
//				if (visited > iteration)
//					iteration = visited;
//				// if reached number of iterations to apply widening
//				if (visited > pw) {
//
//					// if reached number of iterations to apply coarse
//					// widening
//					if (visited > cw)
//						newAuto = oldAuto.coarseWiden(newAuto, pred
//								.getID());
//					else
//						newAuto = oldAuto.preciseWiden(newAuto, pred
//								.getID());
//				}
//				// As we guarantee old is subset of new we just check if
//				// widened is subset of old
//				if (!newAuto.checkInclusion(oldAuto, newAuto.getID(),
//						oldAuto.getID())) {
//					if(!workList.contains(pred))
//						workList.addLast(pred);
//					forwardDeco.put(pred, newAuto);
//					numOfVisits.put(pred, visited);
//				}
//
//				debug("-------------------------", 2);
//			}
//		} while ((!workList.isEmpty()) && ((iteration) <= 30000));
//
//		StrangerAutomaton::debugToFile("//doFixPointComputaion for scc: " + sccNode.getSccID() + " -- end" << endl;
//
//		debug("========================================================================", 2);
//		debug("doFixPointComputaion for scc: " + sccNode.getSccID() + " -- end", 2);
//		debug("========================================================================", 2);
//	}
//
//	// ********************************************************************************

	// returns an automaton for the given operation node
StrangerAutomaton* ForwardImageComputer::makeAutoForOp(DepGraphOpNode* opNode, AnalysisResult& analysisResult, DepGraph& depGraph, bool multiTrack){

		 NodesList successors = depGraph.getSuccessors(opNode);
		StrangerAutomaton* retMe = NULL;

        string opName = opNode->getName();

		vector<int> multiList;

		if (!opNode->isBuiltin()) {
            if (opName == "__vlab_restrict\\n(unmodeled)") {
                if (successors.size() < 3) {
                    throw StrangerStringAnalysisException("SNH\n");
                }
                
                StrangerAutomaton* subjectAuto = analysisResult[successors[1]->getID()];
                StrangerAutomaton* patternAuto = StrangerAutomaton::regExToAuto(analysisResult[successors[0]->getID()]->getStr(), true, -1);
                string complement = analysisResult[successors[2]->getID()]->getStr();
                StrangerAutomaton* restricted = NULL;
                if (complement == "true") {
                    StrangerAutomaton* patternComplement = patternAuto->complement(-1);
                    delete patternAuto;
                    restricted = subjectAuto->restrict(patternComplement, opNode->getID());
                    delete patternComplement;
                }
                else{
                    restricted = subjectAuto->restrict(patternAuto, opNode->getID());
                    delete patternAuto;
                }
                return restricted;
            }
            else {
                //call to function or method for which no definition
                //could be found
                throw StrangerStringAnalysisException("SNH: Call to a non builtin function.\n");
            }

		} else if (opName == ".") {
			 //CONCAT
                for (NodesListIterator it = successors.begin(); it != successors.end(); ++it) {
                     DepGraphNode* succ = *it;
                    StrangerAutomaton* succAuto = analysisResult[succ->getID()];
                    if (retMe == NULL) {
                        //clone necessary to be safe here
                        //if we want to optimize then we must check if succ node
                        //has only
                        //one predecessor. If so then do not clone just copy
                        //reference
                        //otherwise clone.
                        retMe = succAuto->clone(opNode->getID());
                    } else {
                        retMe = retMe->concatenate(succAuto, opNode->getID());
                    }
                }
		} else if (opName == "preg_replace") {
			 //TODO: this code needs to be revised after implementing
			 //reg_replace
			 //Note: subjectAuto should be a string to use the replace function
			 //from StrangerLibrary. Otherwise we need a method to return the
			 //set
			 //of strings that an automaton represents (in PHP replace should
			 //always
			 //have only one string.
			if (successors.size() < 3) {
				throw new StrangerStringAnalysisException("SNH\n");
			}

			StrangerAutomaton* subjectAuto = analysisResult[successors[2]->getID()];
			StrangerAutomaton* patternAuto = StrangerAutomaton::regExToAuto(analysisResult[successors[0]->getID()]->getStr(), true, opNode->getID());
			string replaceStr = analysisResult[successors[1]->getID()]->getStr();

			StrangerAutomaton* replacement = StrangerAutomaton::reg_replace(
					patternAuto, replaceStr, subjectAuto, true, opNode->getID());
            delete patternAuto;
			return replacement;

		} else if (opName == "ereg_replace") {
			 //Note: subjectAuto should be a string to use the replace function
			 //from StrangerLibrary. Otherwise we need a method to return the
			 //set of strings that an automaton represents (in PHP replace
			 //should always have only one string.

			throw new StrangerStringAnalysisException("ereg_replace is not supported yet by stranger.\n");
            
		} else if (opName == "str_replace") {
			 //Note: subjectAuto should be a string to use the replace function
			 //from StrangerLibrary. Otherwise we need a method to return the
			 //set of strings that an automaton represents (in PHP replace
			 //should always have only one string.

			if (successors.size() < 3) {
				throw new StrangerStringAnalysisException("SNH");
			}
			StrangerAutomaton* subjectAuto = analysisResult[successors[2]->getID()];
			StrangerAutomaton* searchAuto = analysisResult[successors[0]->getID()];
			string replaceStr = analysisResult[successors[1]->getID()]->getStr();

			StrangerAutomaton* replacement = StrangerAutomaton::str_replace(
					searchAuto, replaceStr, subjectAuto, opNode->getID());
			return replacement;

		} else if (opName == "addslashes") {

			if (successors.size() != 1) {
				throw new StrangerStringAnalysisException("SNH");
			}
			StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

			StrangerAutomaton* slashesAuto = StrangerAutomaton::addslashes(
					paramAuto, opNode->getID());
			return slashesAuto;

		} else if (opName == "stripslashes") {
			 //this is not a precise model but rather an overapproximation
			if (successors.size() != 1) {
				throw new StrangerStringAnalysisException("SNH");
			}
			 //only has one parameter
			StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

			StrangerAutomaton* slashesAuto = StrangerAutomaton::stripslashes(
					paramAuto, opNode->getID());
			return slashesAuto;

		} else if (opName == "mysql_real_escape_string") {

			if (successors.size() < 1 || successors.size() > 2) {
				throw new StrangerStringAnalysisException("SNH");
			}
            //we only care about the first parameter
			StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];

			StrangerAutomaton* mysqlEscapeAuto = StrangerAutomaton::mysql_real_escape_string(
					paramAuto, opNode->getID());
			return mysqlEscapeAuto;

		} else if (opName == "nl2br"){

			if (successors.size() < 1 || successors.size() > 2) {
				throw new StrangerStringAnalysisException("SNH");
			}
			StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
			StrangerAutomaton* nl2brAuto = StrangerAutomaton::nl2br(
					paramAuto, opNode->getID());
			return nl2brAuto;



		} else if (opName == "trim" || opName == "strtoupper" || opName == "strtolower") {
            if (successors.size() != 1) {
				throw new StrangerStringAnalysisException(stringbuilder() << "SNH: " << opName << " has more than one successor in depgraph.\n" );
			}
			 //TODO: quickly check if this correct
			 //each is treated as a function and it does not do anything
			StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
            
			StrangerAutomaton* resultAuto = NULL;
            if (opName == "trim")
                resultAuto = paramAuto->trimSpaces(opNode->getID());
            else if (opName == "strtoupper") {
                resultAuto = paramAuto->toUpperCase(opNode->getID());
            }
            else if (opName == "strtolower") {
                resultAuto = paramAuto->toLowerCase(opNode->getID());
            }
			return resultAuto;

		} else {
			cout << "Warning: Unmodeled builtin general function : "
					<< opName;

			f_unmodeled.push_back(opNode);

			 //conservative decision for operations that have not been
			 //modeled yet: .*
			retMe = StrangerAutomaton::makeAnyString(opNode->getID());
		}

		return retMe;
	}


//	// ********************************************************************************
//
//
//	// - if all successors are empty: returns empty
//	// - else: returns .*
//	private StrangerAutomaton multiDependencyAutoSanit(
//			DepGraphOpNode node,
//			List<DepGraphNode> succs,
//			Map<DepGraphNode, StrangerAutomaton> deco, List<Integer> indices,
//			boolean inverse) {
//
//		Set<Integer> indexSet = new HashSet<Integer>(indices);
//		int count = -1;
//		for (DepGraphNode succ : succs) {
//			count++;
//			// check if there is a dependency on this successor
//			if (inverse) {
//				if (indexSet.contains(count)) {
//					continue;
//				}
//			} else {
//				if (!indexSet.contains(count)) {
//					continue;
//				}
//			}
//
//			StrangerAutomaton succAuto = deco.get(succ);
//			if (succAuto == null) {
//				throw new StrangerStringAnalysisException("SNH");
//			}
//			if (!succAuto.checkEmptyString()) {
//				cout << "  --> unsafe function" << endl;
//				return StrangerAutomaton::makeAnyString(-1);
//			}
//		}
//		cout << "  --> safe function" << endl;
//		return StrangerAutomaton::makeEmptyString(node.getID());
//	}
//
//	// ********************************************************************************
//
//	// checks if the given node is a custom sanitization node
//	public static boolean isCustomSanit(DepGraphNode node) {
//
//		if (node instanceof DepGraphNormalNode) {
//			return false;
//		} else if (node instanceof DepGraphUninitNode) {
//			return false;
//		} else if (node instanceof DepGraphOpNode) {
//			// check if this operation could be used for custom sanitization
//			DepGraphOpNode opNode = (DepGraphOpNode) node;
//			if (opNode.isBuiltin()) {
//				CfgNode cfgNode = opNode.getCfgNode();
//				if (cfgNode instanceof CfgNodeCallBuiltin) {
//					CfgNodeCallBuiltin callBuiltin = (CfgNodeCallBuiltin) cfgNode;
//					String funcName = callBuiltin.getFunctionName();
//
//					// here is the list of custom sanitization functions
//					if (funcName.equals("ereg_replace")
//							|| funcName.equals("preg_replace")
//							|| funcName.equals("str_replace")) {
//
//						// found it!
//						return true;
//
//					} else {
//						return false;
//					}
//
//				} else {
//					return false;
//				}
//			} else {
//				return false;
//			}
//		} else if (node instanceof DepGraphSccNode) {
//			throw new StrangerStringAnalysisException("SNH");
//		} else {
//			throw new StrangerStringAnalysisException("SNH");
//		}
//	}
//
//	// ********************************************************************************
//
//	public boolean isIneffective(DepGraphNode customSanit,
//			Map<DepGraphNode, StrangerAutomaton> deco) {
//
//		StrangerAutomaton auto = deco.get(customSanit);
//		if (auto == null) {
//			// no decoration for this node: be conservative
//			return true;
//		}
//
//		// intersect!
//		StrangerAutomaton intersection = auto.intersect(this.undesir, -1);
//		// if (!intersection.isEmpty()) {
//		// return true;
//		// } else {
//		// return false;
//		// }
//		// added by Muath to avoid syntac problems
//		return false;
//
//	}
//
//	// ********************************************************************************
//
//	// locates custom sanitization nodes in the given depgraph and returns them
//	public static List<DepGraphNode> findCustomSanit(DepGraph depGraph) {
//		List<DepGraphNode> retMe = new LinkedList<DepGraphNode>();
//		for (DepGraphNode node : depGraph.getNodes()) {
//			if (isCustomSanit(node)) {
//				retMe.add(node);
//			}
//		}
//		return retMe;
//	}
//
//	// ********************************************************************************





AnalysisResult ForwardImageComputer::doBackwardAnalysis(DepGraph& origDepGraph,
                                                        DepGraph& inputDepGraph,
                                                        /*SccNodes& sccNodes,*/
                                                         NodesList& sortedNodes,
                                                        StrangerAutomaton* intersectionAuto,
                                                        const AnalysisResult& fwAnalysisResult) {
		// backward analysis is done inreverse order starting from root (sink) to input node.
		// As inputDepGraph is already sorted to accomodate this fact we have no problem here.
    AnalysisResult bwAnalysisResult;
		// inittialize the backward analysis lattice to phi (bottom value)
//		for (DepGraphNode node: origDepGraph.getNodes()){
//			backwardDeco.put(node, StrangerAutomaton::makePhi(node.getID()));
//		}
	bwAnalysisResult[inputDepGraph.getRoot()->getID()] = intersectionAuto;
	// skip root as we have already
	// decorated the root with the intersection automaton
	NodesListConstIterator it = ++sortedNodes.begin();
	while (it != sortedNodes.end()) {
		 DepGraphNode* node = (DepGraphNode*) *it;
		// if an SCC node then do the fix point computation
//			if (node instanceof DepGraphSccNode) {
//				DepGraphSccNode sccNode = (DepGraphSccNode) node;
//				 doBackwardFixPointComputationForSCC(inputDepGraph,
//				 origDepGraph, backwardDeco, forwardDeco, sccNode, sccNodes);
//			} else {// a regular node in the graph
			doBackwardNodeComputation(inputDepGraph, origDepGraph, bwAnalysisResult, fwAnalysisResult, node, false);

			// the following check for emptiness is only an assertion for debugging purposes
			//if (backwardDeco.get(node).checkEmptiness())
			//	throw new StrangerStringAnalysisException("SNH: and empty automaton in backward analysis." << endl;
			//debugAuto(backwardDeco.get(node));
			//perfInfo.printInfo();
//			}
		 ++it;
		//debugMemoryUsage();
		//debug("--------------------------", 2);
	}
	return bwAnalysisResult;
}

//	// ********************************************************************************
void ForwardImageComputer::doBackwardNodeComputation(DepGraph& inputDepGraph, DepGraph& origDepGraph,
                               AnalysisResult& bwAnalysisResult,
                               const AnalysisResult& fwAnalysisResult,
                                DepGraphNode* node,
                               bool fixPoint){

		// always use nodes from original dep graph as current dep graph
		// does not have the nodes inside scc so predecessors would always
		// be null for these nodes.
		debug(stringbuilder() << "doNodeComputaion for node: " << node->getID(), 2);
		NodesList predecessos = origDepGraph.getPredecessors(node);
		NodesList successors = origDepGraph.getSuccessors(node);
		StrangerAutomaton *newAuto = NULL, *tempAuto = NULL;
         DepGraphNormalNode* normalNode = NULL;
		// Note that node can not be SCC as we deal with scc in fixed point
		// computaion
		if (dynamic_cast< DepGraphNormalNode*>(node)
				|| dynamic_cast< DepGraphUninitNode*>(node)
				|| dynamic_cast< DepGraphOpNode*>(node)) {
			if (predecessos.empty()) {
				// this should be root which we already processed at the
				// beginning node. This only happens if root is in an scc
				// and here we do not deal with sccs
				if ((normalNode = dynamic_cast< DepGraphNormalNode*>(node))) {
					TacPlace* place = normalNode->getPlace();
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: node " << node->getID()
							<< " does not have predecessors. " << place << ", "
							<< normalNode->getFileName() << ","
							<< normalNode->getOrigLineNo());
				} else
					throw StrangerStringAnalysisException(stringbuilder() << "SNH: node " << node->getID()
							<< " does not have predecessors.\n");
			// if node is literal then assign value regardless of backward analysis
			} else if (successors.size() == 0 && (normalNode = dynamic_cast< DepGraphNormalNode*>(node))){
				TacPlace* place = normalNode->getPlace();
				if (dynamic_cast<Literal*>(place)) {
                    newAuto = StrangerAutomaton::makeString(place->toString(), node->getID());
				} else {
                    // this case should not happen any longer (now that
                    // we have "uninit" nodes, see below)
                    throw StrangerStringAnalysisException(stringbuilder() << "SNH: " << place << ", " << normalNode->getFileName() << "," <<
                            normalNode->getOrigLineNo());
				}
			} else {
				// the automaton for this node is the union of
				// all the predecessor automatas. Then intersect
				// with forward analysis value
				StrangerAutomaton* forwardAuto = fwAnalysisResult.find(node->getID())->second;
				for (NodesListIterator it = predecessos.begin(); it != predecessos.end(); ++it) {
                     DepGraphNode* pred = *it;
					StrangerAutomaton* predAuto = NULL;
					if (pred == node) {
						// TODO: check if this is true
						// a simple loop, can be ignored
						continue;
					} else if (dynamic_cast< DepGraphNormalNode*>(pred)) {
						predAuto = bwAnalysisResult.find(pred->getID())->second;
//						// if doing fix point computation then first time backward is null
//						if (fixPoint && pred.getSccID() == node.getSccID() && predAuto.isEmpty())
//							predAuto = forwardDeco.get(pred);

					} else if (dynamic_cast< DepGraphOpNode*>(pred)) {
						predAuto = this->makeBackwardAutoForOpChild(
								dynamic_cast< DepGraphOpNode*>(pred), node, bwAnalysisResult, fwAnalysisResult,
								origDepGraph, fixPoint);
					}

					if (predAuto == NULL)
						throw StrangerStringAnalysisException("SNH");

//					if(check_emptiness(predAuto)) continue;

					if (newAuto == NULL) {
						// newAuto = succAuto; // cloning not necessary here
						// debug(node.getID() + " = " + succAuto.getID());
						// newAuto.setID(node.getID()); // for debugging only
						newAuto = predAuto->clone(node->getID()); // this is
																// better for
																// debugging
					} else {
                        tempAuto = newAuto;
						newAuto = newAuto->union_(predAuto, node->getID());
                        delete tempAuto;
					}
				}
				// intersect the value from predecessors with the value from forward analysis
                tempAuto = newAuto;
				newAuto = newAuto->intersect(forwardAuto, node->getID());
                delete tempAuto;


			}
		} else { // node is not NormalNode or UninitNode or OpNode
			throw new StrangerStringAnalysisException("SNH");
		}

		if (newAuto == NULL) {
			throw new StrangerStringAnalysisException("SNH");
		}

		bwAnalysisResult[node->getID()] = newAuto;
		debug("--------------------------", 2);

	}



//	// ********************************************************************************
StrangerAutomaton* ForwardImageComputer::makeBackwardAutoForOpChild( DepGraphOpNode* opNode,
			 DepGraphNode* childNode,
			AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult, DepGraph& depGraph, boolean fixPoint) {

		StrangerAutomaton* retMe = NULL;

		string opName = opNode->getName();

		NodesList successors = depGraph.getSuccessors(opNode);

		StrangerAutomaton* opAuto = bwAnalysisResult[opNode->getID()];
		// if doing fix point computation then first time backward is NULL
//		if (fixPoint && opNode->getSccID() == childNode.getSccID() && opAuto.isEmpty())
//			opAuto = forwardDeco.get(opNode);
//		else if (opAuto.isEmpty())
//			throw new StrangerStringAnalysisException("SNH: opAuto is empty." << endl;


		if (!opNode->isBuiltin()) {

            if (opName == "restrict\\n(unmodeled)") {
                if (successors.size() < 3) {
                    throw StrangerStringAnalysisException(stringbuilder() << "SNH");
                }
                
                DepGraphNode* subjectNode = successors[1];
                DepGraphNode* patternNode = successors[0];
                DepGraphNode* complementNode = successors[2];
                
                if (childNode->equals(subjectNode)){
                    retMe = opAuto;
                    
                } else if (childNode->equals(patternNode)){
                    //TODO: backward for match is the same as forward
                    // prereplace value for pattern node (match node) can not be computed currently
                    throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of replace (" << opNode->getID() << ") represents match pattern and we can not compute prereplace for it.\n");
                    
                } else if (!childNode->equals(complementNode)){
                    throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of replace (" << opNode->getID() << ") is its successor.\n");
                }
                else
                    // note that both forward and backward analysisi do not handle replace where the replace value is
                    // not a literal (constant) but a variable (automaton)
                    throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of replace (" << opNode->getID() << ") has automoatn replace value and we only handle literal replace values.\n");
                return retMe;
            }
            else {

			// call to function or method for which no definition
			// could be found
            throw StrangerStringAnalysisException(stringbuilder() << "SNH: function " << opName << " is not builtin function.\n");
            }
			
		} else if (opName == ".") {
			// CONCAT

			if (successors.size() < 2)
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: less than two successors for concat node " << opNode->getID());

			StrangerAutomaton* concatAuto = opAuto;

			 DepGraphNode* leftSibling = successors[0];
			 DepGraphNode* rightSibling = successors[1];

			if (childNode->equals(leftSibling)){

				if (isLiteral(rightSibling, depGraph.getSuccessors(rightSibling))){
					string value = getLiteralValue(rightSibling);
					retMe = concatAuto->leftPreConcatConst(value, childNode->getID());
				} else {
					StrangerAutomaton* rightSiblingAuto = fwAnalysisResult.find(rightSibling->getID())->second;
					retMe = concatAuto->leftPreConcat(rightSiblingAuto, childNode->getID());
				}

			} else if (childNode->equals(rightSibling)){

				if (isLiteral(leftSibling, depGraph.getSuccessors(leftSibling))){
					string value = getLiteralValue(leftSibling);
					retMe = concatAuto->rightPreConcatConst(value, childNode->getID());
				} else {
					StrangerAutomaton* leftSiblingAuto = fwAnalysisResult.find(leftSibling->getID())->second;
					retMe = concatAuto->rightPreConcat(leftSiblingAuto, childNode->getID());
				}

			}
			else
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: child (" << childNode->getID() << ") of concat (" << opNode->getID() << ") is not equal to any of the two successors.\n");

			//if (retMe.checkEmptiness())
			//	cout << "auto is empty for node: " << childNode->getID() +"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;

			return retMe;

		} else if (opName == "preg_replace" || opName == "str_replace") {
			// Note: subjectAuto should be a string to use the replace function
			// from StrangerLibrary. Otherwise we need a method to return the
			// set
			// of strings that an automaton represents (in PHP replace should
			// always
			// have only one string.
			if (successors.size() < 3) {
				throw StrangerStringAnalysisException(stringbuilder() << "SNH");
			}

			StrangerAutomaton* replaceAuto = opAuto;
			string replaceStr = fwAnalysisResult.find(successors[1]->getID())->second->getStr();

			 DepGraphNode* subjectNode = successors[2];
			 DepGraphNode* patternNode = successors[0];

			if (childNode->equals(subjectNode)){
				StrangerAutomaton* patternAuto =  NULL;
				if (opName == "preg_replace")
					patternAuto = StrangerAutomaton::regExToAuto(fwAnalysisResult.find(successors[0]->getID())->second->getStr(), true, childNode->getID());
				else if (opName == "str_replace")
					patternAuto = fwAnalysisResult.find(successors[0]->getID())->second;
				retMe = replaceAuto->preReplace(patternAuto, replaceStr, childNode->getID());

			} else if (childNode->equals(patternNode)){
				//TODO: backward for match is the same as forward
				// prereplace value for pattern node (match node) can not be computed currently
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of replace (" << opNode->getID() << ") represents match pattern and we can not compute prereplace for it.\n");

			} else if (!childNode->equals(successors[1])){
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of replace (" << opNode->getID() << ") is its successor.\n");
            }
			else
				// note that both forward and backward analysisi do not handle replace where the replace value is
				// not a literal (constant) but a variable (automaton)
				throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of replace (" << opNode->getID() << ") has automoatn replace value and we only handle literal replace values.\n");
			return retMe;

		} else if (opName == "addslashes") {
			// only has one parameter ==>  string addslashes  ( string $str  )
			return StrangerAutomaton::pre_addslashes(opAuto,childNode->getID());

		} else if (opName == "stripslashes") {
			// only has one parameter ==> string stripslashes  ( string $str  )
			return StrangerAutomaton::pre_stripslashes(opAuto,childNode->getID());

		} else if (opName == "mysql_real_escape_string") {
			// has either one or two parameter. If two the second is not affected. ==> string mysql_real_escape_string  ( string $unescaped_string  [, resource $link_identifier  ] )
			if (childNode->equals(successors[0]))
				return StrangerAutomaton::pre_mysql_real_escape_string(opAuto, childNode->getID());
			else // if second parameter ($dbc ==> DB connection ==> $link_identifier) then no need to calculate anything
				return fwAnalysisResult.find(successors[1]->getID())->second;

		} else if (opName == "nl2br") {
			// has either one or two parameter. If two the second is not affected. ==> string nl2br  ( string $string  [, bool $is_xhtml = true  ] )
			if (childNode->equals(successors[0]))
				return StrangerAutomaton::pre_nl2br(opAuto, childNode->getID());
			else // if second parameter (bool $is_xhtml = true) then no need to calculate anything
				return fwAnalysisResult.find(successors[1]->getID())->second;

		} else if (opName == "trim" ) {
			// only has one parameter ==>  string trim  ( string $str  )
			return opAuto->preTrimSpaces(childNode->getID());

		} else if (opName == "strtoupper"){
            // only has one parameter ==>  string strtoupper  ( string $str  )
			return opAuto->preToUpperCase(childNode->getID());
        }
        else if (opName == "strtolower"){
            // only has one parameter ==>  string strtoupper  ( string $str  )
            return opAuto->preToLowerCase(childNode->getID());
        }
        else {
			cout << "Warning: Unmodeled (backward analysis) builtin general function : "
					<< opName;

			// conservative decision for operations that have not been
			// modeled yet: .*
			retMe = StrangerAutomaton::makeAnyString(opNode->getID());
			return retMe;
		}

}
//	// ********************************************************************************
//
string ForwardImageComputer::getLiteralValue( DepGraphNode* node) {
    string retMe = "";
     DepGraphNormalNode* normalNode = dynamic_cast< DepGraphNormalNode*>(node);
    if (normalNode == NULL)
        throw runtime_error("can not cast DepGraphNode into DepGraphNormalNode in getLiteralValue");
    TacPlace* place = normalNode->getPlace();
    if (dynamic_cast<Literal*>(place) != NULL) {
        retMe = place->toString();
    }
    else
        throw StrangerStringAnalysisException(stringbuilder() << "SNH: node should be literal.\n");
    return retMe;
}



//	// ********************************************************************************
//
bool ForwardImageComputer::isLiteral( DepGraphNode* node, NodesList successors) {
    if ((dynamic_cast< DepGraphNormalNode*>(node) != NULL)  && (successors.empty())){
         DepGraphNormalNode* normalNode = dynamic_cast< DepGraphNormalNode*>(node);
        TacPlace* place = normalNode->getPlace();
        if (dynamic_cast<Literal*>(place) != NULL)
            return true;
        else
            return false;
    } else
        return false;
}
//	// ********************************************************************************
//
//	private void doBackwardFixPointComputationForSCC(DepGraph inputDepGraph,
//			DepGraph origDepGraph,
//			Map<DepGraphNode, StrangerAutomaton> backwardDeco,
//			Map<DepGraphNode, StrangerAutomaton> forwardDeco,
//			DepGraphSccNode sccNode,
//			Map<DepGraphSccNode, List<DepGraphNode>> sccNodes) {
//		debug("========================================================================", 2);
//
//		int pw = MyOptions.precise_widening;
//		int cw = MyOptions.coarse_widening;
//
//		debug("doBackwardFixPointComputaion for scc: " + sccNode.getSccID()
//				+ " -- start", 2);
//		debug("widening values: precise=" + pw + ",  coarse=" + cw, 2);
//		debug("========================================================================",2 );
//
//		StrangerAutomaton::debugToFile("//doBackwardFixPointComputaion for scc: " + sccNode.getSccID() + " -- start" << endl;
//
//		List<DepGraphNode> currentSccNodes = sccNodes.get(sccNode);
//		int currentSccId = sccNode.getSccID();
//
//		// for each node we keep number of times it was visited by the algorithms
//		Map<DepGraphNode, Integer> numOfVisits = new HashMap<DepGraphNode, Integer>();
//
//		// first thing is to have all of the nodes initialized to phi
//		for (DepGraphNode node : currentSccNodes) {
//			backwardDeco.put(node, StrangerAutomaton::makePhi(node.getID()));
//			numOfVisits.put(node, 0);
//		}
//
//		// a work list to do fixpoint computation
//		LinkedList<DepGraphNode> workList = new LinkedList<DepGraphNode>();
//		// first items in the work list are the preds of the scc
//		Set<DepGraphNode> sccPreds = inputDepGraph.getPredecessors(sccNode);
//		for (DepGraphNode sccPred: sccPreds){
//			workList.addLast(sccPred);
//		}
//
//		int iteration = 0;
//		do {
//			DepGraphNode currentNode = workList.removeFirst();
//			debug("*******  iteration "+ (iteration) +"*****************", 2);
//			// current value has changed sol
//			// we calculate the value for successors
//			for (DepGraphNode succ: origDepGraph.getSuccessors(currentNode)){
//				// if succ node is not part of the scc then do not
//				// compute any thing
//				if (succ.getSccID() != currentSccId)
//					continue;
//
//				StrangerAutomaton forwardAuto = forwardDeco.get(succ);
//
//				// save the old auto
//				StrangerAutomaton oldAuto = backwardDeco.get(succ).clone(succ.getID());
//
//				// do a regular computation
//				StrangerAutomaton tmpAuto = null;
//
//				if (currentNode instanceof DepGraphNormalNode){
//					tmpAuto = backwardDeco.get(currentNode).clone(succ.getID());
//				}
//				else if (currentNode instanceof DepGraphOpNode){
//					tmpAuto = makeBackwardAutoForOpChild((DepGraphOpNode)currentNode, succ, backwardDeco, forwardDeco, origDepGraph, true);
//				}
//				else // node is uninit or scc which can not be part of an scc
//					throw new StrangerStringAnalysisException("SNH");
//				if (tmpAuto == null)
//					throw new StrangerStringAnalysisException("SNH");
//
//				// do widening of old and new value union old then compare
//				// with old value.
//				// if different then set changed to true
//				StrangerAutomaton newAuto = tmpAuto.union(oldAuto, succ.getID());
//				int visited = numOfVisits.get(succ).intValue() + 1;
//				if (visited > iteration)
//					iteration = visited;
//				// if reached number of iterations to apply widening
//				if (visited > pw) {
//
//					// if reached number of iterations to apply coarse
//					// widening
//					if (visited > cw)
//						newAuto = oldAuto.coarseWiden(newAuto, succ
//								.getID());
//					else
//						newAuto = oldAuto.preciseWiden(newAuto, succ
//								.getID());
//				}
//				// As we guarantee old is subset of new we just check if
//				// widened is subset of old
//				if (!newAuto.checkInclusion(oldAuto, newAuto.getID(),
//						oldAuto.getID())) {
//					if (!workList.contains(succ))
//						workList.addLast(succ);
//					backwardDeco.put(succ, newAuto);
//					numOfVisits.put(succ, visited);
//				}
//
//				// the following check for emptiness is only an assertion for debugging purposes
////				if (newAuto.checkEmptiness())
////					cout << "auto is empty for node: " + succ.getID() +"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
//
//				debug("-------------------------", 2);
//			}
//		} while ((!workList.isEmpty()) && (iteration <= 30000));
//
//		StrangerAutomaton::debugToFile("//doBackwardFixPointComputaion for scc: " + sccNode.getSccID() + " -- end" << endl;
//
//		debug("========================================================================", 2);
//		debug("doBackwardFixPointComputaion for scc: " + sccNode.getSccID() + " -- end", 2);
//		debug("========================================================================", 2);
//	}
//
//	// ********************************************************************************
//
//	// take care: if trimAllowed == false, no need to call this method...
//	protected TrimInfo checkTrim(DepGraphNode node) {
//
//		// start with default triminfo: everything can be trimmed
//		TrimInfo retMe = new TrimInfo();
//
//		// handle cases where trimming is not allowed
//		if (node instanceof DepGraphOpNode) {
//			DepGraphOpNode opNode = (DepGraphOpNode) node;
//			if (opNode.isBuiltin()) {
//				String opName = opNode.getName();
//				if (opName.equals("preg_replace")
//						|| opName.equals("ereg_replace")
//						|| opName.equals("str_replace")) {
//					retMe.addNoTrim(0);
//				}
//			}
//		}
//
//		return retMe;
//	}
//
//	// ********************************************************************************
//
//	// helper class for exchanging information on whether to allow trimming
//	protected class TrimInfo {
//
//		// these indices must be trimmed
//		private List<Integer> trim;
//		// these indices must not be trimmed
//		private List<Integer> noTrim;
//		// what to do with all remaining indices
//		private boolean defaultTrim;
//
//		public TrimInfo() {
//			this.defaultTrim = true;
//			this.trim = new LinkedList<Integer>();
//			this.noTrim = new LinkedList<Integer>();
//		}
//
//		public void setDefaultTrim(boolean defaultTrim) {
//			this.defaultTrim = defaultTrim;
//		}
//
//		public void addTrim(int i) {
//			this.trim.add(i);
//		}
//
//		public void addNoTrim(int i) {
//			this.noTrim.add(i);
//		}
//
//		public boolean mayTrim(int i) {
//			if (trim.contains(i)) {
//				return true;
//			} else if (noTrim.contains(i)) {
//				return false;
//			} else {
//				return defaultTrim;
//			}
//		}
//	}
//
//

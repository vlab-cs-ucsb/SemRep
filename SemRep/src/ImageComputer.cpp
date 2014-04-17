/*
 * ImageComputer.cpp
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

#include "ImageComputer.hpp"

using namespace std;

ImageComputer::ImageComputer() {
	if (!initialized){
        throw StrangerStringAnalysisException("Forward Image Computer has not been initialized.");
    }

}

ImageComputer::~ImageComputer() {
	// TODO Auto-generated destructor stub
}

bool ImageComputer::initialized = false;
/**
 * 1- prints the dot format for result automatons
 * 2- prints the MONA internal states for result automatons
 */


StrangerAutomaton* ImageComputer::uninit_node_default_initialization = nullptr;

PerfInfo* ImageComputer::perfInfo;
//static
void ImageComputer::staticInit() {
    initialized =  true;
    StrangerAutomaton::perfInfo = ImageComputer::perfInfo;
    StrangerAutomaton::staticInit();
}



/*******************************************************************************************************************************/
/********* VALIDATION PATCH EXTRACTION PHASE METHODS ***************************************************************************/
/*******************************************************************************************************************************/

/**
 *
 * Initial Pre-Image computation for validation function computation
 * 1- Start from first __vlab_restrict function
 * 2- Union negation of all restrict functions, (in the case where third parameter of restrict function is true, you do not need to negate since it is already negated)
 * 3- If there are some other ops after first restrict, do pre-image computation for them and intersect result.
 */
AnalysisResult ImageComputer::doBackwardAnalysis_ValidationCase(DepGraph& origDepGraph, DepGraph& depGraph, StrangerAutomaton* initialAuto) {

	queue<DepGraphNode*> process_queue;
	set<DepGraphNode*> visited;
	set<int> processed_SCCs;

	bool has_validation = false;

	AnalysisResult bwValidationPatchResult;
	string message = "there is no validation function!!!";

	// initialize root node
	bwValidationPatchResult[depGraph.getRoot()->getID()] = initialAuto;


	process_queue.push(depGraph.getRoot());
	while (!process_queue.empty()) {

		DepGraphNode *curr = process_queue.front();

		if (!has_validation) {
			if (dynamic_cast< DepGraphNormalNode*>(curr) || dynamic_cast< DepGraphUninitNode*>(curr) || dynamic_cast< DepGraphOpNode*>(curr)) {
				if (dynamic_cast< DepGraphOpNode*>(curr) ) {
					DepGraphOpNode* op = dynamic_cast< DepGraphOpNode*>(curr);
					if (op->getName().find("__vlab_restrict") != string::npos) {
						has_validation = true;
						message = "validation function found!!!";
					}
				}

			} else { throw new StrangerStringAnalysisException("cannot handle node type"); }

		} else {
			if (depGraph.isSCCElement(curr)) { // handle cycles
				// do not compute a scc more than once
				auto isNotProcessed = processed_SCCs.insert(depGraph.getSCCID(curr));
				if (isNotProcessed.second) {
					doPreImageComputationForSCC_ValidationCase(origDepGraph, curr, bwValidationPatchResult);
				}
			} else {
				doPreImageComputation_ValidationCase(origDepGraph, curr, bwValidationPatchResult);
			}

		}

		process_queue.pop();

		NodesList successors = depGraph.getSuccessors(curr);
		if (!successors.empty()) {
			for (auto succ_node : successors) {
				auto isNotVisited = visited.insert(succ_node);
				if (isNotVisited.second) {
					bwValidationPatchResult[succ_node->getID()] = initialAuto;
					process_queue.push(succ_node);
				}
			}
		}
	}

	cout << "\t" << message << endl;
	return bwValidationPatchResult;
}


/**
 * Do the computation for a node
 * Get the predecessors, and compute the pre-image based on those
 */
void ImageComputer::doPreImageComputation_ValidationCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult) {

	NodesList predecessors = origDepGraph.getPredecessors(node);
	NodesList successors = origDepGraph.getSuccessors(node);
	DepGraphNormalNode* normalNode = nullptr;
	StrangerAutomaton *newAuto = nullptr, *tempAuto = nullptr;

	if (dynamic_cast< DepGraphNormalNode*>(node) || dynamic_cast< DepGraphUninitNode*>(node) || dynamic_cast< DepGraphOpNode*>(node)) {
		if (predecessors.empty()) {
			// root is already initialized
			newAuto = bwAnalysisResult[node->getID()];
		} else if (successors.empty() && (normalNode = dynamic_cast< DepGraphNormalNode*>(node))) {
			newAuto = getLiteralorConstantNodeAuto(normalNode, false);
		} else {

			for (auto pred_node : predecessors) {
				StrangerAutomaton* predAuto = nullptr;
				if (pred_node == node) {
					// ignore simple self loop (check correctness)
					continue;
				} else if (dynamic_cast< DepGraphNormalNode*>(pred_node)) {
					predAuto = bwAnalysisResult[pred_node->getID()];
				} else if (dynamic_cast< DepGraphOpNode*>(pred_node)) {
					predAuto = makePreImageForOpChild_ValidationCase(origDepGraph, dynamic_cast< DepGraphOpNode*>(pred_node), node, bwAnalysisResult);
				}

				if (predAuto == nullptr) {
					continue;
				}

				if (newAuto == nullptr) {
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

	if (newAuto == nullptr) {
		throw new StrangerStringAnalysisException("SNH: pre-image is NULL: doBackwardNodeComputation_ValidationPhase()");
	}
	bwAnalysisResult[node->getID()] = newAuto;

}

/**
 * Pre Image computation for cycles (loops) during validation phase
 */
void ImageComputer::doPreImageComputationForSCC_ValidationCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult) {
	// TODO add to command line options as an optional parameter
	int precise_widening_limit = 5;
	int coarse_widening_limit = 20;

	int scc_id = origDepGraph.getSCCID(node);

	map<int, int> visit_count;
	NodesList current_scc_nodes = origDepGraph.getSCCNodes(scc_id);

	queue<DepGraphNode*> worklist;
	set<DepGraphNode*> visited;

	// initialize all scc_nodes to phi
	for (auto& scc_node : current_scc_nodes) {
		bwAnalysisResult[scc_node->getID()] = StrangerAutomaton::makePhi(scc_node->getID());
		visit_count[scc_node->getID()] = 0;
	}

	// add the predecessors to the worklist
	for ( auto pred_node : origDepGraph.getPredecessors(node)) {
		worklist.push(pred_node);
		visited.insert(pred_node);
	}

	int iteration = 0;

	do {
		DepGraphNode* curr_node = worklist.front();
		worklist.pop();
		// calculate the values for predecessors (in a depgraph predecessors are children during forward analysis)
		for (auto succ_node : origDepGraph.getSuccessors(curr_node)) {
			// ignore nodes that are not part of the current scc
			if (!origDepGraph.isSCCElement(succ_node) || origDepGraph.getSCCID(succ_node) != scc_id)
				continue;

			StrangerAutomaton* prev_auto = bwAnalysisResult[succ_node->getID()]; // may need clone
			StrangerAutomaton* tmp_auto = nullptr;
			StrangerAutomaton* new_auto = nullptr;

			if (dynamic_cast<DepGraphNormalNode*>(curr_node) != nullptr) {
				tmp_auto = bwAnalysisResult[curr_node->getID()]; // may need clone
			} else if (dynamic_cast<DepGraphOpNode*>(curr_node) != nullptr) {
				tmp_auto = makePreImageForOpChild_ValidationCase(origDepGraph, dynamic_cast< DepGraphOpNode*>(curr_node), succ_node, bwAnalysisResult);
			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "Node cannot be an element of SCC component!, node id: " << node->getID());
			}

			if (tmp_auto == nullptr) {
				throw StrangerStringAnalysisException(stringbuilder() << "Could not calculate the corresponding automaton!, node id: " << node->getID());
			}

			new_auto = tmp_auto->union_(prev_auto, succ_node->getID());

			int new_visit_count = visit_count[succ_node->getID()] + 1;
			if (new_visit_count > iteration)
				iteration = new_visit_count;

			// decide whether to do widening operations
			if (new_visit_count > coarse_widening_limit) {
				new_auto = prev_auto->coarseWiden(new_auto, succ_node->getID());
			} else if (new_visit_count > precise_widening_limit) {
				new_auto = prev_auto->preciseWiden(new_auto, succ_node->getID());
			}

			if (!new_auto->checkInclusion(prev_auto, new_auto->getID(), prev_auto->getID())) {
				auto isVisited = visited.insert(succ_node);
				if (isVisited.second) {
					worklist.push(succ_node);
				}

				bwAnalysisResult[succ_node->getID()] = new_auto;
				visit_count[succ_node->getID()] = new_visit_count;
			}
		}

	} while( !worklist.empty() && iteration < 30000 );
}


/**
 * Do pre-image computation for validation patch
 */
StrangerAutomaton* ImageComputer::makePreImageForOpChild_ValidationCase(DepGraph& depGraph, DepGraphOpNode* opNode,
			 DepGraphNode* childNode, AnalysisResult& bwAnalysisResult) {

	StrangerAutomaton* retMe = nullptr;
	NodesList successors = depGraph.getSuccessors(opNode);
	StrangerAutomaton* opAuto = bwAnalysisResult[opNode->getID()];
	string opName = opNode->getName();

	// __vlab_restrict
	if (opName.find("__vlab_restrict") != string::npos) {
		boost::posix_time::ptime start_time = perfInfo->current_time();
		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "__vlab_restrict invalid number of arguments");
		}

		DepGraphNode* subjectNode = successors[1];
		DepGraphNode* patternNode = successors[0];
		DepGraphNode* complementNode = successors[2];

		if (childNode->equals(subjectNode)) {
			//TODO handle general case for patternAuto and complementString
			StrangerAutomaton* patternAuto = getLiteralorConstantNodeAuto(patternNode, true);
			// Union __vlab_restricts considering complement parameter
			string complementString = getLiteralOrConstantValue(complementNode);
			if (complementString.find("false") != string::npos || complementString.find("FALSE") != string::npos) {
				StrangerAutomaton* complementAuto = patternAuto->complement(patternNode->getID());
				retMe = opAuto->union_(complementAuto, childNode->getID());
				delete complementAuto;
			} else {
				retMe = opAuto->union_(patternAuto, childNode->getID());
			}

			perfInfo->pre_vlab_restrict_total_time += perfInfo->current_time() - start_time;
			perfInfo->number_of_pre_vlab_restrict++;

		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "child node (" << childNode->getID() << ") of __vlab_restrict (" << opNode->getID() << ") is not in backward path");
		}
	}  else if (opName == ".") {
		// CONCAT
		throw StrangerStringAnalysisException( "concats are not handled here until we really need");
	} else if (opName == "addslashes") {
		// only has one parameter ==>  string addslashes  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = StrangerAutomaton::addslashes(sigmaStar, opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = StrangerAutomaton::pre_addslashes(intersection, childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "trim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = sigmaStar->trimSpaces(opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = intersection->preTrimSpaces(childNode->getID());

		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "ltrim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = sigmaStar->trimSpacesLeft(opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = intersection->preTrimSpacesLeft(childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	}  else if (opName == "rtrim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = sigmaStar->trimSpacesRight(opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = intersection->preTrimSpacesRigth(childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "strtoupper" ) {
		// only has one parameter ==> string strtoupper  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = sigmaStar->toUpperCase(opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = intersection->preToUpperCase(childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	}  else if (opName == "strtolower" ) {
		// only has one parameter ==> string strtolower  ( string $str  )
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = sigmaStar->toLowerCase(opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = intersection->preToLowerCase(childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "mysql_escape_string") {
		// has one parameter
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = StrangerAutomaton::mysql_escape_string(sigmaStar, opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = StrangerAutomaton::pre_mysql_escape_string(intersection, childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "mysql_real_escape_string") {
		// has one parameter
		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
		StrangerAutomaton* forward = StrangerAutomaton::mysql_real_escape_string(sigmaStar, opNode->getID());
		StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
		retMe = StrangerAutomaton::pre_mysql_real_escape_string(intersection, childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "htmlspecialchars") {
		if (childNode->equals(successors[0])) {
			string flagString = "ENT_COMPAT";
			if (successors.size() > 1) {
				flagString = getLiteralOrConstantValue(successors[1]);
			}
			StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(opNode->getID());
			StrangerAutomaton* forward = StrangerAutomaton::htmlSpecialChars(sigmaStar, flagString, opNode->getID());
			StrangerAutomaton* intersection = opAuto->intersect(forward, childNode->getID());
			retMe = StrangerAutomaton::preHtmlSpecialChars(intersection, flagString, childNode->getID());
			delete sigmaStar;
			delete forward;
			delete intersection;
		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of htmlspecialchars (" << opNode->getID() << ") is not in backward path,\ncheck implementation");
		}

	} else if (opName == "preg_replace" || opName == "ereg_replace" || opName == "str_replace") {

		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "replace invalid number of arguments");
		}

		DepGraphNode* subjectNode = successors[2];
		DepGraphNode* patternNode = successors[0];
		DepGraphNode* replaceNode = successors[1];

		StrangerAutomaton* subjectAuto = opAuto;

		if (childNode->equals(subjectNode)) {

			AnalysisResult analysisResult;
			doForwardAnalysis_GeneralCase(depGraph, patternNode, analysisResult);
			doForwardAnalysis_GeneralCase(depGraph, replaceNode, analysisResult);

			StrangerAutomaton* patternAuto = analysisResult[patternNode->getID()];
			StrangerAutomaton* replaceAuto = analysisResult[replaceNode->getID()];

			StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(subjectAuto->getID());
			StrangerAutomaton* forward = StrangerAutomaton::general_replace(patternAuto, replaceAuto, sigmaStar, subjectAuto->getID());
			StrangerAutomaton* intersection = subjectAuto->intersect(forward, childNode->getID());
			string replaceStr = replaceAuto->getStr();

			// checking for special case where a character is escaped by another character
			if (patternAuto->isSingleton()) {
				string patternStr = patternAuto->generateSatisfyingExample();
				if ( replaceStr.length() == 2 && patternStr.length() == 1 && patternStr[0] == replaceStr[1]) {
					retMe = StrangerAutomaton::general_replace(replaceAuto, patternAuto, intersection, childNode->getID());
				} else {
					retMe = intersection->preReplace(patternAuto, replaceStr, childNode->getID());
				}
			} else {
				retMe = intersection->preReplace(patternAuto, replaceStr, childNode->getID());
			}

			delete sigmaStar;
			delete forward;
			delete intersection;

		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of preg_replace (" << opNode->getID() << ") is not in backward path,\ncheck implementation: "
					"makeBackwardAutoForOpChild_ValidationPhase()");
		}


	}  else if (opName == "substr"){

		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: substr invalid number of arguments: "
					"makeForwardAutoForOp_RegularPhase()");
		}

		DepGraphNode* startNode = successors[1];
		DepGraphNode* lengthNode = successors[2];

		StrangerAutomaton* subjectAuto = opAuto;

		AnalysisResult analysisResult;
		doForwardAnalysis_GeneralCase(depGraph, startNode, analysisResult);
		doForwardAnalysis_GeneralCase(depGraph, lengthNode, analysisResult);

		string startValue = analysisResult[startNode->getID()]->getStr();
		int start = stoi(startValue);
		string lengthValue = analysisResult[lengthNode->getID()]->getStr();
		int length = stoi(lengthValue);

		StrangerAutomaton* sigmaStar = StrangerAutomaton::makeAnyString(subjectAuto->getID());
		StrangerAutomaton* forward = sigmaStar->substr(start, length, subjectAuto->getID());
		StrangerAutomaton* intersection = subjectAuto->intersect(forward, childNode->getID());
		retMe = intersection->pre_substr(start, length, childNode->getID());
		delete sigmaStar;
		delete forward;
		delete intersection;

	} else if (opName == "") {

	} else if (opName == "md5") {
		retMe = StrangerAutomaton::makeAnyString(childNode->getID());
	} else {
		throw StrangerStringAnalysisException( "Not implemented yet for validation phase: " + opName);
	}

//	cout << endl << "auto after each operation : " << opName << endl << endl;
//	retMe->toDotAscii(0);
//	cout << endl << endl;
	return retMe;

}

/*******************************************************************************************************************************/
/*********** SANITIZATION PATCH EXTRACTION METHODS *****************************************************************************/
/*******************************************************************************************************************************/

/**
 *
 */
void ImageComputer::doForwardAnalysis_SingleInput(DepGraph& origDepGraph,  DepGraphUninitNode* inputNode, AnalysisResult& analysisResult) {
	DepGraph inputDepGraph = origDepGraph.getInputRelevantGraph(inputNode);
	doForwardAnalysis_SingleInput(origDepGraph, inputDepGraph, analysisResult);
}

/**
 * Do forward analysis from an input node to a sink
 *
 */
void ImageComputer::doForwardAnalysis_SingleInput(
		DepGraph& origDepGraph, DepGraph& inputDepGraph, AnalysisResult& analysisResult) {

	stack<DepGraphNode*> process_stack;
	set<DepGraphNode*> visited;

	// TODO remove that from here for a general option
	if (uninit_node_default_initialization)
		delete uninit_node_default_initialization;
	uninit_node_default_initialization = StrangerAutomaton::makePhi();


	process_stack.push( inputDepGraph.getRoot() );
	while (!process_stack.empty()) {

		DepGraphNode *curr = process_stack.top();
		auto isNotVisited = visited.insert(curr);
		NodesList successors = inputDepGraph.getSuccessors(curr);

		if (!successors.empty() && isNotVisited.second) {
			for (NodesListConstReverseIterator it = successors.rbegin(); it != successors.rend(); it++) {
				if (analysisResult.find((*it)->getID()) == analysisResult.end()) {
					process_stack.push(*it);
				}
			}
		} else {
			doPostImageComputation_SingleInput(origDepGraph, inputDepGraph, curr, analysisResult);
			process_stack.pop();
		}
	}

	return;
}


void ImageComputer::doPostImageComputation_SingleInput(
		DepGraph& origDepGraph, DepGraph& inputDepGraph, DepGraphNode* node, AnalysisResult& analysisResult) {

	NodesList successors = origDepGraph.getSuccessors(node);

	StrangerAutomaton* newAuto = nullptr;
    DepGraphNormalNode* normalnode;
    DepGraphOpNode* opNode;
    DepGraphUninitNode* uninitNode;
    if ((normalnode = dynamic_cast<DepGraphNormalNode*>(node)) != nullptr) {
    	if (successors.empty()) {
			newAuto = getLiteralorConstantNodeAuto(normalnode, false);
    	} else {
    		// an interior node, union of all its successors
    		for (auto succ_node : successors) {
    			if (succ_node->getID() == node->getID() ) {
    				// avoid simple loops
    				continue;
    			}
    			// explore new paths
				if (analysisResult.find(succ_node->getID()) == analysisResult.end()) {
					cout << "exploring succ_node: " << succ_node->getID() << endl;
					doForwardAnalysis_GeneralCase(origDepGraph, succ_node, analysisResult);
				}

				StrangerAutomaton *succAuto = analysisResult[succ_node->getID()];
    			if (newAuto == nullptr) {
    				newAuto = succAuto->clone(node->getID());
    			} else {
    				StrangerAutomaton* temp = newAuto;
    				newAuto = newAuto->union_(succAuto, node->getID());
    				delete temp;
    			}
    		}
    	}

    } else if ((opNode = dynamic_cast<DepGraphOpNode*>(node)) != nullptr) {
		newAuto = makePostImageForOp_GeneralCase(origDepGraph, opNode, analysisResult);
    } else if ((uninitNode = dynamic_cast<DepGraphUninitNode*>(node)) != nullptr) {
    	// input node that we are interested in should have been initialized already
    	if (analysisResult.find(node->getID()) == analysisResult.end()){
    		throw StrangerStringAnalysisException(stringbuilder() << "input node id(" << uninitNode->getID() << ") automaton must be initizalized before analysis begins!");
    	}
    	newAuto = analysisResult[node->getID()];
    } else {
    	throw StrangerStringAnalysisException(stringbuilder() << "Cannot figure out node type!, node id: " << node->getID());
    }

    if (newAuto == nullptr) {
    	throw StrangerStringAnalysisException(stringbuilder() << "Forward automaton cannot be computed!, node id: " << node->getID());
    }
    analysisResult[node->getID()] = newAuto;
}

/*******************************************************************************************************************************/
/*********** REGULAR BACKWARD IMAGE COMPUTATION METHODS *************************************************************************/
/*******************************************************************************************************************************/

/**
 * Giving an initial auto for root, using the results from previous forward analysis,
 * do a backward analysis. Second parameter is the input relevant depgraph.
 *
 */
AnalysisResult ImageComputer::doBackwardAnalysis_GeneralCase(
		DepGraph& origDepGraph, DepGraph& depGraph, StrangerAutomaton* initialAuto, const AnalysisResult& fwAnalysisResult) {

	queue<DepGraphNode*> process_queue;
	set<DepGraphNode*> visited;
	set<int> processed_SCCs;

	AnalysisResult bwAnalysisResult;

	// initialize root node
	bwAnalysisResult[depGraph.getRoot()->getID()] = initialAuto;

	process_queue.push(depGraph.getRoot());
	while (!process_queue.empty()) {

	DepGraphNode *curr = process_queue.front();
	if (depGraph.isSCCElement(curr)) { // handle cycles
		// do not compute a scc more than once
		auto isNotProcessed = processed_SCCs.insert(depGraph.getSCCID(curr));
		if (isNotProcessed.second) {
			doPreImageComputationForSCC_GeneralCase(origDepGraph, curr, bwAnalysisResult, fwAnalysisResult);
		}
	} else {
		doPreImageComputation_GeneralCase(origDepGraph, curr, bwAnalysisResult, fwAnalysisResult);
	}

	process_queue.pop();

	NodesList successors = depGraph.getSuccessors(curr);
		if (!successors.empty()) {
			for (auto succ_node : successors) {
				auto isNotVisited = visited.insert(succ_node);
				if (isNotVisited.second) {
					process_queue.push(succ_node);
				}
			}
		}
	}

	return bwAnalysisResult;
}

/**
 *
 */
void ImageComputer::doPreImageComputation_GeneralCase(
		DepGraph& origDepGraph, DepGraphNode* node,
		AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult) {

	NodesList predecessors = origDepGraph.getPredecessors(node);
	NodesList successors = origDepGraph.getSuccessors(node);
	DepGraphNormalNode* normalNode = nullptr;
	StrangerAutomaton *newAuto = nullptr, *tempAuto = nullptr;

	if (dynamic_cast< DepGraphNormalNode*>(node) || dynamic_cast< DepGraphUninitNode*>(node) || dynamic_cast< DepGraphOpNode*>(node)) {
		if (predecessors.empty()) {
			// root is already initialized
			newAuto = bwAnalysisResult[node->getID()];
		} else if (successors.empty() && (normalNode = dynamic_cast< DepGraphNormalNode*>(node))) {
			newAuto = getLiteralorConstantNodeAuto(normalNode, false);
		} else {
			// the automa is union of all prodecessors and interstect with forward analysis result
			StrangerAutomaton* forwardAuto = fwAnalysisResult.find(node->getID())->second;
			for (auto pred_node : predecessors) {
				StrangerAutomaton* predAuto = nullptr;
				if (pred_node == node) {
					// ignore simple self loop (check correctness)
					continue;
				} else if (dynamic_cast< DepGraphNormalNode*>(pred_node)) {
					predAuto = bwAnalysisResult[pred_node->getID()];
				} else if (dynamic_cast< DepGraphOpNode*>(pred_node)) {
					predAuto = makePreImageForOpChild_GeneralCase(origDepGraph,dynamic_cast< DepGraphOpNode*>(pred_node), node,
													bwAnalysisResult, fwAnalysisResult);
				}

				if (predAuto == nullptr) {
					continue;
				}

				if (newAuto == nullptr) {
					newAuto = predAuto->clone(node->getID());
				} else {
					tempAuto = newAuto;
					newAuto = newAuto->union_(predAuto, node->getID());
					delete tempAuto;
					delete predAuto;
				}
			}

			if (newAuto == nullptr) {
				throw StrangerStringAnalysisException("Cannot calculate backward auto, fix me\nndoBackwardNodeComputation_RegularPhase()");
			}

			tempAuto = newAuto;
			newAuto = forwardAuto->intersect(newAuto, node->getID());
			delete tempAuto;
		}

	} else {
		throw new StrangerStringAnalysisException("SNH: cannot handle node type:\ndoBackwardNodeComputation_RegularPhase()");
	}


	if (newAuto == nullptr) {
		throw new StrangerStringAnalysisException("SNH: pre-image is NULL:\ndoBackwardNodeComputation_RegularPhase()");
	}

	bwAnalysisResult[node->getID()] = newAuto;
}

/**
 * Pre Image Computation for cycles (loops)
 */
void ImageComputer::doPreImageComputationForSCC_GeneralCase(DepGraph& origDepGraph, DepGraphNode* node, AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult) {
	// TODO add to command line options as an optional parameter
	int precise_widening_limit = 5;
	int coarse_widening_limit = 20;

	int scc_id = origDepGraph.getSCCID(node);

	map<int, int> visit_count;
	NodesList current_scc_nodes = origDepGraph.getSCCNodes(scc_id);

	queue<DepGraphNode*> worklist;
	set<DepGraphNode*> visited;

	// initialize all scc_nodes to phi
	for (auto& scc_node : current_scc_nodes) {
		bwAnalysisResult[scc_node->getID()] = StrangerAutomaton::makePhi(scc_node->getID());
		visit_count[scc_node->getID()] = 0;
	}

	// add the predecessors to the worklist
	for ( auto pred_node : origDepGraph.getPredecessors(node)) {
		worklist.push(pred_node);
		visited.insert(pred_node);
	}

	int iteration = 0;

	do {
		DepGraphNode* curr_node = worklist.front();
		worklist.pop();
		// calculate the values for predecessors (in a depgraph predecessors are children during forward analysis)
		for (auto succ_node : origDepGraph.getSuccessors(curr_node)) {
			// ignore nodes that are not part of the current scc
			if (!origDepGraph.isSCCElement(succ_node) || origDepGraph.getSCCID(succ_node) != scc_id)
				continue;

			StrangerAutomaton* forward_auto = fwAnalysisResult.find(succ_node->getID())->second;
			StrangerAutomaton* prev_auto = bwAnalysisResult[succ_node->getID()]; // may need clone
			StrangerAutomaton* tmp_auto = nullptr;
			StrangerAutomaton* new_auto = nullptr;

			if (dynamic_cast<DepGraphNormalNode*>(curr_node) != nullptr) {
				tmp_auto = bwAnalysisResult[curr_node->getID()]; // may need clone
			} else if (dynamic_cast<DepGraphOpNode*>(curr_node) != nullptr) {
				tmp_auto = makePreImageForOpChild_GeneralCase(origDepGraph, dynamic_cast< DepGraphOpNode*>(curr_node), succ_node,
						bwAnalysisResult, fwAnalysisResult);
			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "Node cannot be an element of SCC component!, node id: " << node->getID());
			}

			if (tmp_auto == nullptr) {
				throw StrangerStringAnalysisException(stringbuilder() << "Could not calculate the corresponding automaton!, node id: " << node->getID());
			}

			new_auto = tmp_auto->union_(prev_auto, succ_node->getID());

			int new_visit_count = visit_count[succ_node->getID()] + 1;
			if (new_visit_count > iteration)
				iteration = new_visit_count;

			// decide whether to do widening operations
			if (new_visit_count > coarse_widening_limit) {
				new_auto = prev_auto->coarseWiden(new_auto, succ_node->getID());
			} else if (new_visit_count > precise_widening_limit) {
				new_auto = prev_auto->preciseWiden(new_auto, succ_node->getID());
			}

			if (!new_auto->checkInclusion(prev_auto, new_auto->getID(), prev_auto->getID())) {
				auto isVisited = visited.insert(succ_node);
				if (isVisited.second) {
					worklist.push(succ_node);
				}

				tmp_auto = new_auto;
				new_auto = forward_auto->intersect(new_auto, node->getID());
				delete tmp_auto;

				bwAnalysisResult[succ_node->getID()] = new_auto;
				visit_count[succ_node->getID()] = new_visit_count;
			}
		}

	} while( !worklist.empty() && iteration < 30000 );
}

/**
 *
 */
StrangerAutomaton* ImageComputer::makePreImageForOpChild_GeneralCase(
		DepGraph& depGraph, DepGraphOpNode* opNode, DepGraphNode* childNode,
		AnalysisResult& bwAnalysisResult, const AnalysisResult& fwAnalysisResult) {

	StrangerAutomaton* retMe = nullptr;
	NodesList successors = depGraph.getSuccessors(opNode);
	StrangerAutomaton* opAuto = bwAnalysisResult[opNode->getID()];
	string opName = opNode->getName();



	// __vlab_restrict
	if (opName.find("__vlab_restrict") != string::npos) {
		boost::posix_time::ptime start_time = perfInfo->current_time();
		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "__vlab_restrict invalid number of arguments");
		}

		DepGraphNode* subjectNode = successors[1];
		DepGraphNode* patternNode = successors[0];
		DepGraphNode* complementNode = successors[2];

		if (childNode->equals(subjectNode)){
			retMe = opAuto->clone(childNode->getID());
		} else if (childNode->equals(patternNode) || childNode->equals(complementNode)) {
			throw StrangerStringAnalysisException(stringbuilder() << "child node (" << childNode->getID() << ") of __vlab_restrict (" << opNode->getID() << ") should not be on the backward path");
		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "child node (" << childNode->getID() << ") of __vlab_restrict (" << opNode->getID() << ") is not in backward path");
		}
		perfInfo->pre_vlab_restrict_total_time += perfInfo->current_time() - start_time;
		perfInfo->number_of_pre_vlab_restrict++;

	} else if (opName == ".") {
		if (successors.size() < 2)
			throw StrangerStringAnalysisException(stringbuilder() << "less than two successors for concat node " << opNode->getID());

		StrangerAutomaton* concatAuto = opAuto;

		DepGraphNode* leftSibling = successors[0];
		DepGraphNode* rightSibling = successors[1];

		AnalysisResultConstIterator leftIt = fwAnalysisResult.find(leftSibling->getID());
		AnalysisResultConstIterator rightIt = fwAnalysisResult.find(rightSibling->getID());

		if (childNode->equals(leftSibling)){
			if (leftIt == fwAnalysisResult.end()) {
				throw StrangerStringAnalysisException(stringbuilder() << "Should not visit left node(" << leftSibling->getID() << ") in concat");
			} else if (rightIt == fwAnalysisResult.end()) {
				// we can just clone the previous auto, in that case actual concat operation is not done during forward analysis
				retMe = concatAuto->clone(childNode->getID());
			} else {
				if (isLiteralOrConstant(rightSibling, depGraph.getSuccessors(rightSibling))){
					string value = getLiteralOrConstantValue(rightSibling);
					retMe = concatAuto->leftPreConcatConst(value, childNode->getID());
				} else {
					StrangerAutomaton* rightSiblingAuto = fwAnalysisResult.find(rightSibling->getID())->second;
					retMe = concatAuto->leftPreConcat(rightSiblingAuto, childNode->getID());
				}
			}

		} else if (childNode->equals(rightSibling)){

			if (rightIt == fwAnalysisResult.end()) {
				throw StrangerStringAnalysisException(stringbuilder() << "Should not visit right node(" << leftSibling->getID() << ") in concat");
			} else if (leftIt == fwAnalysisResult.end()) {
				// we can just clone the previous auto, in that case actual concat operation is not done during forward analysis
				retMe = concatAuto->clone(childNode->getID());
			} else {
				if (isLiteralOrConstant(leftSibling, depGraph.getSuccessors(leftSibling))){
					string value = getLiteralOrConstantValue(leftSibling);
					retMe = concatAuto->rightPreConcatConst(value, childNode->getID());
				} else {
					StrangerAutomaton* leftSiblingAuto = fwAnalysisResult.find(leftSibling->getID())->second;
					retMe = concatAuto->rightPreConcat(leftSiblingAuto, childNode->getID());
				}
			}

		}else {
			throw StrangerStringAnalysisException(stringbuilder() << "child (" << childNode->getID() << ") of concat (" << opNode->getID() << ") is not equal to any of the two successors.");
		}

	} else if (opName == "addslashes") {
		// only has one parameter ==>  string addslashes  ( string $str  )
		retMe = StrangerAutomaton::pre_addslashes(opAuto,childNode->getID());

	} else if (opName == "trim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		retMe = opAuto->preTrimSpaces(childNode->getID());
	} else if (opName == "rtrim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		retMe = opAuto->preTrimSpacesRigth(childNode->getID());
	} else if (opName == "ltrim" ) {
		// only has one parameter ==>  string trim  ( string $str  )
		retMe = opAuto->preTrimSpacesLeft(childNode->getID());

	}  else if (opName == "strtoupper" ) {
		// only has one parameter ==>  string strtoupper  ( string $str  )
		retMe = opAuto->preToUpperCase(childNode->getID());

	}  else if (opName == "strtolower" ) {
		// only has one parameter ==>  string strtolower  ( string $str  )
		retMe = opAuto->preToLowerCase(childNode->getID());

	} else if (opName == "htmlspecialchars") {
		if (childNode->equals(successors[0])) {
			string flagString = "ENT_COMPAT";
			if (successors.size() > 1) {
				DepGraphNode* flagNode = successors[1];
				StrangerAutomaton* flagAuto = fwAnalysisResult.find(flagNode->getID())->second;
				flagString = flagAuto->getStr();
			}
			retMe = StrangerAutomaton::preHtmlSpecialChars(opAuto, flagString, childNode->getID());
		} else {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: child node (" << childNode->getID() << ") of htmlspecialchars (" << opNode->getID() << ") is not in backward path");
		}
	} else if (opName == "mysql_escape_string") {
		// has one parameter
		retMe = StrangerAutomaton::pre_mysql_escape_string(opAuto, childNode->getID());

	}  else if (opName == "mysql_real_escape_string") {
		// has one parameter
		retMe = StrangerAutomaton::pre_mysql_real_escape_string(opAuto, childNode->getID());

	} else if (opName == "preg_replace" || opName == "ereg_replace" || opName == "str_replace") {

		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "replace invalid number of arguments");
		}

		DepGraphNode* patternNode = successors[0];
		DepGraphNode* replaceNode = successors[1];

		StrangerAutomaton* subjectAuto = opAuto;

		StrangerAutomaton* patternAuto = fwAnalysisResult.find(patternNode->getID())->second;
		StrangerAutomaton* replaceAuto = fwAnalysisResult.find(replaceNode->getID())->second;
		string replaceStr = replaceAuto->getStr();

		// checking for special case where a character is escaped by another character
		if (patternAuto->isSingleton()) {
			string patternStr = patternAuto->generateSatisfyingExample();
			if ( replaceStr.length() == 2 && patternStr.length() == 1 && patternStr[0] == replaceStr[1]) {
				retMe = StrangerAutomaton::general_replace(replaceAuto, patternAuto, subjectAuto, childNode->getID());
			} else {
				retMe = subjectAuto->preReplace(patternAuto, replaceStr, childNode->getID());
			}
		} else {
			retMe = subjectAuto->preReplace(patternAuto, replaceStr, childNode->getID());
		}

	} else if (opName == "substr"){

		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "substr invalid number of arguments");
		}
		DepGraphNode* startNode = successors[1];
		DepGraphNode* lengthNode = successors[2];

		StrangerAutomaton* subjectAuto = opAuto;
		StrangerAutomaton* startAuto = fwAnalysisResult.find(startNode->getID())->second;
		StrangerAutomaton* lengthAuto = fwAnalysisResult.find(lengthNode->getID())->second;

		string startValue = startAuto->getStr();
		int start = stoi(startValue);

		string lengthValue = lengthAuto->getStr();
		int length = stoi(lengthValue);

		StrangerAutomaton* substrAuto = subjectAuto->pre_substr(start,length,opNode->getID());
		retMe = substrAuto;

	} else if (opName == "md5") {
		retMe = StrangerAutomaton::makeAnyString(opNode->getID());
	} else {
		throw StrangerStringAnalysisException( "Not implemented yet for regular validation phase: " + opName);
	}

	return retMe;


}

// ********************************************************************************
//
string ImageComputer::getLiteralOrConstantValue( DepGraphNode* node) {
    string retMe = "";
    DepGraphNormalNode* normalNode = dynamic_cast< DepGraphNormalNode*>(node);
    if (normalNode == nullptr)
        throw runtime_error("can not cast DepGraphNode into DepGraphNormalNode");
    TacPlace* place = normalNode->getPlace();
    if (dynamic_cast<Literal*>(place) != nullptr || dynamic_cast<Constant*>(place) != nullptr) {
        retMe = place->toString();
    }
    else
        throw StrangerStringAnalysisException(stringbuilder() << "SNH: node should be literal.\n");
    return retMe;
}



// ********************************************************************************
//
bool ImageComputer::isLiteralOrConstant( DepGraphNode* node, NodesList successors) {
    if ((dynamic_cast< DepGraphNormalNode*>(node) != nullptr)  && (successors.empty())){
         DepGraphNormalNode* normalNode = dynamic_cast< DepGraphNormalNode*>(node);
        TacPlace* place = normalNode->getPlace();
        if (dynamic_cast<Literal*>(place) != nullptr || dynamic_cast<Constant*>(place) )
            return true;
        else
            return false;
    } else
        return false;
}

StrangerAutomaton* ImageComputer::getLiteralorConstantNodeAuto(DepGraphNode* node, bool is_vlab_restrict) {
	StrangerAutomaton* retMe = nullptr;
    DepGraphNormalNode* normalNode = dynamic_cast< DepGraphNormalNode*>(node);
    if (normalNode == nullptr)
        throw runtime_error("can not cast DepGraphNode into DepGraphNormalNode");
	TacPlace* place = normalNode->getPlace();
	if (dynamic_cast<Literal*>(place) != nullptr || dynamic_cast<Constant*>(place)) {
		string value = place->toString();
		// check if it is a regular expression
		if (value.find_first_of('/') == 0 &&
				value.find_last_of('/') == (value.length() -1) ) {
			string regString = value.substr( 1,value.length()-2);
			if(regString.find_first_of('^') == 0 &&
					regString.find_last_of('$') == (regString.length() -1)){
				regString = "/" + regString.substr( 1,regString.length()-2) + "/";
			}
			else if (is_vlab_restrict) {
				regString = "/.*(" + regString + ").*/";
			}
			else {
				regString = "/" + regString + "/";
			}
			retMe = StrangerAutomaton::regExToAuto(regString, true, node->getID());
		}
		else {
			retMe = StrangerAutomaton::makeString(value, node->getID());
		}
	}
	else {
		throw StrangerStringAnalysisException(stringbuilder() << "Unhandled node type, node id: " << node->getID());
	}

	return retMe;
}

/**
 * Calculate the automaton for the given node, using post-order dfs traversal of the Depgraph starting from given node
 */
void ImageComputer::doForwardAnalysis_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult) {


	stack<DepGraphNode*> process_stack;
	set<DepGraphNode*> visited;
	set<int> processed_SCCs;

	// TODO remove that from here for a general option
	if (!uninit_node_default_initialization)
		uninit_node_default_initialization = StrangerAutomaton::makePhi();

	process_stack.push(node);
	while (!process_stack.empty()) {

		DepGraphNode *curr = process_stack.top();
		auto isNotVisited = visited.insert(curr);
		NodesList successors = depGraph.getSuccessors(curr);

		if (!successors.empty() && isNotVisited.second) {
			for (NodesListConstReverseIterator it = successors.rbegin(); it != successors.rend(); it++) {
				if (analysisResult.find((*it)->getID()) == analysisResult.end()) {
					process_stack.push(*it);
				}
			}
		} else {
			if (depGraph.isSCCElement(curr)) { // handle cycles
				// do not compute a scc more than once
				auto isNotProcessed = processed_SCCs.insert(depGraph.getSCCID(curr));
				if (isNotProcessed.second) {
					doPostImageComputationForSCC_GeneralCase(depGraph, curr, analysisResult);
				}
			} else {
				doPostImageComputation_GeneralCase(depGraph, curr, analysisResult);
			}
			process_stack.pop();
		}
	  }
}

void ImageComputer::doPostImageComputation_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult) {

	NodesList successors = depGraph.getSuccessors(node);

	StrangerAutomaton* newAuto = nullptr;
	DepGraphNormalNode* normalNode;
	DepGraphOpNode* opNode;
	DepGraphUninitNode* uninitNode;
	if ((normalNode = dynamic_cast<DepGraphNormalNode*>(node)) != nullptr) {
		if (successors.empty()) {
			newAuto = getLiteralorConstantNodeAuto(normalNode, false);
		} else {
			// an interior node, union of all its successors
			for (auto succ_node : successors) {
				if (succ_node->getID() == node->getID() ) {
					// avoid simple loops
					continue;
				}

				StrangerAutomaton *succAuto = analysisResult[succ_node->getID()];
				if (newAuto == nullptr) {
					newAuto = succAuto->clone(node->getID());
				} else {
					StrangerAutomaton* temp = newAuto;
					newAuto = newAuto->union_(succAuto, node->getID());
					delete temp;
				}
			}
		}
	} else if ((opNode = dynamic_cast<DepGraphOpNode*>(node)) != nullptr) {
		newAuto = makePostImageForOp_GeneralCase(depGraph, opNode, analysisResult);
	} else if ((uninitNode = dynamic_cast<DepGraphUninitNode*>(node)) != nullptr) {
		newAuto = ImageComputer::uninit_node_default_initialization->clone(node->getID());
	} else {
		throw StrangerStringAnalysisException(stringbuilder() << "Cannot figure out node type!, node id: " << node->getID());
	}

	if (newAuto == nullptr) {
		throw StrangerStringAnalysisException(stringbuilder() << "Forward automaton cannot be computed!, node id: " << node->getID());
	}
	analysisResult[node->getID()] = newAuto;
}

/**
 * Post Image computation for cycles (loops)
 */
void ImageComputer::doPostImageComputationForSCC_GeneralCase(DepGraph& depGraph, DepGraphNode* node, AnalysisResult& analysisResult) {

	// TODO add to command line options as an optional parameter
	int precise_widening_limit = 5;
	int coarse_widening_limit = 20;

	int scc_id = depGraph.getSCCID(node);

	map<int, int> visit_count;
	NodesList current_scc_nodes = depGraph.getSCCNodes(scc_id);

	queue<DepGraphNode*> worklist;
	set<DepGraphNode*> visited;

	// initialize all scc_nodes to phi
	for (auto& scc_node : current_scc_nodes) {
		analysisResult[scc_node->getID()] = StrangerAutomaton::makePhi(scc_node->getID());
		visit_count[scc_node->getID()] = 0;
	}

	// add the successors to the worklist (in a depgraph successors are parents during forward analysis)
	for ( auto succ_node : depGraph.getSuccessors(node)) {
		worklist.push(succ_node);
		visited.insert(succ_node);
	}

	int iteration = 0;

	do {
		DepGraphNode* curr_node = worklist.front();
		worklist.pop();
		// calculate the values for predecessors (in a depgraph predecessors are children during forward analysis)
		for (auto pred_node : depGraph.getPredecessors(curr_node)) {
			// ignore nodes that are not part of the current scc
			if (!depGraph.isSCCElement(pred_node) || depGraph.getSCCID(pred_node) != scc_id)
				continue;

			StrangerAutomaton* prev_auto = analysisResult[pred_node->getID()]; // may need clone
			StrangerAutomaton* tmp_auto = nullptr;
			StrangerAutomaton* new_auto = nullptr;

			if (dynamic_cast<DepGraphNormalNode*>(pred_node) != nullptr) {
				tmp_auto = analysisResult[curr_node->getID()]; // may need clone
			} else if (dynamic_cast<DepGraphOpNode*>(pred_node) != nullptr) {
				tmp_auto = makePostImageForOp_GeneralCase(depGraph, dynamic_cast<DepGraphOpNode*>(pred_node), analysisResult);
			} else {
				throw StrangerStringAnalysisException(stringbuilder() << "Node cannot be an element of SCC component!, node id: " << node->getID());
			}

			if (tmp_auto == nullptr) {
				throw StrangerStringAnalysisException(stringbuilder() << "Could not calculate the corresponding automaton!, node id: " << node->getID());
			}

			new_auto = tmp_auto->union_(prev_auto, pred_node->getID());

			int new_visit_count = visit_count[pred_node->getID()] + 1;
			if (new_visit_count > iteration)
				iteration = new_visit_count;

			// decide whether to do widening operations
			if (new_visit_count > coarse_widening_limit) {
				new_auto = prev_auto->coarseWiden(new_auto, pred_node->getID());
			} else if (new_visit_count > precise_widening_limit) {
				new_auto = prev_auto->preciseWiden(new_auto, pred_node->getID());
			}

			if (!new_auto->checkInclusion(prev_auto, new_auto->getID(), prev_auto->getID())) {
				auto isVisited = visited.insert(pred_node);
				if (isVisited.second) {
					worklist.push(pred_node);
				}
				analysisResult[pred_node->getID()] = new_auto;
				visit_count[pred_node->getID()] = new_visit_count;
			}
		}

	} while( !worklist.empty() && iteration < 30000 );
}

/**
 * Calculates post image of an operation
 * Recursive calls may only happen if the function is called from single input analysis functions
 */
StrangerAutomaton* ImageComputer::makePostImageForOp_GeneralCase(DepGraph& depGraph, DepGraphOpNode* opNode, AnalysisResult& analysisResult) {
	NodesList successors = depGraph.getSuccessors(opNode);
	StrangerAutomaton* retMe = nullptr;
	string opName = opNode->getName();

	// __vlab_restrict
	if (opName.find("__vlab_restrict") != string::npos) {
		boost::posix_time::ptime start_time = perfInfo->current_time();
		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "__vlab_restrict invalid number of arguments: " << opNode->getID());
		}

		DepGraphNode* subjectNode = successors[1];
		DepGraphNode* patternNode = successors[0];
		DepGraphNode* complementNode = successors[2];

		// recursion happens only in single input mode when needed
		if (analysisResult.find(subjectNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, subjectNode, analysisResult);
		}

		// TODO handle general case
		if (analysisResult.find(patternNode->getID()) == analysisResult.end()) {
//				doForwardAnalysis_GeneralCase(depGraph, patternNode, analysisResult);
			analysisResult[patternNode->getID()] = getLiteralorConstantNodeAuto(patternNode, true);
		}
		// TODO handle general case
		if (analysisResult.find(complementNode->getID()) == analysisResult.end()) {
//				doForwardAnalysis_GeneralCase(depGraph, complementNode, analysisResult);

		}

		StrangerAutomaton* subjectAuto = analysisResult[subjectNode->getID()];
		StrangerAutomaton* patternAuto = analysisResult[patternNode->getID()];
//			string complementString = analysisResult[complementNode->getID()]->getStr();
		string complementString = getLiteralOrConstantValue(complementNode);
		if (complementString.find("false") != string::npos || complementString.find("FALSE") != string::npos) {
			retMe = subjectAuto->intersect(patternAuto, opNode->getID());
		} else {
			StrangerAutomaton* complementAuto = patternAuto->complement(patternNode->getID());
			retMe = subjectAuto->intersect(complementAuto, opNode->getID());
			delete complementAuto;
		}
		perfInfo->vlab_restrict_total_time += perfInfo->current_time() - start_time;
		perfInfo->number_of_vlab_restrict++;

	} else if (opName == ".") {
		// TODO add option to ignore concats (heuristic)
		for (auto succ_node : successors){
			if (analysisResult.find(succ_node->getID()) == analysisResult.end()) {
				if (handle_concats) {
					doForwardAnalysis_GeneralCase(depGraph, succ_node, analysisResult);
				} else {
					continue;
				}
			}
			StrangerAutomaton* succAuto = analysisResult[succ_node->getID()];
			if (retMe == nullptr) {
				retMe = succAuto->clone(opNode->getID());
			} else {
				StrangerAutomaton* temp = retMe;
				retMe = retMe->concatenate(succAuto, opNode->getID());
				delete temp;
			}
		}
		if (retMe == nullptr) {
			throw StrangerStringAnalysisException(stringbuilder() << "Check successors of concatenation: " << opNode->getID());
		}

	} else if (opName == "preg_replace" || opName == "ereg_replace" || opName == "str_replace") {
		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "replace invalid number of arguments: " << opNode->getID());
		}

		DepGraphNode* subjectNode = successors[2];
		DepGraphNode* patternNode = successors[0];
		DepGraphNode* replaceNode = successors[1];

		if (analysisResult.find(subjectNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, subjectNode, analysisResult);
		}
		if (analysisResult.find(patternNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, patternNode, analysisResult);
		}

		if (analysisResult.find(replaceNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, replaceNode, analysisResult);
		}

		StrangerAutomaton* subjectAuto = analysisResult[subjectNode->getID()];
		StrangerAutomaton* patternAuto = analysisResult[patternNode->getID()];
		StrangerAutomaton* replaceAuto = analysisResult[replaceNode->getID()];

		retMe = StrangerAutomaton::general_replace(patternAuto,replaceAuto,subjectAuto, opNode->getID());

	} else if (opName == "addslashes") {
		if (successors.size() != 1) {
			throw new StrangerStringAnalysisException(stringbuilder() << "addslashes should have one child: " << opNode->getID());
		}

		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		StrangerAutomaton* slashesAuto = StrangerAutomaton::addslashes(paramAuto, opNode->getID());
		retMe = slashesAuto;

	} else if (opName == "stripslashes") {
		throw new StrangerStringAnalysisException(stringbuilder() << "stripslashes is not handled yet: " << opNode->getID());

	} else if (opName == "mysql_escape_string") {
		if (successors.size() < 1 || successors.size() > 2) {
			throw new StrangerStringAnalysisException(stringbuilder() << "mysql_escape_string wrong number of arguments: " << opNode->getID());
		}
		//we only care about the first parameter
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		StrangerAutomaton* mysqlEscapeAuto = StrangerAutomaton::mysql_escape_string(paramAuto, opNode->getID());
		retMe = mysqlEscapeAuto;

	} else if (opName == "mysql_real_escape_string") {
		if (successors.size() < 1 || successors.size() > 2) {
			throw new StrangerStringAnalysisException(stringbuilder() << "mysql_real_escape_string wrong number of arguments: " << opNode->getID());
		}
		//we only care about the first parameter
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		StrangerAutomaton* mysqlEscapeAuto = StrangerAutomaton::mysql_real_escape_string(paramAuto, opNode->getID());
		retMe = mysqlEscapeAuto;

	} else if (opName == "htmlspecialchars") {
		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		string flagString = "ENT_COMPAT";
		if (successors.size() > 1) {
			if (analysisResult.find(successors[1]->getID()) == analysisResult.end()) {
				doForwardAnalysis_GeneralCase(depGraph, successors[1], analysisResult);
			}
			flagString = analysisResult[successors[1]->getID()]->getStr();
		}

		StrangerAutomaton* htmlSpecAuto = StrangerAutomaton::htmlSpecialChars(paramAuto, flagString, opNode->getID());
		retMe = htmlSpecAuto;

	} else if (opName == "nl2br"){
		if (successors.size() < 1 || successors.size() > 2) {
			throw new StrangerStringAnalysisException(stringbuilder() << "nl2br wrong number of arguments: " << opNode->getID());
		}

		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		StrangerAutomaton* nl2brAuto = StrangerAutomaton::nl2br(paramAuto, opNode->getID());
		retMe = nl2brAuto;

	}  else if (opName == "substr"){
		if (successors.size() != 3) {
			throw StrangerStringAnalysisException(stringbuilder() << "SNH: substr invalid number of arguments: " << opNode->getID());
		}

		DepGraphNode* subjectNode = successors[0];
		DepGraphNode* startNode = successors[1];
		DepGraphNode* lengthNode = successors[2];

		if (analysisResult.find(subjectNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, subjectNode, analysisResult);
		}
		if (analysisResult.find(startNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, startNode, analysisResult);
		}
		if (analysisResult.find(lengthNode->getID()) == analysisResult.end()) {
			doForwardAnalysis_GeneralCase(depGraph, lengthNode, analysisResult);
		}

		StrangerAutomaton* subjectAuto = analysisResult[subjectNode->getID()];
		string startValue = analysisResult[startNode->getID()]->getStr();
		int start = stoi(startValue);
		string lengthValue = analysisResult[lengthNode->getID()]->getStr();
		int length = stoi(lengthValue);
		StrangerAutomaton* substrAuto = subjectAuto->substr(start,length,opNode->getID());
		retMe = substrAuto;

	} else if (opName == "strtoupper" || opName == "strtolower") {
		if (successors.size() != 1) {
			throw new StrangerStringAnalysisException(stringbuilder() << opName << " has more than one successor in depgraph" );
		}

		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		if (opName == "strtoupper") {
			retMe = paramAuto->toUpperCase(opNode->getID());
		}
		else if (opName == "strtolower") {
			retMe = paramAuto->toLowerCase(opNode->getID());
		}

	} else if (opName == "trim" || opName == "rtrim" || opName == "ltrim") {
		if (successors.size() > 2) {
			throw new StrangerStringAnalysisException(stringbuilder() << opName << " has more than one successor in depgraph" );
		} else if (successors.size() == 2) {
			cout << "!!! Second parameter of " << opName << " ignored!!!. If it is not whitespace, modify implementation to handle that situation" << endl;
//			if (analysisResult.find(successors[1]->getID()) == analysisResult.end()) {
//				doForwardAnalysis_GeneralCase(depGraph, successors[1], analysisResult);
//			}
			// get trim char from automaton
		}

		StrangerAutomaton* paramAuto = analysisResult[successors[0]->getID()];
		if (opName == "trim")
			retMe = paramAuto->trimSpaces(opNode->getID());
		else if (opName == "rtrim") {
			retMe = paramAuto->trimSpacesRight(opNode->getID());
		}
		else if (opName == "ltrim") {
			retMe = paramAuto->trimSpacesLeft(opNode->getID());
		}

	} else if (opName == "md5") {
		//conservative desicion
		retMe = StrangerAutomaton::regExToAuto("/[aAbBcCdDeEfF0-9]{32,32}/",true, opNode->getID());
	} else {
		cout << "!!! Warning: Unmodeled builtin general function : " << opName << endl;
		f_unmodeled.push_back(opNode);

		 //conservative decision for operations that have not been
		 //modeled yet: .*
		retMe = StrangerAutomaton::makeAnyString(opNode->getID());
	}
	return retMe;
}

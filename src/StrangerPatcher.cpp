/*
 * StrangerPatcher.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: baki
 */

#include "StrangerPatcher.h"

PerfInfo StrangerPatcher::perfInfo;

StrangerPatcher::StrangerPatcher(string patcher_dep_graph_file_name,string patchee_dep_graph_file_name, string input_field_name) {

	this->patcher_dep_graph_file_name = patcher_dep_graph_file_name;
	this->patcher_dep_graph_file_name = patchee_dep_graph_file_name;
	this->input_field_name = input_field_name;

	// read dep graphs
	this->patcher_dep_graph = DepGraph::parseDotFile(patcher_dep_graph_file_name);
	this->patchee_dep_graph = DepGraph::parseDotFile(patchee_dep_graph_file_name);

	// initialize input nodes
	this->patcher_uninit_field_node = patcher_dep_graph.findInputNode(input_field_name);
	if (patcher_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in patcher dep graph.");
	}
	message(stringbuilder() << "patcher uninit node(" << patcher_uninit_field_node->getID() << ") found for field " << input_field_name << ".");

	this->patchee_uninit_field_node = patchee_dep_graph.findInputNode(input_field_name);
	if (patchee_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in patchee dep graph.");
	}
	message(stringbuilder() << "patchee uninit node(" << patchee_uninit_field_node->getID() << ") found for field " << input_field_name << ".");

	// initialize input relevant graphs
	this->patcher_field_relevant_graph = patcher_dep_graph.getInputRelevantGraph(patcher_uninit_field_node);
	this->patchee_field_relevant_graph = patchee_dep_graph.getInputRelevantGraph(patchee_uninit_field_node);

	// get sorted node lists for each field
	patcher_field_relevant_graph.sort(patcher_field_relevant_graph);
	this->patcher_sorted_field_relevant_nodes = patcher_field_relevant_graph.getSortedNodes();
	patchee_field_relevant_graph.sort(patchee_field_relevant_graph);
	this->patchee_sorted_field_relevant_nodes = patchee_field_relevant_graph.getSortedNodes();

	if (DEBUG_ENABLED_INIT != 0) {
		DEBUG_MESSAGE("------------ Debugging Initalization ------------");

//		DEBUG_MESSAGE("Patcher Dependency Graph");
//		this->patcher_dep_graph.toDot();
//		DEBUG_MESSAGE("Patcher Field Relevant Dependency Graph");
//		this->patcher_field_relevant_graph.toDot();

		DEBUG_MESSAGE("Patchee Dependency Graph");
		this->patchee_dep_graph.toDot();
		DEBUG_MESSAGE("Patchee Field Relevant Dependency Graph");
		this->patchee_field_relevant_graph.toDot();

	}

	ForwardImageComputer::perfInfo = &StrangerPatcher::perfInfo;
	ForwardImageComputer::staticInit();
}

StrangerPatcher::~StrangerPatcher() {
	delete this->patcher_uninit_field_node;
	delete this->patchee_uninit_field_node;
}

void StrangerPatcher::message(string msg) {
	cout << endl << "~~~~~~~~~~~>>> StrangerPatcher says: " << msg << endl;
}



void StrangerPatcher::printAnalysisResults(AnalysisResult& result) {
	cout << endl;
	for (AnalysisResultConstIterator it = result.begin(); it != result.end(); it++ ) {
		cout << "Printing automata for node ID: " << it->first << endl;
		(it->second)->toDot();
		cout << endl << endl;
	}
}

void StrangerPatcher::printNodeList(NodesList nodes) {
	cout << endl;
	for (NodesListConstIterator it = nodes.begin(); it != nodes.end(); it++ ) {
		cout << (*it)->getID() << " ";
	}
	cout << endl;
}

bool StrangerPatcher::isLengthAnIssue(StrangerAutomaton* patcherAuto, StrangerAutomaton*patcheeAuto) {
	boost::posix_time::ptime start_time = perfInfo.current_time();
	bool result = false;
	if(patcherAuto->isLengthFinite()) {
		if (patcheeAuto->isLengthFinite()) {
			if (patcherAuto->getMaxLength() < patcheeAuto->getMaxLength()) {
				result = true;
			}
		}
		else {
			result = true;
		}
	}
	perfInfo.sanitization_length_issue_check_time = perfInfo.current_time() - start_time;
	return result;
}

/**
 * Initial backward analysis phase for extracting validation behavior
 */
StrangerAutomaton* StrangerPatcher::extractValidationPatch() {

	message("BEGIN VALIDATION EXTRACTION PHASE........................................");

	ForwardImageComputer analyzer;
	boost::posix_time::ptime start_time;
	try {
		message("extracting validation from patcher");
		start_time = perfInfo.current_time();
		AnalysisResult patcher_validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationPhase(patcher_dep_graph, patcher_field_relevant_graph, patcher_sorted_field_relevant_nodes);
		StrangerAutomaton* patcher_negVPatch = patcher_validationExtractionResults[patcher_uninit_field_node->getID()];
		StrangerAutomaton* patcher_validation = patcher_negVPatch->complement(patcher_uninit_field_node->getID());
		perfInfo.validation_patcher_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("patcher validation auto:");
			DEBUG_AUTO(patcher_validation);
		}

		message("extracting validation from patchee");
		start_time = perfInfo.current_time();
		AnalysisResult patchee_validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes);
		StrangerAutomaton* patchee_negVPatch = patchee_validationExtractionResults[patchee_uninit_field_node->getID()];
		StrangerAutomaton* patchee_validation = patchee_negVPatch->complement(patchee_uninit_field_node->getID());
		perfInfo.validation_patchee_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("patchee validation auto:");
			DEBUG_AUTO(patchee_validation);
		}

		message("computing validation patch");
		start_time = perfInfo.current_time();
		StrangerAutomaton* diffAuto = patchee_validation->difference(patcher_validation, patchee_uninit_field_node->getID());
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("difference auto:");
			DEBUG_AUTO(diffAuto);
		}

		StrangerAutomaton* interAuto = patchee_validation->intersect(patcher_validation, patchee_uninit_field_node->getID());
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("intersection auto:");
			DEBUG_AUTO(interAuto);
		}
		if (diffAuto->isEmpty()) {
			message("difference auto is empty, no validation patch is required!!!");
			is_validation_patch_required = false;
			validation_patch_auto_1 = StrangerAutomaton::makeAnyString(patchee_uninit_field_node->getID());
			validation_patch_auto = validation_patch_auto_1;
			delete interAuto;

		} else if (interAuto->isEmpty()) {
			message("intersection auto is empty; client and server accepts different sets, validation patch rejects everything!");
			is_validation_patch_required = true;
			validation_patch_auto_1 = interAuto->clone(-11);
			validation_patch_auto = interAuto;
			if (DEBUG_ENABLED_VP != 0) {
				DEBUG_MESSAGE("validation patch is intersection auto...");
			}
		} else {
			message("validation patch is generated for input: " + input_field_name);
			is_validation_patch_required = true;
			validation_patch_auto_1 = interAuto->clone(-11);
			validation_patch_auto = interAuto;
			if (DEBUG_ENABLED_VP != 0) {
				DEBUG_MESSAGE("validation patch is intersection auto...");
			}
		}
		perfInfo.validation_comparison_time = perfInfo.current_time() - start_time;

		delete patcher_validation;
		delete patchee_validation;
		delete diffAuto;

	} catch (StrangerStringAnalysisException const &e) {
		cerr << e.what();
		exit(EXIT_FAILURE);
	}

	perfInfo.calculate_total_validation_extraction_time();
	message("........................................END VALIDATION EXTRACTION PHASE");
	return validation_patch_auto;
}

/**
 * Computes sink post image for patcher
 */
StrangerAutomaton* StrangerPatcher::computePatcherFWAnalysis() {
	message("computing patcher sink post image...");
	AnalysisResult patcherAnalysisResult;
	UninitNodesList patcherUninitNodes = patcher_dep_graph.getUninitNodes();

	// initialize patcher input nodes to bottom
	message("initializing patcher inputs with bottom");
	for (UninitNodesListConstIterator it = patcherUninitNodes.begin(); it != patcherUninitNodes.end(); it++) {
		patcherAnalysisResult[(*it)->getID()] = StrangerAutomaton::makePhi((*it)->getID());
	}
	// initialize uninit node that we are interested in with sigma star
	message(stringbuilder() << "initializing input node(" << patcher_uninit_field_node->getID() << ") with sigma star");
	delete patcherAnalysisResult[patcher_uninit_field_node->getID()];
	patcherAnalysisResult[patcher_uninit_field_node->getID()] = StrangerAutomaton::makeAnyString(patcher_uninit_field_node->getID());

	ForwardImageComputer patcherAnalyzer;

	try {
		message("starting forward analysis for patcher...");
		patcherAnalyzer.doForwardAnalysis_RegularPhase(patcher_dep_graph,patcher_field_relevant_graph,patcher_sorted_field_relevant_nodes,patcherAnalysisResult);
		message("...finished forward analysis for patcher.");

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }

	patcher_sink_auto = patcherAnalysisResult[patcher_field_relevant_graph.getRoot()->getID()];
	message("...computed patcher sink post image.");
	return patcher_sink_auto;
}

/**
 * Computes sink post image for patchee, first time
 */
AnalysisResult StrangerPatcher::computePatcheeFWAnalysis() {
	AnalysisResult patcheeAnalysisResult;
	UninitNodesList patcheeUninitNodes = patchee_dep_graph.getUninitNodes();

	message("initializing patchee inputs with bottom");
	for (UninitNodesListConstIterator it = patcheeUninitNodes.begin(); it != patcheeUninitNodes.end(); it++) {
		patcheeAnalysisResult[(*it)->getID()] = StrangerAutomaton::makePhi((*it)->getID());
	}

	// initialize uninit node that we are interested in with validation patch_auto
	message(stringbuilder() << "initializing input node(" << patchee_uninit_field_node->getID() << ") with validation patch auto");
	delete patcheeAnalysisResult[patchee_uninit_field_node->getID()];
	patcheeAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto;

	ForwardImageComputer patcheeAnalyzer;

	try {

		message("starting forward analysis for patchee");
		patcheeAnalyzer.doForwardAnalysis_RegularPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes, patcheeAnalysisResult);
		message("...finished forward analysis for patchee.");

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }

	return patcheeAnalysisResult;
}

// TODO modify length patch function
AnalysisResult StrangerPatcher::computePatcheeLengthPatch(StrangerAutomaton* initialAuto, AnalysisResult& fwAnalysisResult) {
	message("starting a backward analysis to calculate length patch...");
	AnalysisResult patcheeAnalysisResult;
	UninitNodesList patcheeUninitNodes = patchee_dep_graph.getUninitNodes();

	//TODO discuss initialization here,

	for (UninitNodesListConstIterator it = patcheeUninitNodes.begin(); it != patcheeUninitNodes.end(); it++) {
		patcheeAnalysisResult[(*it)->getID()] = StrangerAutomaton::makePhi((*it)->getID());
	}

	ForwardImageComputer patcheeAnalyzer;

	try {

		message("starting backward analysis to patch length constraints...");

		fwAnalysisResult[patchee_uninit_field_node->getID()] = StrangerAutomaton::makeAnyString(-5);

		boost::posix_time::ptime start_time = perfInfo.current_time();
		AnalysisResult bwResult = patcheeAnalyzer.doBackwardAnalysis_RegularPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes,initialAuto, fwAnalysisResult);
		perfInfo.sanitization_length_backward_time = perfInfo.current_time() - start_time;

		StrangerAutomaton* negPatchAuto = bwResult[patchee_uninit_field_node->getID()];
		validation_patch_auto_2 = negPatchAuto->complement(-5);

		if (DEBUG_ENABLED_LENB != 0) {
			DEBUG_MESSAGE("Length issue patch:");
			DEBUG_AUTO(validation_patch_auto_2);
		}

		fwAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto;


	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }
	message("...length patch is generated");
	return patcheeAnalysisResult;
}

StrangerAutomaton* StrangerPatcher::computePatcheeSanitizationPatch(StrangerAutomaton* initialAuto,	const AnalysisResult& fwAnalysisResult) {
	ForwardImageComputer patcheeAnalyzer;
	AnalysisResult bwResult = patcheeAnalyzer.doBackwardAnalysis_RegularPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes,initialAuto, fwAnalysisResult);
	sanitization_patch_auto = bwResult[patchee_uninit_field_node->getID()];
	return sanitization_patch_auto;
}


/**
 *
 */
StrangerAutomaton* StrangerPatcher::extractSanitizationPatch() {

	boost::posix_time::ptime start_time = perfInfo.current_time();
	StrangerAutomaton* patcherSinkAuto = computePatcherFWAnalysis();
	perfInfo.sanitization_patcher_first_forward_time = perfInfo.current_time() - start_time;
	if (DEBUG_ENABLED_1F != 0) {
		DEBUG_MESSAGE("Patcher Sink Auto - First forward analysis");
		DEBUG_AUTO(patcherSinkAuto);
	}
	start_time = perfInfo.current_time();
	AnalysisResult patcheeAnalysisResult = computePatcheeFWAnalysis();
	StrangerAutomaton* patcheeSinkAuto = patcheeAnalysisResult[patchee_field_relevant_graph.getRoot()->getID()];
	perfInfo.sanitization_patchee_first_forward_time = perfInfo.current_time() - start_time;
	if (DEBUG_ENABLED_1F != 0) {
		DEBUG_MESSAGE("Patchee Sink Auto - First forward analysis");
		DEBUG_AUTO(patcheeSinkAuto);
	}

	message("checking difference between patcher and patchee");
	boost::posix_time::ptime comp_time = perfInfo.current_time();
	StrangerAutomaton* differenceAuto = patcheeSinkAuto->difference(patcherSinkAuto, -3);
	bool isDifferenceAutoEmpty = differenceAuto->isEmpty();
	perfInfo.sanitization_comparison_time = perfInfo.current_time() - comp_time;
	if (DEBUG_ENABLED_1F != 0) {
		DEBUG_MESSAGE("Difference auto after first forward analysis:");
		DEBUG_AUTO(differenceAuto);
	}

	if (isDifferenceAutoEmpty) {
		message("no difference, no sanitization patch required!");
		delete differenceAuto;
		sanitization_patch_auto = NULL;
		is_sanitization_patch_required = false;
	} else if(isLengthAnIssue(patcherSinkAuto,patcheeSinkAuto)) {
		message("length constraints contribute to the difference, fixing issue...");
		start_time = perfInfo.current_time();
		StrangerAutomaton* lengthRestrictAuto =
				patcheeSinkAuto->restrictLengthByOtherAutomatonFinite(patcherSinkAuto, -4);
		perfInfo.sanitization_length_issue_check_time += perfInfo.current_time() - start_time; // adding to length issue check in if statements
		if (DEBUG_ENABLED_LENB != 0) {
			DEBUG_MESSAGE("Length restricted patchee sink automaton:");
			DEBUG_AUTO(lengthRestrictAuto);
		}

		try {
			// use negation of length restricted auto to solve the problem of overapproximation
			StrangerAutomaton* negLengthRestrictAuto = lengthRestrictAuto->complement(-4);
			AnalysisResult patcheeAnalysisResult_2 = computePatcheeLengthPatch(negLengthRestrictAuto, patcheeAnalysisResult);

			patcheeSinkAuto = patcheeAnalysisResult_2[patchee_field_relevant_graph.getRoot()->getID()];

			if (DEBUG_ENABLED_LENF != 0) {
				DEBUG_MESSAGE("Patchee sink auto with length patch update");
				DEBUG_AUTO(patcheeSinkAuto);
			}
			message("checking difference between patcher and patchee second time");
			delete differenceAuto;
			comp_time = perfInfo.current_time();
			differenceAuto = patcheeSinkAuto->difference(patcherSinkAuto, -3);
			isDifferenceAutoEmpty = differenceAuto->isEmpty();
			perfInfo.sanitization_comparison_time += perfInfo.current_time() - comp_time; // added to older comparison time
			if (DEBUG_ENABLED_LENF != 0) {
				DEBUG_MESSAGE("Difference auto after length patch");
				DEBUG_AUTO(differenceAuto);
			}
			if (isDifferenceAutoEmpty){
				// TODO length patch is still necessary for our goal, justification
				message("no difference, no sanitization patch required 2!");
				delete differenceAuto;
				sanitization_patch_auto = NULL;
				is_sanitization_patch_required = false;
				is_validation_patch_for_length_issue_required = false;
			}
			else {
				message("starting last backward analysis for sanitization patch with diff auto(length constraint included)...");
				validation_patch_auto = validation_patch_auto_2;
				start_time = perfInfo.current_time();
				sanitization_patch_auto = computePatcheeSanitizationPatch(differenceAuto, patcheeAnalysisResult_2);
				perfInfo.sanitization_last_backward_time = perfInfo.current_time() - start_time;
				if (DEBUG_ENABLED_LASTB != 0) {
					DEBUG_MESSAGE("Sanitization patch auto (length fix applied)");
					DEBUG_AUTO(sanitization_patch_auto);
				}
				message("...finished last backward analysis for sanitization patch with diff auto(length constraint included).");
				is_sanitization_patch_required = true;
				is_validation_patch_for_length_issue_required = true;
			}

		} catch (StrangerStringAnalysisException const &e) {
			cerr << e.what();
			exit(EXIT_FAILURE);
		}

	} else {
		message("starting last backward analysis for sanitization patch with diff auto...");
		start_time = perfInfo.current_time();
		sanitization_patch_auto = computePatcheeSanitizationPatch(differenceAuto, patcheeAnalysisResult);
		perfInfo.sanitization_last_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_LASTB != 0) {
			DEBUG_MESSAGE("Sanitization patch auto");
			DEBUG_AUTO(sanitization_patch_auto);
		}
		message("...finished last backward analysis for sanitization patch with diff auto.");
		is_sanitization_patch_required = true;
		is_validation_patch_for_length_issue_required = false;
	}
	perfInfo.calculate_total_sanitization_extraction_time();
	return sanitization_patch_auto;
}



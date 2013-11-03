/*
 * StrangerPatcher.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: baki
 */

#include "StrangerPatcher.h"

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
	message(stringbuilder() << "patcher uninit node(" << patcher_uninit_field_node->getID() << ") found.");
	this->patchee_uninit_field_node = patchee_dep_graph.findInputNode(input_field_name);
	if (patchee_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in patchee dep graph.");
	}
	message(stringbuilder() << "patchee uninit node(" << patchee_uninit_field_node->getID() << ") found.");

	// initialize input relevant graphs
	this->patcher_field_relevant_graph = patcher_dep_graph.getInputRelevantGraph(patcher_uninit_field_node);
	this->patchee_field_relevant_graph = patchee_dep_graph.getInputRelevantGraph(patchee_uninit_field_node);

	// get sorted node lists for each field
	patcher_field_relevant_graph.sort(patcher_field_relevant_graph);
	this->patcher_sorted_field_relevant_nodes = patcher_field_relevant_graph.getSortedNodes();
	patchee_field_relevant_graph.sort(patchee_field_relevant_graph);
	this->patchee_sorted_field_relevant_nodes = patchee_field_relevant_graph.getSortedNodes();
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

/**
 * TODO extract both validations and generate better patch
 * Initial backward analysis phase for extracting validation behavior
 */
StrangerAutomaton* StrangerPatcher::extractValidationPatch() {

		ForwardImageComputer::staticInit();
		ForwardImageComputer analyzer;

		try {
			AnalysisResult validationExtractionResults =
					analyzer.doBackwardAnalysis_ValidationPhase(patcher_dep_graph, patcher_field_relevant_graph, patcher_sorted_field_relevant_nodes);

			StrangerAutomaton* negVPatch = validationExtractionResults[patcher_uninit_field_node->getID()];
					validation_patch_auto = negVPatch->complement(patcher_uninit_field_node->getID());

		} catch (StrangerStringAnalysisException const &e) {
	        cerr << e.what();
	        exit(EXIT_FAILURE);
	    }

		message("validation patch is generated for input: " + input_field_name);
//		validation_patch_auto->toDotAscii(0);
		return validation_patch_auto;

}

/**
 * Computes sink post image for patcher
 */
StrangerAutomaton* StrangerPatcher::computePatcherFWAnalysis() {
	message("computing patcher sink post image...");
	AnalysisResult patcherAnalysisResult;
	UninitNodesList patcherUninitNodes = patcher_dep_graph.getUninitNodes();

	// initialize patcher input nodes to sigma star
	message("initializing patcher inputs with sigma star");
	for (UninitNodesListConstIterator it = patcherUninitNodes.begin(); it != patcherUninitNodes.end(); it++) {
		patcherAnalysisResult[(*it)->getID()] = StrangerAutomaton::makeAnyString((*it)->getID());
	}

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
}

/**
 * TODO refoctor for less code, easy to handle code
 */
StrangerAutomaton* StrangerPatcher::checkSanitizationDifference() {


	AnalysisResult patcheeAnalysisResult;
	AnalysisResult patcheeAnalysisResult2;


	UninitNodesList patcheeUninitNodes = patchee_dep_graph.getUninitNodes();


	//TODO discuss initialization here,
	message("initializing patchee inputs with bottom");
	for (UninitNodesListConstIterator it = patcheeUninitNodes.begin(); it != patcheeUninitNodes.end(); it++) {
		patcheeAnalysisResult[(*it)->getID()] = StrangerAutomaton::makePhi((*it)->getID());
		patcheeAnalysisResult2[(*it)->getID()] = StrangerAutomaton::makePhi((*it)->getID());
	}

	// initialize uninit node that we are interested in with validation patch_auto
	message(stringbuilder() << "initializing input node(" << patchee_uninit_field_node->getID() << ") with validation patch auto");
	delete patcheeAnalysisResult[patchee_uninit_field_node->getID()];
	patcheeAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto;


//	ForwardImageComputer::staticInit();
	ForwardImageComputer patcheeAnalyzer;

	try {

		message("starting forward analysis for patchee");
		patcheeAnalyzer.doForwardAnalysis_RegularPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes, patcheeAnalysisResult);
		message("...finished forward analysis for patchee.");

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }


	StrangerAutomaton* patcheeSinkAuto = patcheeAnalysisResult[patchee_field_relevant_graph.getRoot()->getID()];

	message("checking difference between patcher and patchee");
	StrangerAutomaton* complementAuto = patcherSinkAuto->complement(-3);
	StrangerAutomaton* differenceAuto = patcheeSinkAuto->intersect(complementAuto, -3);
	delete complementAuto;

	if (differenceAuto->isEmpty() ){
		message("no difference, no patch required!");
		delete differenceAuto;
		differenceAuto = NULL;
	} else if(patcherSinkAuto->isLengthFinite()){
		message("length constraints contribute to the difference, fixing issue...");
		StrangerAutomaton* lengthRestrictAuto =
				patcheeSinkAuto->restrictLengthByOtherAutomatonFinite(patcherSinkAuto, -4);
//		cout << endl << endl;
//		patcheeSinkAuto->toDotAscii(0);
//
//		cout << endl << endl;
//		lengthRestrictAuto->toDotAscii(0);
		// do backward and forward again and check difference again

		//TODO implementing here
		try {
			message("starting backward analysis to patch length constraints...");
			AnalysisResult bwResult = patcheeAnalyzer.doBackwardAnalysis_RegularPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes,lengthRestrictAuto, patcheeAnalysisResult);
			delete validation_patch_auto;
			validation_patch_auto = bwResult[patchee_uninit_field_node->getID()];
			patcheeAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto;
			message("...validation patch is updated using length constraints");

			message("second forward analysis begins for patchee...");
			delete patcheeAnalysisResult2[patchee_uninit_field_node->getID()];
			patcheeAnalysisResult2[patchee_uninit_field_node->getID()] = validation_patch_auto;
			patcheeAnalyzer.doForwardAnalysis_RegularPhase(patchee_dep_graph, patchee_field_relevant_graph, patchee_sorted_field_relevant_nodes, patcheeAnalysisResult2);
			message("...second forward analysis ends for pathcee.");

			patcheeSinkAuto = patcheeAnalysisResult2[patchee_field_relevant_graph.getRoot()->getID()];

			message("checking difference between patcher and patchee");
			complementAuto = patcherSinkAuto->complement(-3);
			delete differenceAuto;
			differenceAuto = patcheeSinkAuto->intersect(complementAuto, -3);
			delete complementAuto;

			if (differenceAuto->isEmpty() ){
				message("no difference, no patch required!");
				delete differenceAuto;
				differenceAuto = NULL;
			}
			else {
				message("difference auto is computed with additional phases for length");
			}

		} catch (StrangerStringAnalysisException const &e) {
			cerr << e.what();
			exit(EXIT_FAILURE);
		}

	} else {
		message("difference auto is computed");
	}
	return differenceAuto;
}

/**
 *
 */
StrangerAutomaton* StrangerPatcher::extractSanitizationPatch(StrangerAutomaton* differenceAuto, const AnalysisResult& fwAnalysisResult) {
	return NULL;
}

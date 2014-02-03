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

	ImageComputer::perfInfo = &StrangerPatcher::perfInfo;
	ImageComputer::staticInit();
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

void StrangerPatcher::printResults() {
	if (is_validation_patch_required) {
		cout << "\t    - validation patch is generated" << endl;
		cout << "\t size : states " << validation_patch_auto->get_num_of_states() << " : "
				<< "bddnodes " << validation_patch_auto->get_num_of_bdd_nodes() << endl;
		if (DEBUG_ENABLED_RESULTS != 0) {
			DEBUG_MESSAGE("validation patch auto:");
			DEBUG_AUTO(validation_patch_auto);
		}
	} else {
		cout << "\t    - no validation patch" << endl;
		cout << "\t size : states 0 : bddnodes 0" << endl;
	}


	if (is_length_patch_required) {
		cout << "\t    - length patch is generated" << endl;
		cout << "\t size : states " << length_patch_auto->get_num_of_states() << " : "
						<< "bddnodes " << length_patch_auto->get_num_of_bdd_nodes() << endl;
		if (DEBUG_ENABLED_RESULTS != 0) {
			DEBUG_MESSAGE("length patch auto:");
			DEBUG_AUTO(length_patch_auto);
		}
	} else {
		cout << "\t    - no length patch" << endl;
		cout << "\t size : states 0 : bddnodes 0" << endl;
	}

	if (is_sanitization_patch_required) {
		cout << "\t    - sanitization patch is generated :" << endl;
		cout << "\t size : states " << sanitization_patch_auto->get_num_of_states() << " : "
				<< "bddnodes " << sanitization_patch_auto->get_num_of_bdd_nodes() << endl;
		if (DEBUG_ENABLED_RESULTS != 0) {
			DEBUG_MESSAGE("sanitization patch auto:");
			DEBUG_AUTO(sanitization_patch_auto);
		}
	} else {
		cout << "\t    - no sanitization patch" << endl;
		cout << "\t size : states 0 : bddnodes 0" << endl;
	}

	perfInfo.print_validation_extraction_info();
	perfInfo.print_sanitization_extraction_info();
	perfInfo.print_operations_info();
	perfInfo.reset();
}
// TODO parameterize paths
void StrangerPatcher::writeAutosforCodeGeneration(string field_name, string patcherName, string patcheeName) {
	string patcher_name = patcherName.substr( patcherName.find_last_of('/') + 1,patcherName.find_last_of('.') - patcherName.find_last_of('/') - 1 );
	string patchee_name = patcheeName.substr( patcheeName.find_last_of('/') + 1,patcheeName.find_last_of('.') - patcheeName.find_last_of('/') - 1 );
	string type = "accept";
	if (calculate_rejected_set) {
		type = "reject";
	}
	if (is_validation_patch_required) {
		string v_patch_auto_path = stringbuilder() << "/home/abaki/RA/PLDI/PLDI14/experiments/patches/" << field_name << "_" << patcher_name << "_" << patchee_name <<
				"_VP_" << type << "_auto.dot";
		validation_patch_auto->toDotFile(v_patch_auto_path);
		message(stringbuilder() << "Validation Patch auto is written... : " << v_patch_auto_path);
	}
	if (is_length_patch_required) {
		string l_patch_auto_path = stringbuilder() << "/home/abaki/RA/PLDI/PLDI14/experiments/patches/" << patcher_name << "_" << patchee_name <<
						"_LP_" << type << "_auto.dot";
		length_patch_auto->toDotFile(l_patch_auto_path);
		message(stringbuilder() << "Length Patch auto is written... : " << l_patch_auto_path);
	}
	if (is_sanitization_patch_required) {
		string path = "/home/abaki/RA/PLDI/PLDI14/experiments/mincutresults/";

		string ref_auto_name = path + "references/" + field_name + "_" + patcher_name + "_" + patchee_name + "_auto.dot";
		string patch_auto_name = path + "patches/" + field_name + "_" + patcher_name + "_" + patchee_name + "_auto.dot";
		patcher_sink_auto->toDotFile(ref_auto_name);
		sanitization_patch_auto->toDotFile(patch_auto_name);
		message(stringbuilder() << "Sanitization Patch autos are written for mincut... : " << "ref auto: " << ref_auto_name << ", patch auto: " << patch_auto_name);
		cout << endl << "\t Automata for Mincut are written:" << endl;
		cout << "\t Reference auto : " << ref_auto_name << endl;
		cout << "\t Patch     auto : " << patch_auto_name << endl;
	}
}

/**
 * checks if length has maximum restriction, or minimum restriction without a maximium restriction
 * TODO currently that function only checks if the minimum restriction length is 1 or not, handle any minimum restriction later
 * result 0 : no worries about length
 * result 1 : there is a maximum length restriction (may also have minimum inside)
 * result 2 : there is a minimum length restriction (max length is infinite in that case)
 */
int StrangerPatcher::isLengthAnIssue(StrangerAutomaton* patcherAuto, StrangerAutomaton*patcheeAuto) {
	message("BEGIN LENGTH PATCH ANALYSIS PHASE........................................");
	boost::posix_time::ptime start_time = perfInfo.current_time();
	int result = 0;
	if(patcherAuto->isLengthFinite()) {
		if (patcheeAuto->isLengthFinite()) {
			if (patcherAuto->getMaxLength() < patcheeAuto->getMaxLength()) {
				result = 1;
			}
		}
		else {
			result = 1;
		}
	} else if (patcherAuto->checkEmptyString()) {
		if( !patcheeAuto->checkEmptyString() ) {
			result = 2;
		}
	}
	perfInfo.sanitization_length_issue_check_time = perfInfo.current_time() - start_time;
	return result;
}

/**
 * Initial backward analysis phase for extracting validation behavior
 */
StrangerAutomaton* StrangerPatcher::computeValidationPatch() {

	message("BEGIN VALIDATION ANALYSIS PHASE........................................");

	ImageComputer analyzer;
	boost::posix_time::ptime start_time;
	try {
		message("extracting validation from patcher");
		start_time = perfInfo.current_time();
		AnalysisResult patcher_validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationCase(patcher_dep_graph, patcher_field_relevant_graph, StrangerAutomaton::makeBottom());
		StrangerAutomaton* patcher_negVPatch = patcher_validationExtractionResults[patcher_uninit_field_node->getID()];
		StrangerAutomaton* patcher_validation = patcher_negVPatch;
		if ( !calculate_rejected_set ) {
			patcher_validation = patcher_negVPatch->complement(patcher_uninit_field_node->getID());
		}

		perfInfo.validation_patcher_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("patcher validation auto:");
			DEBUG_AUTO(patcher_validation);
		}

		message("extracting validation from patchee");
		start_time = perfInfo.current_time();
		AnalysisResult patchee_validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationCase(patchee_dep_graph, patchee_field_relevant_graph, StrangerAutomaton::makeBottom());
		StrangerAutomaton* patchee_negVPatch = patchee_validationExtractionResults[patchee_uninit_field_node->getID()];
		StrangerAutomaton* patchee_validation = patchee_negVPatch;
		if ( !calculate_rejected_set ) {
			patchee_validation = patchee_negVPatch->complement(patchee_uninit_field_node->getID());
		}
		perfInfo.validation_patchee_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("patchee validation auto:");
			DEBUG_AUTO(patchee_validation);
		}

		message("computing validation patch");
		start_time = perfInfo.current_time();
		StrangerAutomaton* diffAuto = NULL;
		if (calculate_rejected_set)  {
			diffAuto = patcher_validation->difference(patchee_validation, patchee_uninit_field_node->getID());
		}
		else {
			diffAuto = patchee_validation->difference(patcher_validation, patchee_uninit_field_node->getID());
		}
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("difference auto:");
			DEBUG_AUTO(diffAuto);
		}
		if (calculate_rejected_set) {
			if (diffAuto->isEmpty()) {
				message("difference auto is empty, no validation patch is required!!!");
				is_validation_patch_required = false;
				validation_patch_auto = StrangerAutomaton::makePhi(patchee_uninit_field_node->getID());

			} else {
				message("validation patch is generated for input: " + input_field_name);
				is_validation_patch_required = true;
				validation_patch_auto = diffAuto->clone(patchee_uninit_field_node->getID());
				if (DEBUG_ENABLED_VP != 0) {
					DEBUG_MESSAGE("validation patch is difference auto...");
				}
			}
		}
		else {
			StrangerAutomaton* interAuto = patchee_validation->intersect(patcher_validation, patchee_uninit_field_node->getID());
			if (DEBUG_ENABLED_VP != 0) {
				DEBUG_MESSAGE("intersection auto:");
				DEBUG_AUTO(interAuto);
			}
			if (diffAuto->isEmpty()) {
				message("difference auto is empty, no validation patch is required!!!");
				is_validation_patch_required = false;
				validation_patch_auto = StrangerAutomaton::makeAnyString(patchee_uninit_field_node->getID());
//				validation_patch_auto = patchee_validation;
				delete interAuto;

			} else if (interAuto->isEmpty()) {
				message("intersection auto is empty; client and server accepts different sets, validation patch rejects everything!");
				is_validation_patch_required = true;
				validation_patch_auto = interAuto->clone(-11);
				if (DEBUG_ENABLED_VP != 0) {
					DEBUG_MESSAGE("validation patch is intersection auto...");
				}
			} else {
				message("validation patch is generated for input: " + input_field_name);
				is_validation_patch_required = true;
				validation_patch_auto = interAuto->clone(-11);
				if (DEBUG_ENABLED_VP != 0) {
					DEBUG_MESSAGE("validation patch is intersection auto...");
				}
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
	message("........................................END VALIDATION ANALYSIS PHASE");
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

	ImageComputer patcherAnalyzer;

	try {
		message("starting forward analysis for patcher...");
		patcherAnalyzer.doForwardAnalysis_SingleInput(patcher_dep_graph, patcher_field_relevant_graph, patcherAnalysisResult);
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
	if (calculate_rejected_set) {
		patcheeAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto->complement(patchee_uninit_field_node->getID());
	} else {
		patcheeAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto;
	}


	ImageComputer patcheeAnalyzer;

	try {

		message("starting forward analysis for patchee");
		patcheeAnalyzer.doForwardAnalysis_SingleInput(patchee_dep_graph, patchee_field_relevant_graph, patcheeAnalysisResult);
		message("...finished forward analysis for patchee.");

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }

	return patcheeAnalysisResult;
}

StrangerAutomaton* StrangerPatcher::computePatcheeLengthPatch(StrangerAutomaton* initialAuto, AnalysisResult& fwAnalysisResult) {
	message("starting a backward analysis to calculate length patch...");
	ImageComputer patcheeAnalyzer;
	try {
		fwAnalysisResult[patchee_uninit_field_node->getID()] = StrangerAutomaton::makeAnyString(-5);
		boost::posix_time::ptime start_time = perfInfo.current_time();
		AnalysisResult bwResult = patcheeAnalyzer.doBackwardAnalysis_GeneralCase(patchee_dep_graph, patchee_field_relevant_graph, initialAuto, fwAnalysisResult);
		perfInfo.sanitization_length_backward_time = perfInfo.current_time() - start_time;
		StrangerAutomaton* negPatchAuto = bwResult[patchee_uninit_field_node->getID()];
		if ( calculate_rejected_set ) {
			length_patch_auto = negPatchAuto->clone(-5);
		} else {
			length_patch_auto = negPatchAuto->complement(-5);
		}

		if (DEBUG_ENABLED_LP != 0) {
			DEBUG_MESSAGE("Length patch:");
			DEBUG_AUTO(length_patch_auto);
		}

//		fwAnalysisResult[patchee_uninit_field_node->getID()] = validation_patch_auto->intersect(length_patch_auto,-5);

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }
	message("...length patch is generated");
	return length_patch_auto;
}

StrangerAutomaton* StrangerPatcher::computePatcheeSanitizationPatch(StrangerAutomaton* initialAuto,	const AnalysisResult& fwAnalysisResult) {
	ImageComputer patcheeAnalyzer;
	AnalysisResult bwResult = patcheeAnalyzer.doBackwardAnalysis_GeneralCase(patchee_dep_graph, patchee_field_relevant_graph, initialAuto, fwAnalysisResult);
	sanitization_patch_auto = bwResult[patchee_uninit_field_node->getID()];
	return sanitization_patch_auto;
}


/**
 *
 */
StrangerAutomaton* StrangerPatcher::computeSanitizationPatch() {
	message("BEGIN SANITIZATION ANALYSIS PHASE........................................");
	boost::posix_time::ptime start_time = perfInfo.current_time();
	patcher_sink_auto = computePatcherFWAnalysis();
	perfInfo.sanitization_patcher_first_forward_time = perfInfo.current_time() - start_time;
	if (DEBUG_ENABLED_SC != 0) {
		DEBUG_MESSAGE("Patcher Sink Auto - First forward analysis");
		DEBUG_AUTO(patcher_sink_auto);
	}
	start_time = perfInfo.current_time();
	AnalysisResult patcheeAnalysisResult = computePatcheeFWAnalysis();
	StrangerAutomaton* patcheeSinkAuto = patcheeAnalysisResult[patchee_field_relevant_graph.getRoot()->getID()];
	perfInfo.sanitization_patchee_first_forward_time = perfInfo.current_time() - start_time;
	if (DEBUG_ENABLED_SC != 0) {
		DEBUG_MESSAGE("Patchee Sink Auto - First forward analysis");
		DEBUG_AUTO(patcheeSinkAuto);
	}

	message("checking difference between patcher and patchee");
	boost::posix_time::ptime comp_time = perfInfo.current_time();
	StrangerAutomaton* differenceAuto = patcheeSinkAuto->difference(patcher_sink_auto, -3);
	bool isDifferenceAutoEmpty = differenceAuto->isEmpty();
	perfInfo.sanitization_comparison_time = perfInfo.current_time() - comp_time;
	if (DEBUG_ENABLED_SC != 0) {
		DEBUG_MESSAGE("Difference auto after first forward analysis:");
		DEBUG_AUTO(differenceAuto);
	}
	int length_check_result = 0;
	if (isDifferenceAutoEmpty) {
		message("no difference, no sanitization patch required!");
		delete differenceAuto;
		sanitization_patch_auto = NULL;
		length_patch_auto = NULL;
		is_sanitization_patch_required = false;
		is_length_patch_required = false;
	} else if( (length_check_result = isLengthAnIssue(patcher_sink_auto,patcheeSinkAuto)) != 0) {
		message("length constraints contribute to the difference, fixing issue...");
		StrangerAutomaton* lengthRestrictAuto = NULL;
		if (length_check_result == 1) {
			start_time = perfInfo.current_time();
			lengthRestrictAuto = patcheeSinkAuto->restrictLengthByOtherAutomatonFinite(patcher_sink_auto, -4);
			perfInfo.sanitization_length_issue_check_time += perfInfo.current_time() - start_time; // adding to length issue check in if statements
		} else if (length_check_result == 2) {
			start_time = perfInfo.current_time();
			StrangerAutomaton* emptyAuto = StrangerAutomaton::makeEmptyString(-4);
			StrangerAutomaton* negEmptyAuto = emptyAuto->complement(-4);
			lengthRestrictAuto = patcheeSinkAuto->intersect(negEmptyAuto);
			delete emptyAuto;
			delete negEmptyAuto;
			perfInfo.sanitization_length_issue_check_time += perfInfo.current_time() - start_time;
		}

		if (DEBUG_ENABLED_LP != 0) {
			DEBUG_MESSAGE("Length restricted patchee sink automaton:");
			DEBUG_AUTO(lengthRestrictAuto);
		}

		try {
			// use negation of length restricted auto to solve the problem of overapproximation
			StrangerAutomaton* negLengthRestrictAuto = lengthRestrictAuto->complement(-4);
			StrangerAutomaton* rejectedLengthAuto = patcheeSinkAuto->intersect(negLengthRestrictAuto, -4);
			delete negLengthRestrictAuto;
			computePatcheeLengthPatch(rejectedLengthAuto, patcheeAnalysisResult);

			if (DEBUG_ENABLED_LP != 0) {
				DEBUG_MESSAGE("Length patch automaton");
				DEBUG_AUTO(length_patch_auto);
			}
			message("........................................END LENGTH_PATCH ANALYSIS PHASE");
			message("CONTINUE SANITIZATION ANALYSIS PHASE........................................");
			message("checking difference between patcher and patchee after length restriction");
			delete differenceAuto;
			comp_time = perfInfo.current_time();
			differenceAuto = lengthRestrictAuto->difference(patcher_sink_auto, -3);
			bool isDifferenceAutoEmpty = differenceAuto->isEmpty();
			perfInfo.sanitization_comparison_time += perfInfo.current_time() - comp_time;
			if (DEBUG_ENABLED_SP != 0) {
				DEBUG_MESSAGE("Difference auto after length restriction");
				DEBUG_AUTO(differenceAuto);
			}

			if (isDifferenceAutoEmpty) {
				message("no difference, no sanitization patch required!");
				delete differenceAuto;
				sanitization_patch_auto = NULL;
				is_sanitization_patch_required = false;
				is_length_patch_required = true;
			} else {
				message("starting last backward analysis for sanitization patch with diff auto after length restriction...");
				start_time = perfInfo.current_time();
				sanitization_patch_auto = computePatcheeSanitizationPatch(differenceAuto, patcheeAnalysisResult);
				perfInfo.sanitization_patch_backward_time = perfInfo.current_time() - start_time;
				if (DEBUG_ENABLED_SP != 0) {
					DEBUG_MESSAGE("Sanitization patch auto");
					DEBUG_AUTO(sanitization_patch_auto);
				}
				message("...finished last backward analysis for sanitization patch with diff after length restriction.");
				is_sanitization_patch_required = true;
				is_length_patch_required = true;
			}

		} catch (StrangerStringAnalysisException const &e) {
			cerr << e.what();
			exit(EXIT_FAILURE);
		}

	} else {
		message("CONTINUE SANITIZATION ANALYSIS PHASE........................................");
		message("starting last backward analysis for sanitization patch with diff auto...");
		start_time = perfInfo.current_time();
		sanitization_patch_auto = computePatcheeSanitizationPatch(differenceAuto, patcheeAnalysisResult);
		perfInfo.sanitization_patch_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_SP != 0) {
			DEBUG_MESSAGE("Sanitization patch auto");
			DEBUG_AUTO(sanitization_patch_auto);
		}
		message("...finished last backward analysis for sanitization patch with diff auto.");
		is_sanitization_patch_required = true;
		is_length_patch_required = false;
	}
	perfInfo.calculate_total_sanitization_length_extraction_time();
	perfInfo.calculate_total_sanitization_extraction_time();
	message("........................................END SANITIZATION ANALYSIS PHASE");
	return sanitization_patch_auto;
}

//StrangerAutomaton* StrangerPatcher::testVulnerabilitySignature(){
//
//	validation_patch_auto = StrangerAutomaton::makeAnyString();
//	AnalysisResult patcheeAnalysisResult = computePatcheeFWAnalysis();
//	StrangerAutomaton* sinkAuto = patcheeAnalysisResult[patchee_field_relevant_graph.getRoot()->getID()];
//	StrangerAutomaton* attackPattern = StrangerAutomaton::regExToAuto("/.*<script.*>.*/");
//	StrangerAutomaton* intersection = sinkAuto->intersect(attackPattern);
//	StrangerAutomaton* signature = computePatcheeSanitizationPatch(intersection, patcheeAnalysisResult);
//
//	cout << "********************************** signature **********************************" << endl;
//	signature->toDotAscii(0);
//	cout << endl << endl;
//	return signature;
//
//}

void StrangerPatcher::testNewFunctions() {
	AnalysisResult testAnalysisResult;
	ImageComputer testImageComputer;

//	cout << this->patcher_field_relevant_graph.toDot() << endl;
//	testImageComputer.doForwardAnalysis_GeneralCase(patcher_dep_graph, patcher_dep_graph.getRoot(), testAnalysisResult);

	testImageComputer.doBackwardAnalysis_GeneralCase(patcher_dep_graph,patcher_dep_graph,StrangerAutomaton::makeAnyString(), testAnalysisResult);
}


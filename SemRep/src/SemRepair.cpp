/*
 * SemRepair.cpp
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

#include "SemRepair.hpp"

PerfInfo SemRepair::perfInfo;

SemRepair::SemRepair(string reference_dep_graph_file_name,string target_dep_graph_file_name, string input_field_name) {

	this->reference_dep_graph_file_name = reference_dep_graph_file_name;
	this->target_dep_graph_file_name = target_dep_graph_file_name;
	this->input_field_name = input_field_name;

	// read dep graphs
	this->reference_dep_graph = DepGraph::parseDotFile(reference_dep_graph_file_name);
	this->target_dep_graph = DepGraph::parseDotFile(target_dep_graph_file_name);

	// initialize input nodes
	this->reference_uninit_field_node = reference_dep_graph.findInputNode(input_field_name);
	if (reference_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in reference dep graph.");
	}
	message(stringbuilder() << "reference uninit node(" << reference_uninit_field_node->getID() << ") found for field " << input_field_name << ".");

	this->target_uninit_field_node = target_dep_graph.findInputNode(input_field_name);
	if (target_uninit_field_node == NULL) {
		throw StrangerStringAnalysisException("Cannot find input node " + input_field_name + " in target dep graph.");
	}
	message(stringbuilder() << "target uninit node(" << target_uninit_field_node->getID() << ") found for field " << input_field_name << ".");

	// initialize input relevant graphs
	this->reference_field_relevant_graph = reference_dep_graph.getInputRelevantGraph(reference_uninit_field_node);
	this->target_field_relevant_graph = target_dep_graph.getInputRelevantGraph(target_uninit_field_node);


	if (DEBUG_ENABLED_INIT != 0) {
		DEBUG_MESSAGE("------------ Debugging Initalization ------------");

//		DEBUG_MESSAGE("reference Dependency Graph");
//		this->reference_dep_graph.toDot();
//		DEBUG_MESSAGE("reference Field Relevant Dependency Graph");
//		this->reference_field_relevant_graph.toDot();

		DEBUG_MESSAGE("Target Dependency Graph");
		this->target_dep_graph.toDot();
		DEBUG_MESSAGE("Target Field Relevant Dependency Graph");
		this->target_field_relevant_graph.toDot();

	}

		DEBUG_AUTO_TO_FILE_MN(validation_patch_auto, "");
	ImageComputer::perfInfo = &SemRepair::perfInfo;
	ImageComputer::staticInit();
}

SemRepair::~SemRepair() {
	delete this->reference_uninit_field_node;
	delete this->target_uninit_field_node;
}

void SemRepair::message(string msg) {
	cout << endl << "~~~~~~~~~~~>>> SemRepair says: " << msg << endl;
}

string SemRepair::generateOutputFilePath() {
	boost::filesystem::path curr_dir(boost::filesystem::current_path());
	boost::filesystem::path output_dir(curr_dir / "outputs");

	if (! boost::filesystem::exists(output_dir)) {
		boost::filesystem::create_directory(output_dir);
	}

	size_t ref_ext_index = reference_dep_graph_file_name.find_last_of('.');
	if (ref_ext_index == string::npos) {
		ref_ext_index = reference_dep_graph_file_name.length() - 1;
	}
	size_t ref_index =  reference_dep_graph_file_name.find_last_of('/');
	if (ref_index == string::npos) {
		ref_index = 0;
	}
	string ref_file = reference_dep_graph_file_name.substr(ref_index + 1, ref_ext_index - ref_index - 1);

	size_t tar_ext_index = target_dep_graph_file_name.find_last_of('.');
	if (tar_ext_index == string::npos) {
		tar_ext_index = target_dep_graph_file_name.length() - 1;
	}
	size_t tar_index = target_dep_graph_file_name.find_last_of('/');
	if (tar_index == string::npos) {
		tar_index = 0;
	}

	string tar_file = target_dep_graph_file_name.substr(tar_index + 1, tar_ext_index - tar_index - 1);

	return stringbuilder() << output_dir.string() << "/" << ref_file << "_" << tar_file << "_" << input_field_name;
}

void SemRepair::printAnalysisResults(AnalysisResult& result) {
	cout << endl;
	for (auto& entry : result ) {
		cout << "Printing automata for node ID: " << entry.first << endl;
		(entry.second)->toDot();
		cout << endl << endl;
	}
}

void SemRepair::printNodeList(NodesList nodes) {
	cout << endl;
	for (auto node : nodes ) {
		cout << node->getID() << " ";
	}
	cout << endl;
}

// TODO add output file option
void SemRepair::printResults() {
	string file_path =  generateOutputFilePath();

	if (is_validation_patch_required) {
		cout << "\t    - validation patch is generated" << endl;
		string vp_fname = stringbuilder() << file_path << "_vp_auto.dot";
		string vp_mn_fname = stringbuilder() << file_path << "_vp_mn_auto.dot";
		cout << "\t file : " << vp_fname << endl;
		cout << "\t file : " << vp_mn_fname << endl;
		DEBUG_AUTO_TO_FILE(validation_patch_auto, vp_fname);
		DEBUG_AUTO_TO_FILE_MN(validation_patch_auto,vp_mn_fname);

		cout << "\t size : states " << validation_patch_auto->get_num_of_states() << " : "
				<< "bddnodes " << validation_patch_auto->get_num_of_bdd_nodes() << endl;

		//cout << "\t    - validation patch is generated" << endl;

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

		string lp_fname = stringbuilder() << file_path << "_lp_auto.dot";
		string lp_mn_fname = stringbuilder() << file_path << "_lp_mn_auto.dot";
		cout << "\t file : " << lp_fname << endl;
		cout << "\t file : " << lp_mn_fname << endl;
		DEBUG_AUTO_TO_FILE(length_patch_auto,lp_fname);
		DEBUG_AUTO_TO_FILE_MN(length_patch_auto,lp_mn_fname);

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

		string sp_fname = stringbuilder() << file_path << "_sp_auto.dot";
		string sp_mn_fname = stringbuilder() << file_path << "_sp_mn_auto.dot";
		string refsink_mn_fname = stringbuilder() << file_path << "_refsink_mn_auto.dot";
		cout << "\t file : " << sp_fname << endl;
		cout << "\t file : " << sp_mn_fname << endl;
		cout << "\t file : " << refsink_mn_fname << endl;
		DEBUG_AUTO_TO_FILE(sanitization_patch_auto,sp_fname);
		DEBUG_AUTO_TO_FILE_MN(sanitization_patch_auto,sp_mn_fname);
		DEBUG_AUTO_TO_FILE_MN(reference_sink_auto,refsink_mn_fname);

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


/**
 * checks if length has maximum restriction, or minimum restriction without a maximium restriction
 * TODO currently that function only checks if the minimum restriction length is 1 or not, handle any minimum restriction later
 * result 0 : no worries about length
 * result 1 : there is a maximum length restriction (may also have minimum inside)
 * result 2 : there is a minimum length restriction (max length is infinite in that case)
 */
int SemRepair::isLengthAnIssue(StrangerAutomaton* referenceAuto, StrangerAutomaton*targetAuto) {
	message("BEGIN LENGTH PATCH ANALYSIS PHASE........................................");
	boost::posix_time::ptime start_time = perfInfo.current_time();
	int result = 0;
	if(referenceAuto->isLengthFinite()) {
		if (targetAuto->isLengthFinite()) {
			if (referenceAuto->getMaxLength() < targetAuto->getMaxLength()) {
				result = 1;
			}
		}
		else {
			result = 1;
		}
	} else if (referenceAuto->checkEmptyString()) {
		if( !targetAuto->checkEmptyString() ) {
			result = 2;
		}
	}
	perfInfo.sanitization_length_issue_check_time = perfInfo.current_time() - start_time;
	return result;
}

/**
 * Initial backward analysis phase for extracting validation behavior
 */
StrangerAutomaton* SemRepair::computeValidationPatch() {

	message("BEGIN VALIDATION ANALYSIS PHASE........................................");

	ImageComputer analyzer;
	boost::posix_time::ptime start_time;
	try {
		message("extracting validation from reference");
		start_time = perfInfo.current_time();
		AnalysisResult reference_validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationCase(reference_dep_graph, reference_field_relevant_graph, StrangerAutomaton::makeBottom());
		StrangerAutomaton* reference_negVPatch = reference_validationExtractionResults[reference_uninit_field_node->getID()];
		StrangerAutomaton* reference_validation = reference_negVPatch;
		if ( !calculate_rejected_set ) {
			reference_validation = reference_negVPatch->complement(reference_uninit_field_node->getID());
		}

		perfInfo.validation_reference_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("reference validation auto:");
			DEBUG_AUTO(reference_validation);
		}

		message("extracting validation from target");
		start_time = perfInfo.current_time();
		AnalysisResult target_validationExtractionResults =
				analyzer.doBackwardAnalysis_ValidationCase(target_dep_graph, target_field_relevant_graph, StrangerAutomaton::makeBottom());
		StrangerAutomaton* target_negVPatch = target_validationExtractionResults[target_uninit_field_node->getID()];
		StrangerAutomaton* target_validation = target_negVPatch;
		if ( !calculate_rejected_set ) {
			target_validation = target_negVPatch->complement(target_uninit_field_node->getID());
		}
		perfInfo.validation_target_backward_time = perfInfo.current_time() - start_time;
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("target validation auto:");
			DEBUG_AUTO(target_validation);
		}

		message("computing validation patch");
		start_time = perfInfo.current_time();
		StrangerAutomaton* diffAuto = NULL;
		if (calculate_rejected_set)  {
			diffAuto = reference_validation->difference(target_validation, target_uninit_field_node->getID());
		}
		else {
			diffAuto = target_validation->difference(reference_validation, target_uninit_field_node->getID());
		}
		if (DEBUG_ENABLED_VP != 0) {
			DEBUG_MESSAGE("difference auto:");
			DEBUG_AUTO(diffAuto);
		}
		if (calculate_rejected_set) {
			if (diffAuto->isEmpty()) {
				message("difference auto is empty, no validation patch is required!!!");
				is_validation_patch_required = false;
				validation_patch_auto = StrangerAutomaton::makeBottom(target_uninit_field_node->getID());

			} else {
				message("validation patch is generated for input: " + input_field_name);
				is_validation_patch_required = true;
				validation_patch_auto = diffAuto->clone(target_uninit_field_node->getID());
				if (DEBUG_ENABLED_VP != 0) {
					DEBUG_MESSAGE("validation patch is difference auto...");
				}
			}
		}
		else {
			StrangerAutomaton* interAuto = target_validation->intersect(reference_validation, target_uninit_field_node->getID());
			if (DEBUG_ENABLED_VP != 0) {
				DEBUG_MESSAGE("intersection auto:");
				DEBUG_AUTO(interAuto);
			}
			if (diffAuto->isEmpty()) {
				message("difference auto is empty, no validation patch is required!!!");
				is_validation_patch_required = false;
				validation_patch_auto = StrangerAutomaton::makeAnyString(target_uninit_field_node->getID());
//				validation_patch_auto = target_validation;
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

		delete reference_validation;
		delete target_validation;
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
 * Computes sink post image for reference
 */
StrangerAutomaton* SemRepair::computeReferenceFWAnalysis() {

	message("computing reference sink post image...");
	AnalysisResult referenceAnalysisResult;
	UninitNodesList referenceUninitNodes = reference_dep_graph.getUninitNodes();

	// initialize reference input nodes to bottom
	message("initializing reference inputs with bottom");
	for (auto uninit_node : referenceUninitNodes) {
		referenceAnalysisResult[uninit_node->getID()] = StrangerAutomaton::makePhi(uninit_node->getID());
	}
	// initialize uninit node that we are interested in with sigma star
	message(stringbuilder() << "initializing input node(" << reference_uninit_field_node->getID() << ") with sigma star");
	delete referenceAnalysisResult[reference_uninit_field_node->getID()];
	referenceAnalysisResult[reference_uninit_field_node->getID()] = StrangerAutomaton::makeAnyString(reference_uninit_field_node->getID());

	ImageComputer referenceAnalyzer;

	try {
		message("starting forward analysis for reference...");
		referenceAnalyzer.doForwardAnalysis_SingleInput(reference_dep_graph, reference_field_relevant_graph, referenceAnalysisResult);
		message("...finished forward analysis for reference.");

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }

	reference_sink_auto = referenceAnalysisResult[reference_field_relevant_graph.getRoot()->getID()];
	message("...computed reference sink post image.");
	return reference_sink_auto;
}

/**
 * Computes sink post image for target, first time
 */
AnalysisResult SemRepair::computeTargetFWAnalysis() {
	AnalysisResult targetAnalysisResult;
	UninitNodesList targetUninitNodes = target_dep_graph.getUninitNodes();

	message("initializing target inputs with bottom");
	for (auto uninit_node : targetUninitNodes) {
		targetAnalysisResult[uninit_node->getID()] = StrangerAutomaton::makePhi(uninit_node->getID());
	}

	// initialize uninit node that we are interested in with validation patch_auto
	message(stringbuilder() << "initializing input node(" << target_uninit_field_node->getID() << ") with validation patch auto");
	delete targetAnalysisResult[target_uninit_field_node->getID()];
	if (calculate_rejected_set) {
		targetAnalysisResult[target_uninit_field_node->getID()] = validation_patch_auto->complement(target_uninit_field_node->getID());
	} else {
		targetAnalysisResult[target_uninit_field_node->getID()] = validation_patch_auto;
	}


	ImageComputer targetAnalyzer;

	try {

		message("starting forward analysis for target...");
		targetAnalyzer.doForwardAnalysis_SingleInput(target_dep_graph, target_field_relevant_graph, targetAnalysisResult);
		message("...finished forward analysis for target.");

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }

	return targetAnalysisResult;
}

StrangerAutomaton* SemRepair::computeTargetLengthPatch(StrangerAutomaton* initialAuto, AnalysisResult& fwAnalysisResult) {
	message("starting a backward analysis to calculate length patch...");
	ImageComputer targetAnalyzer;
	try {
		fwAnalysisResult[target_uninit_field_node->getID()] = StrangerAutomaton::makeAnyString(-5);
		boost::posix_time::ptime start_time = perfInfo.current_time();
		AnalysisResult bwResult = targetAnalyzer.doBackwardAnalysis_GeneralCase(target_dep_graph, target_field_relevant_graph, initialAuto, fwAnalysisResult);
		perfInfo.sanitization_length_backward_time = perfInfo.current_time() - start_time;
		StrangerAutomaton* negPatchAuto = bwResult[target_uninit_field_node->getID()];
		if ( calculate_rejected_set ) {
			length_patch_auto = negPatchAuto->clone(-5);
		} else {
			length_patch_auto = negPatchAuto->complement(-5);
		}
//		fwAnalysisResult[target_uninit_field_node->getID()] = validation_patch_auto->intersect(length_patch_auto,-5);

	} catch (StrangerStringAnalysisException const &e) {
        cerr << e.what();
        exit(EXIT_FAILURE);
    }
	message("...length patch is generated");
	return length_patch_auto;
}

StrangerAutomaton* SemRepair::computeTargetSanitizationPatch(StrangerAutomaton* initialAuto,	const AnalysisResult& fwAnalysisResult) {
	ImageComputer targetAnalyzer;
	AnalysisResult bwResult = targetAnalyzer.doBackwardAnalysis_GeneralCase(target_dep_graph, target_field_relevant_graph, initialAuto, fwAnalysisResult);
	sanitization_patch_auto = bwResult[target_uninit_field_node->getID()];
	return sanitization_patch_auto;
}


/**
 *
 */
StrangerAutomaton* SemRepair::computeSanitizationPatch() {
	message("BEGIN SANITIZATION ANALYSIS PHASE........................................");
	boost::posix_time::ptime start_time = perfInfo.current_time();
	reference_sink_auto = computeReferenceFWAnalysis();
	perfInfo.sanitization_reference_first_forward_time = perfInfo.current_time() - start_time;
	if (DEBUG_ENABLED_SC != 0) {
		DEBUG_MESSAGE("Reference Sink Auto - First forward analysis");
		DEBUG_AUTO(reference_sink_auto);
	}
	start_time = perfInfo.current_time();
	AnalysisResult targetAnalysisResult = computeTargetFWAnalysis();
	StrangerAutomaton* targetSinkAuto = targetAnalysisResult[target_field_relevant_graph.getRoot()->getID()];
	perfInfo.sanitization_target_first_forward_time = perfInfo.current_time() - start_time;
	if (DEBUG_ENABLED_SC != 0) {
		DEBUG_MESSAGE("Target Sink Auto - First forward analysis");
		DEBUG_AUTO(targetSinkAuto);
	}

	message("checking difference between reference and target");
	boost::posix_time::ptime comp_time = perfInfo.current_time();
	StrangerAutomaton* differenceAuto = targetSinkAuto->difference(reference_sink_auto, -3);
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
	} else if( (length_check_result = isLengthAnIssue(reference_sink_auto,targetSinkAuto)) != 0) {
		message("length constraints contribute to the difference, fixing issue...");
		StrangerAutomaton* lengthRestrictAuto = NULL;
		if (length_check_result == 1) {
			start_time = perfInfo.current_time();
			lengthRestrictAuto = targetSinkAuto->restrictLengthByOtherAutomatonFinite(reference_sink_auto, -4);
			perfInfo.sanitization_length_issue_check_time += perfInfo.current_time() - start_time; // adding to length issue check in if statements
		} else if (length_check_result == 2) {
			start_time = perfInfo.current_time();
			StrangerAutomaton* emptyAuto = StrangerAutomaton::makeEmptyString(-4);
			StrangerAutomaton* negEmptyAuto = emptyAuto->complement(-4);
			lengthRestrictAuto = targetSinkAuto->intersect(negEmptyAuto);
			delete emptyAuto;
			delete negEmptyAuto;
			perfInfo.sanitization_length_issue_check_time += perfInfo.current_time() - start_time;
		}

		if (DEBUG_ENABLED_LP != 0) {
			DEBUG_MESSAGE("Length restricted target sink automaton:");
			DEBUG_AUTO(lengthRestrictAuto);
		}

		try {
			// use negation of length restricted auto to solve the problem of overapproximation
			StrangerAutomaton* negLengthRestrictAuto = lengthRestrictAuto->complement(-4);
			StrangerAutomaton* rejectedLengthAuto = targetSinkAuto->intersect(negLengthRestrictAuto, -4);
			delete negLengthRestrictAuto;
//			cout << "rejected length automaton: " << endl;
//			rejectedLengthAuto->toDotAscii(0);
			computeTargetLengthPatch(rejectedLengthAuto, targetAnalysisResult);

			if (DEBUG_ENABLED_LP != 0) {
				DEBUG_MESSAGE("Length patch automaton");
				DEBUG_AUTO(length_patch_auto);
			}
			message("........................................END LENGTH_PATCH ANALYSIS PHASE");
			message("CONTINUE SANITIZATION ANALYSIS PHASE........................................");
			message("checking difference between reference and target after length restriction");
			delete differenceAuto;
			comp_time = perfInfo.current_time();
			differenceAuto = lengthRestrictAuto->difference(reference_sink_auto, -3);
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
				sanitization_patch_auto = computeTargetSanitizationPatch(differenceAuto, targetAnalysisResult);
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
		sanitization_patch_auto = computeTargetSanitizationPatch(differenceAuto, targetAnalysisResult);
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

void SemRepair::testNewFunctions() {
	this->reference_dep_graph.calculateSCCs();
}





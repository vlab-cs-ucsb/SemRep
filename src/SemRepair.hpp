/*
 * SemRepair.hpp
 *
 *  Created on: Feb 10, 2014
 *      Author: baki
 */

#ifndef SEMREPAIR_HPP_
#define SEMREPAIR_HPP_

#include "StrangerAutomaton.hpp"
#include "DepGraph.hpp"
#include "ImageComputer.hpp"
#include "SemRepairDebugger.hpp"

using namespace std;

class SemRepair {
public:
	SemRepair(string reference_dep_graph_file_name, string target_dep_graph_file_name, string input_field_name);
	virtual ~SemRepair();

	StrangerAutomaton* computeValidationPatch();
	StrangerAutomaton* computeSanitizationPatch();

	StrangerAutomaton* computeReferenceFWAnalysis();
	AnalysisResult computeTargetFWAnalysis();
	StrangerAutomaton* computeTargetLengthPatch(StrangerAutomaton* initialAuto, AnalysisResult& fwAnalysisResult);
	StrangerAutomaton* computeTargetSanitizationPatch(StrangerAutomaton* initialAuto, const AnalysisResult& fwAnalysisResult);

	StrangerAutomaton* getValidationPatchAuto() { return validation_patch_auto; }
	StrangerAutomaton* getLengthPatchAuto() { return length_patch_auto; }
	StrangerAutomaton* getSanitizationPatchAuto() { return sanitization_patch_auto; }

//	StrangerAutomaton* testVulnerabilitySignature();
	void testNewFunctions();

	void printResults();
	void writeAutosforCodeGeneration(string field_name, string referenceName, string patchName);

	bool is_validation_patch_required = false;
	bool is_length_patch_required = false;
	bool is_sanitization_patch_required = false;

	bool calculate_rejected_set = false;

	static PerfInfo perfInfo;
private:
	string reference_dep_graph_file_name;
	string target_dep_graph_file_name;
	string input_field_name;

	DepGraph reference_dep_graph;
	DepGraph target_dep_graph;
	DepGraph reference_field_relevant_graph;
	DepGraph target_field_relevant_graph;

	DepGraphNode* reference_uninit_field_node;
	DepGraphNode* target_uninit_field_node;

	NodesList reference_sorted_field_relevant_nodes;
	NodesList target_sorted_field_relevant_nodes;

	StrangerAutomaton* reference_sink_auto;

	StrangerAutomaton* length_patch_auto;
	StrangerAutomaton* validation_patch_auto;
	StrangerAutomaton* sanitization_patch_auto;

	void message(string msg);
	void printAnalysisResults(AnalysisResult& result);
	void printNodeList(NodesList nodes);
	int isLengthAnIssue(StrangerAutomaton* patcherAuto, StrangerAutomaton*patcheeAuto);
};



#endif /* SEMREPAIR_HPP_ */

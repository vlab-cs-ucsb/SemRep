/*
 * StrangerPatcher.h
 *
 *  Created on: Oct 30, 2013
 *      Author: baki
 */

#ifndef STRANGERPATCHER_H_
#define STRANGERPATCHER_H_

#include "stranger_lib_internal.h"
#include <iostream>
#include <vector>
#include "stranger.h"
#include "bdd.h"
#include "dfa.h"
#include "bdd_dump.h"
#include "bdd_external.h"
#include "StrangerAutomaton.hpp"
#include <regex>
#include <fstream>
#include <cstdio>
#include <ctime>

#include <string>
#include <map>
#include <utility>


#include "DepGraph.h"
#include "ImageComputer.h"
#include "StrangerDebugger.hpp"

using namespace std;

class StrangerPatcher {
public:
	StrangerPatcher(string patcher_dep_graph_file_name, string patchee_dep_graph_file_name, string input_field_name);
	virtual ~StrangerPatcher();

	StrangerAutomaton* computeValidationPatch();
	StrangerAutomaton* computeSanitizationPatch();

	StrangerAutomaton* computePatcherFWAnalysis();
	AnalysisResult computePatcheeFWAnalysis();
	StrangerAutomaton* computePatcheeLengthPatch(StrangerAutomaton* initialAuto, AnalysisResult& fwAnalysisResult);
	StrangerAutomaton* computePatcheeSanitizationPatch(StrangerAutomaton* initialAuto, const AnalysisResult& fwAnalysisResult);

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
	string patcher_dep_graph_file_name;
	string patchee_dep_graph_file_name;
	string input_field_name;

	DepGraph patcher_dep_graph;
	DepGraph patchee_dep_graph;
	DepGraph patcher_field_relevant_graph;
	DepGraph patchee_field_relevant_graph;

	DepGraphNode* patcher_uninit_field_node;
	DepGraphNode* patchee_uninit_field_node;

	NodesList patcher_sorted_field_relevant_nodes;
	NodesList patchee_sorted_field_relevant_nodes;

	StrangerAutomaton* patcher_sink_auto;

	StrangerAutomaton* length_patch_auto;
	StrangerAutomaton* validation_patch_auto;
	StrangerAutomaton* sanitization_patch_auto;

	void message(string msg);
	void printAnalysisResults(AnalysisResult& result);
	void printNodeList(NodesList nodes);
	int isLengthAnIssue(StrangerAutomaton* patcherAuto, StrangerAutomaton*patcheeAuto);
};


#endif /* STRANGERPATCHER_H_ */

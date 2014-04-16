/*
 * SemRepair.hpp
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

#ifndef SEMREPAIR_HPP_
#define SEMREPAIR_HPP_

#include <boost/filesystem.hpp>
#include "StrangerAutomaton.hpp"
#include "ImageComputer.hpp"
#include "SemRepairDebugger.hpp"
#include "depgraph/DepGraph.hpp"

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

	void testNewFunctions();

	void printResults();

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

	StrangerAutomaton* reference_sink_auto;

	StrangerAutomaton* length_patch_auto;
	StrangerAutomaton* validation_patch_auto;
	StrangerAutomaton* sanitization_patch_auto;

	void message(string msg);
	string generateOutputFilePath();
	void printAnalysisResults(AnalysisResult& result);
	void printNodeList(NodesList nodes);
	int isLengthAnIssue(StrangerAutomaton* patcherAuto, StrangerAutomaton*patcheeAuto);
};



#endif /* SEMREPAIR_HPP_ */

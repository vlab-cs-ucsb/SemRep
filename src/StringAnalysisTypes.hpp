/*
 * StringAnalysisTypes.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef STRINGANALYSISTYPES_HPP_
#define STRINGANALYSISTYPES_HPP_

#include "StrangerAutomaton.hpp"
#include "DepGraph.hpp"
#include <map>
#include <vector>

typedef std::map<const int, StrangerAutomaton*> AnalysisResult;
typedef std::map<const int, StrangerAutomaton*>::iterator AnalysisResultIterator;
typedef std::map<const int, StrangerAutomaton*>::const_iterator AnalysisResultConstIterator;

typedef std::map<int, NodesList> SccNodes;
typedef std::map<int, NodesList>::iterator SccNodesIterator;
typedef std::map<int, NodesList>::const_iterator SccNodesConstIterator;


#endif /* STRINGANALYSISTYPES_HPP_ */

//
//  StringAnalysisTypes.h
//  stranger
//
//  Created by Muath Alkhalaf on 9/5/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//

#ifndef stranger_StringAnalysisTypes_h
#define stranger_StringAnalysisTypes_h

#include "StrangerAutomaton.hpp"
#include "DepGraph.h"
#include <map>
#include <vector>

typedef std::map<const int, StrangerAutomaton*> AnalysisResult;
typedef std::map<const int, StrangerAutomaton*>::iterator AnalysisResultIterator;
typedef std::map<const int, StrangerAutomaton*>::const_iterator AnalysisResultConstIterator;

typedef std::map<int, NodesList> SccNodes;
typedef std::map<int, NodesList>::iterator SccNodesIterator;
typedef std::map<int, NodesList>::const_iterator SccNodesConstIterator;

#endif

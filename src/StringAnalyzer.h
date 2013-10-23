//
//  StringAnalyzer.h
//  stranger
//
//  Created by Muath Alkhalaf on 8/25/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//

#ifndef stranger_StringAnalyzer_h
#define stranger_StringAnalyzer_h

#include "StrangerAutomaton.hpp"
#include "StringAnalysisTypes.h"
#include "ForwardImageComputer.h"
#include "DepGraph.h"
#include "StrangerStringAnalysisException.h"
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <exception>
#include <stdexcept>


class StringAnalyzer {
public:
    StringAnalyzer(std::string depGraphFileName, string depGraphFileName2, std::string analysisName);
    StringAnalyzer(DepGraph depGraph);
    virtual ~StringAnalyzer(){};
    void analyze();
    
private:
    ForwardImageComputer imageComputer;
    DepGraph depGraph;
    DepGraph depGraph2;
    std::string name;
    AnalysisResult annotateInputWithSigmaStar(UninitNodesList inputNodes);
};

#endif

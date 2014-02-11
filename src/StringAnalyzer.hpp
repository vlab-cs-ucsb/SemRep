/*
 * StringAnalyzer.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef STRINGANALYZER_HPP_
#define STRINGANALYZER_HPP_

#include "StrangerAutomaton.hpp"
#include "StringAnalysisTypes.hpp"
#include "StrangerStringAnalysisException.hpp"
#include "ImageComputer.hpp"
#include "DepGraph.hpp"


class StringAnalyzer {
public:
    StringAnalyzer(std::string depGraphFileName, std::string depGraphFileName2, std::string analysisName);
    StringAnalyzer(DepGraph depGraph);
    virtual ~StringAnalyzer(){};
    void analyze();

private:
    ImageComputer imageComputer;
    DepGraph depGraph;
    DepGraph depGraph2;
    std::string name;
    AnalysisResult annotateInputWithSigmaStar(UninitNodesList inputNodes);
};


#endif /* STRINGANALYZER_HPP_ */

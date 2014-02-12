/*
 * StringAnalyzer.hpp
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

#ifndef STRINGANALYZER_HPP_
#define STRINGANALYZER_HPP_

#include "StrangerAutomaton.hpp"
#include "StringAnalysisTypes.hpp"
#include "exceptions/StrangerStringAnalysisException.hpp"
#include "ImageComputer.hpp"
#include "depgraph/DepGraph.hpp"


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

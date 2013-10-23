//
//  StringAnalyzer.cpp
//  stranger
//
//  Created by Muath Alkhalaf on 9/5/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//

#include "StringAnalyzer.h"

using namespace std;

StringAnalyzer::StringAnalyzer(string depGraphFileName, string depGraphFileName2, string analysisName){
    this->depGraph = DepGraph::parseDotFile(depGraphFileName);
    this->depGraph2 = DepGraph::parseDotFile(depGraphFileName2);
    this->name = analysisName;
}

StringAnalyzer::StringAnalyzer(DepGraph depGraph){
    this->depGraph = depGraph;
}

AnalysisResult StringAnalyzer::annotateInputWithSigmaStar(UninitNodesList inputNodes) {
    AnalysisResult inputValuesMap;
    for (auto inputNode: inputNodes){
        inputValuesMap[inputNode->getID()] = StrangerAutomaton::makeAnyString(inputNode->getID());
    }
    return inputValuesMap;
}

void StringAnalyzer::analyze(){
    cout << endl;
    cout << "*****************" << endl;
    cout << name
                       << " Stranger Sanit Analysis BEGIN" << endl;
    cout << "*****************" << endl;
    cout << endl;
    

    // dump the automaton that represents the undesired stuff
    // TODO: it is good to have this for stranger
    // this.dumpDotAuto(this.undesir, "undesired_" << name,
    // MyOptions.graphPath);
    

        try {
            long start = imageComputer.perfInfo.currentTimeMillis();
            
            
            OpNodesList f_unmodeled;
//            DepGraph acyclicWorkGraph(this->depGraph);
            DepGraph& acyclicWorkGraph = this->depGraph;
            DepGraph& acyclicWorkGraph2 = this->depGraph2;
            acyclicWorkGraph.sort(depGraph);
            acyclicWorkGraph2.sort(depGraph2);
            // get a mapping between each SCC nodes and nodes in its cycle
//            SccNodes sccNodes = acyclicWorkGraph
//            .sort(this->depGraph);
            // This is to open a file where we have a ctrace for our analysis
//            prepare_ctrace(acyclicWorkGraph, name);
            
            // input nodes are nodes that contribute untrusted input to the program:
            // 1- predecessors of tainted uninit nodes (input comes from HTTP GET or POST)
            // 2- nodes for untrusted input functions. Input functions are functions used
            // 	  to get user input such as DB function mysql_fetch_array.
            //    We only add the function if it is not trusted
//            NodesConstList inputNodes = getHTTPInputNodes(this->depGraph, acyclicWorkGraph);
            cout << depGraph.toDot();
            cout << depGraph2.toDot();
            UninitNodesList inputNodes = depGraph.getUninitNodes();
            UninitNodesList inputNodes2 = depGraph2.getUninitNodes();

//            getUntrustedInputFuncsNodes(acyclicWorkGraph, inputNodes);
            
            
            // even if there is no input we may need to run the analysis
            // This is in case we have unmodeled functions which cased
            // the sink to be tainted (although there is no dangerous input)
            if (inputNodes.size() != 1 || inputNodes2.size() != 1)
                cout << "\ninputNodes size is not 1 in string analysis phase!!\n" << endl;
            
            
            // initially we will take the most general case which is annotating the input with
            // sigma*. Later one when starting backward analysis this should change
            AnalysisResult inputValuesMap = annotateInputWithSigmaStar(inputNodes);
            AnalysisResult inputValuesMap2 = annotateInputWithSigmaStar(inputNodes2);

            
            
            /**************************************************/
            AnalysisResult forwardDeco = imageComputer.computeFwImage(this->depGraph, acyclicWorkGraph,/* sccNodes,*/ inputValuesMap, false);
            /**************************************************/
            
            /**************************************************/
            AnalysisResult forwardDeco2 = imageComputer.computeFwImage(this->depGraph2, acyclicWorkGraph2,/* sccNodes,*/ inputValuesMap2, false);
            /**************************************************/
            
            long startIntersection = imageComputer.perfInfo.currentTimeMillis();
            // intersect root automaton with the undesired stuff;
            // if the intersection is empty, it means that we are safe!
            StrangerAutomaton* rootAuto = forwardDeco[acyclicWorkGraph.getRoot()->getID()];
            StrangerAutomaton* rootAuto2 = forwardDeco2[acyclicWorkGraph2.getRoot()->getID()];
            rootAuto->toDotAscii(0);
            rootAuto2->toDotAscii(0);
            
            if (rootAuto2->isLengthFinite()){
                rootAuto = rootAuto->restrictLengthByOtherAutomaton(rootAuto2, acyclicWorkGraph.getRoot()->getID());
                AnalysisResult bwAnalysisResult = imageComputer.computeBwImage(this->depGraph, acyclicWorkGraph,
                                             /*sccNodes,*/ inputValuesMap, rootAuto, *(inputNodes.begin()), forwardDeco);
                //Here we generate the length patch
                
                for (auto resultPair : forwardDeco){
                    delete resultPair.second;
                }
                forwardDeco.clear();

                DepGraphNode* firstInputNode = *inputNodes.begin();
                inputValuesMap[firstInputNode->getID()] = bwAnalysisResult[firstInputNode->getID()]->clone(firstInputNode->getID());
                
                for (auto resultPair : bwAnalysisResult){
                    delete resultPair.second;
                }
                bwAnalysisResult.clear();
                
                /**************************************************/
                forwardDeco = imageComputer.computeFwImage(this->depGraph, acyclicWorkGraph,/* sccNodes,*/ inputValuesMap, false);
                /**************************************************/

            }
            rootAuto = forwardDeco[acyclicWorkGraph.getRoot()->getID()];
            StrangerAutomaton* diffAuto = rootAuto->intersect(rootAuto2->complement(rootAuto2->getID()), rootAuto->getID());
            
            AnalysisResult bwAnalysisResult = imageComputer.computeBwImage(this->depGraph, acyclicWorkGraph,
                                                                           /*sccNodes,*/ inputValuesMap, diffAuto, *(inputNodes.begin()), forwardDeco);
            
            for (auto resultPair : forwardDeco){
                delete resultPair.second;
            }
            forwardDeco.clear();
            for (auto resultPair : bwAnalysisResult){
                delete resultPair.second;
            }
            bwAnalysisResult.clear();
            
            
            //Here we generate the mincut patch
            
            
            /***************************************/
            /** get performance info for this sink **/
            long stop = imageComputer.perfInfo.currentTimeMillis();
            imageComputer.perfInfo.sinkRunningTime = stop - start;
//            printPerfInfo();
            // we gather perf into per depgraph (per sink)
//            resetPerfInfo();
            /***************************************/
            
        } catch (StrangerStringAnalysisException& e) {
            cerr << e.what();
        }
        
        
    
    
    

    
    
    cout << endl;
    cout << "*****************" << endl;
    cout << name
                       <<" Stranger Sanit Analysis END" << endl;
    cout << "*****************" << endl;
    cout << endl;
    
    // only to output c trace file. One file per all sink places(depgraphs)
    //StrangerAutomaton.closeCtraceFile();
    
}



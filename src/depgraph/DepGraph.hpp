/*
 * DepGraph.hpp
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

#ifndef DEPGRAPH_HPP_
#define DEPGRAPH_HPP_

#include "DepGraphNode.hpp"
#include "DepGraphSccNode.hpp"
#include "DepGraphUninitNode.hpp"
#include "DepGraphOpNode.hpp"
#include "DepGraphNormalNode.hpp"

#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>
#include <fstream>
#include <boost/regex.hpp>

struct NodeLessThan : public std::binary_function<const DepGraphNode*, const DepGraphNode*, bool> {
    bool operator() (const DepGraphNode* pN1,const DepGraphNode* pN2) const
    {
    	return (pN1->getID() < pN2->getID());
    }
};
struct NodeEqual : public std::binary_function<const DepGraphNode*, const DepGraphNode*, bool> {
    bool operator() (const DepGraphNode* pN1, const DepGraphNode* pN2) const
    {
    	return (pN1->getID() == pN2->getID());
    }
};

typedef std::map<int, DepGraphNode*> NodesMap;
typedef std::map<int, DepGraphNode*>::iterator NodesMapIterator;
typedef std::map<int, DepGraphNode*>::const_iterator NodesMapConstIterator;
typedef std::vector<DepGraphNode*> NodesList;
typedef std::vector<DepGraphNode*>::iterator NodesListIterator;
typedef std::vector<DepGraphNode*>::const_iterator NodesListConstIterator;
typedef std::vector<DepGraphNode*>::reverse_iterator NodesListReverseIterator;
typedef std::vector<DepGraphNode *>::const_reverse_iterator NodesListConstReverseIterator;
//typedef std::vector<const DepGraphNode*> NodesConstList;
//typedef std::vector<const DepGraphNode*>::iterator NodesConstListIterator;
//typedef std::vector<const DepGraphNode*>::const_iterator NodesConstListConstIterator;
typedef std::map<const DepGraphNode*, NodesList, NodeLessThan> EdgesMap;
typedef std::map<const DepGraphNode*, NodesList, NodeLessThan>::iterator EdgesMapIterator;
typedef std::map<const DepGraphNode*, NodesList, NodeLessThan>::const_iterator EdgesMapConstIterator;
//typedef std::set<DepGraphNode*, NodeLessThan> NodesSet;
//typedef std::set<DepGraphNode*, NodeLessThan>::iterator NodesSetIterator;
//typedef std::set<DepGraphNode*, NodeLessThan>::const_iterator NodesSetConstIterator;
//typedef std::set<const DepGraphNode*, NodeLessThan> NodesConstSet;
//typedef std::set<const DepGraphNode*, NodeLessThan>::iterator NodesConstSetIterator;
//typedef std::set<const DepGraphNode*, NodeLessThan>::const_iterator NodesConstSetConstIterator;
//typedef std::set<DepGraphOpNode*, NodeLessThan> OpNodesSet;
//typedef std::set<DepGraphOpNode*, NodeLessThan>::iterator OpNodesSetIterator;
//typedef std::set<DepGraphOpNode*, NodeLessThan>::const_iterator OpNodesSetConstIterator;
//typedef std::set<const DepGraphOpNode*, NodeLessThan> OpNodesConstSet;
//typedef std::set<const DepGraphOpNode*, NodeLessThan>::iterator OpNodesConstSetIterator;
//typedef std::set<const DepGraphOpNode*, NodeLessThan>::const_iterator OpNodesConstSetConstIterator;
typedef std::vector<DepGraphOpNode*> OpNodesList;
typedef std::vector<DepGraphOpNode*>::iterator OpNodesListIterator;
typedef std::set<DepGraphOpNode*>::const_iterator OpNodesListConstIterator;


//typedef std::set<DepGraphUninitNode*, NodeLessThan> UninitNodesSet;
//typedef std::set<DepGraphUninitNode*, NodeLessThan>::iterator UninitNodesSetIterator;
//typedef std::set<DepGraphUninitNode*, NodeLessThan>::const_iterator UninitNodesSetConstIterator;
//typedef std::set<const DepGraphUninitNode*, NodeLessThan> UninitNodesConstSet;
//typedef std::set<const DepGraphUninitNode*, NodeLessThan>::iterator UninitNodesConstSetIterator;
//typedef std::set<const DepGraphUninitNode*, NodeLessThan>::const_iterator UninitNodesConstSetConstIterator;
typedef std::vector<DepGraphUninitNode*> UninitNodesList;
typedef std::vector<DepGraphUninitNode*>::iterator UninitNodesListIterator;
typedef std::vector<DepGraphUninitNode*>::const_iterator UninitNodesListConstIterator;



class DepGraph {
public:
	DepGraph();
	DepGraph(DepGraphNormalNode* root) : leavesReduced(false) { this->root = root; this->addNode(root);};
    DepGraph(const DepGraph& other);
    DepGraph& operator=(const DepGraph &other);
	virtual ~DepGraph();
	NodesList getPredecessors(const DepGraphNode* node);
	NodesList getSuccessors(DepGraphNode* node);
	DepGraphNormalNode *getRoot() {
		return this->root;
	};
	void setRoot(DepGraphNormalNode* root) {
		this->root = root;
	};
	DepGraphNode *getTopLeaf()  {
			return this->topLeaf;
	};
	void setTopLeaf(DepGraphNode* topLeaf) {
		this->topLeaf = topLeaf;
	};
	OpNodesList getFuncsNodes(const std::vector<std::string> funcsNames) ;
	int getNumOfNodes() const{
	    	return ((int)nodes.size());
	};
	int getNumOfEdges() const {
		int num = 0;
		for (EdgesMapConstIterator it = edges.begin(); it != edges.end(); ++it){
			num += it->second.size();
		}
		return num;
	};
	NodesList getNodes();
	UninitNodesList getUninitNodes() ;

	void calculateSCCs();


	DepGraph getInputRelevantGraph(DepGraphNode* inputNode) ;
	bool containsNode(const DepGraphNode* node) ;
	// you must only draw edges between already existing nodes in the graph
	void addEdge(DepGraphNode* from, DepGraphNode* to);
	// never add an already existing node
	DepGraphNode* addNode(DepGraphNode* node);
    DepGraphNode* getNode(const int id) ;
    static DepGraph parseDotFile(std::string fname);
    std::string toDot();
    void dumpDot(string fname);

    DepGraphUninitNode* findInputNode(string name);

    std::string label;
    std::string labelloc;
private:
// map from a node to *the same* node;
	NodesMap nodes;

	// the root node (is also contained in "nodes")
	DepGraphNormalNode *root;

	// node where we should start fix point computation for StrangerAnalysis
	// this node should be the one with the highest topological order number
	DepGraphNode *topLeaf;

	// edges (from -> to)
	EdgesMap edges;


	// set to true if reduceWithLeaves() is called
	bool leavesReduced;


	static int currentID;
	static int currentSccID;
	static int currentOrder;

	// members for scc nodes (computed with tarjan's algorithm)
	map<int, vector<DepGraphNode*>> scc_components;
	// contains an entry for a node if it is involved in a cycle that has more than one node
	map<int, int> scc_map;

	void dfsSCC(DepGraphNode* node, int& time_count, map<int, int>& lowlink, map<int, bool>& used, stack<int>& process_stack);

	void printSCCInfo();

	void doGetInputRelevantGraph(DepGraphNode* node,
				DepGraph& inputDepGraph) ;
};



#endif /* DEPGRAPH_HPP_ */

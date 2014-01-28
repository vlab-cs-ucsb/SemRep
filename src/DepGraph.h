/*
 * DepGraph.h
 *
 *  Created on: Aug 20, 2013
 *      Author: muath
 */

#ifndef DEPGRAPH_H_
#define DEPGRAPH_H_

#include "DepGraphNode.h"
#include "DepGraphSccNode.h"
#include "DepGraphUninitNode.h"
#include "DepGraphOpNode.h"
#include "DepGraphNormalNode.h"
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <sstream>

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
	std::vector<NodesList> getSccs()  {
		throw std::runtime_error("DepGraph.getSccs() not implemented yet.");
	};
	// sort the graph in a topologically
	void sort(DepGraph *origGraph);
	NodesList getSortedNodes()  {
		return sortedNodes;
	};
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
    void sort(DepGraph& origGraph);

    DepGraphUninitNode* findInputNode(string name);

    std::string label;
    std::string labelloc;
private:
	// map from a node to *the same* node;
	    // necessary due to the usual limitation of java.util.Set
	    NodesMap nodes;

	    // the root node (is also contained in "nodes")
	    DepGraphNormalNode *root;

	    // node where we should start fix point computation for StrangerAnalysis
	    // this node should be the one with the highest topological order number
	    DepGraphNode *topLeaf;

	    // edges (from -> to)
	    EdgesMap edges;

	    // nodes sorted topologically
	    NodesList sortedNodes;

	    // just a helper for SCC computation
	    int n;

	    // set to true if reduceWithLeaves() is called
	    bool leavesReduced;


	    static int currentID;
	    static int currentSccID;
	    static int currentOrder;

	    void sccVisit(DepGraphNode* v, NodesList stack,
	            std::map<const DepGraphNode*,int> dfsnum,
	            std::map<const DepGraphNode*,int> low,
	            NodesList old,
	            vector<NodesList> sccs)  {throw std::runtime_error("DepGraph.sccVisit() not implemented yet.");};

	    void doGetInputRelevantGraph(DepGraphNode* node,
	    			DepGraph& inputDepGraph) ;
};

#endif /* DEPGRAPH_H_ */

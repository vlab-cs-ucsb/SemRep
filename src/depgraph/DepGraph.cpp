/*
 * DepGraph.cpp
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

#include "DepGraph.hpp"

using namespace std;

DepGraph::DepGraph() {
    root = NULL;
    topLeaf = NULL;
    n = 0;
	leavesReduced = false;
    label = "";
    labelloc = "";
    time = 0;

}

DepGraph::DepGraph(const DepGraph& other){
    this->root = other.root;
    this->nodes = other.nodes;
    this->topLeaf = other.topLeaf;
    this->edges = other.edges;
    this->sortedNodes = other.sortedNodes;
    this->n = other.n;
    this->leavesReduced = other.leavesReduced;
    this->currentID = other.currentID;
    this->currentSccID = other.currentSccID;
    this->currentOrder = other.currentOrder;
    this->label = other.label;
    this->labelloc = other.labelloc;
    this->components = other.components;
    time = 0;
}

DepGraph::~DepGraph() {
//    for (auto nodePair : nodes){
//        delete nodePair.second;
//    }
}

DepGraph& DepGraph::operator=(const DepGraph &other){
    this->root = other.root;
    this->nodes = other.nodes;
    this->topLeaf = other.topLeaf;
    this->edges = other.edges;
    this->sortedNodes = other.sortedNodes;
    this->n = other.n;
    this->leavesReduced = other.leavesReduced;
    this->currentID = other.currentID;
    this->currentSccID = other.currentSccID;
    this->currentOrder = other.currentOrder;
    this->label = other.label;
    this->labelloc = other.labelloc;
    this->components = other.components;
    return *this;
}

int DepGraph::currentID = 0;
int DepGraph::currentSccID = 0;
int DepGraph::currentOrder = 0;

NodesList DepGraph::getPredecessors(const DepGraphNode* node) {
    NodesList retMe;
	for (EdgesMapConstIterator it = edges.begin(); it != edges.end(); ++it) {
		const DepGraphNode* from = it->first;
		NodesList toList = it->second;
		NodesListIterator it2 = std::find_if(toList.begin(), toList.end(), std::bind2nd(NodeEqual(), node));
		if (it2 != toList.end()) {
			retMe.push_back(const_cast<DepGraphNode*>(from));
		}
	}
	return retMe;
}

NodesList DepGraph::getSuccessors(DepGraphNode* node){
    NodesList retMe;
	EdgesMapIterator it = edges.find(node);
	if (it != edges.end()) {
		NodesList toList = it->second;
        retMe = NodesList(toList.size());
        copy(toList.begin(), toList.end(), retMe.begin());
//		for (NodesListConstIterator it2 = toList.begin(); it2 != toList.end(); ++it2){
//			const DepGraphNode* pNode = *it2;
//			retMe.push_back(pNode);
//		}
	}
	return retMe;
}

OpNodesList DepGraph::getFuncsNodes(const std::vector<std::string> funcsNames) {
    OpNodesList retMe;
	for (NodesMapIterator it = nodes.begin(); it != nodes.end(); ++it) {
		DepGraphOpNode* opNode = dynamic_cast<DepGraphOpNode*>(it->second);
		if (opNode != NULL) {
			std::vector<std::string>::const_iterator funIt = std::find(funcsNames.begin(), funcsNames.end(), opNode->getName());
			if (funIt != funcsNames.end())
				retMe.push_back(opNode);
		}
	}
	return retMe;
}

NodesList DepGraph::getNodes() {
	NodesList retMe;
	for (NodesMapConstIterator it = nodes.begin(); it != nodes.end(); it++) {
		retMe.push_back(it->second);
	}
	return retMe;
}

UninitNodesList DepGraph::getUninitNodes() {
    UninitNodesList retMe;
//	for (NodesMapConstIterator it = nodes.begin(); it != nodes.end(); ++it) {
    for (auto nodePair : nodes){
//		const DepGraphUninitNode* uninitNode = dynamic_cast<const DepGraphUninitNode*>(it->second);
   		DepGraphUninitNode* uninitNode = dynamic_cast<DepGraphUninitNode*>(nodePair.second);
		if (uninitNode != NULL) {
			retMe.push_back(uninitNode);
		}
	}
	return retMe;
}

DepGraphUninitNode* DepGraph::findInputNode(string name){
	    for (auto nodePair : nodes){
	   		DepGraphUninitNode* uninitNode = dynamic_cast<DepGraphUninitNode*>(nodePair.second);
			if (uninitNode != NULL) {
				NodesList preds = this->getPredecessors(uninitNode);
				for (NodesListIterator it = preds.begin(); it != preds.end();it++ ) {
					DepGraphNormalNode* varNode = dynamic_cast<DepGraphNormalNode*>(*it);
					if (varNode != NULL) {
						Variable* o = dynamic_cast<Variable*>(varNode->getPlace());
						if (o != NULL ){
//							cout << "var name: " << o->getName() << endl;
							boost::smatch sm;
							boost::regex regxGraphLabel(".*(POST|GET|REQUEST)\\[(" + name + ").*");
							try {
								if (boost::regex_match(o->getName(), sm, regxGraphLabel)) {
										return uninitNode;
								}

							} catch (exception const &e) {
						        cerr << "Cannot handle var name: " << o->getName() << ". Following exception happened:\n" << e.what();
						        exit(EXIT_FAILURE);
						    }
						}
					}
				}
			}
		}
		return NULL;
}

DepGraph DepGraph::getInputRelevantGraph(DepGraphNode* inputNode) {
	DepGraph inputDepGraph(this->getRoot());
	inputDepGraph.addNode(inputNode);
	this->doGetInputRelevantGraph(inputNode, inputDepGraph);
	// we add the sorted nodes from original graph in reverse order
	// such that root will be the first node in inputDepGraph to
	// accomodate backward analysis
    inputDepGraph.sortedNodes = NodesList(this->sortedNodes.size());
    auto it = std::copy_if(this->sortedNodes.begin(), this->sortedNodes.end(),
                           inputDepGraph.sortedNodes.begin(),
                                         [&](const DepGraphNode* node) {
                                             return (inputDepGraph.containsNode(node));
                                         });
    inputDepGraph.sortedNodes.resize(std::distance(inputDepGraph.sortedNodes.begin(), it));
    reverse(inputDepGraph.sortedNodes.begin(), inputDepGraph.sortedNodes.end());
//	for (NodesConstListConstIterator it = this->sortedNodes.begin(); it != this->sortedNodes.end(); ++it){
//		const DepGraphNode* node = *it;
//		NodesMapConstIterator it2 = inputDepGraph->nodes.find(node->getID());
//		if (it2 == inputDepGraph->nodes.end())
//			inputDepGraph->sortedNodes.insert(inputDepGraph->sortedNodes.begin(), node);
//	}
	inputDepGraph.setTopLeaf(this->root);
	return inputDepGraph;
}

void DepGraph::doGetInputRelevantGraph(DepGraphNode* node,
	    			DepGraph& inputDepGraph)
{
	NodesList preds = this->getPredecessors(node);
//	for (NodesConstSetConstIterator predIt = preds.begin(); predIt != preds.end(); ++predIt ) {
//		const DepGraphNode* pred = *predIt;
    for (auto pred : preds) {
	   // if this node has already been added to the inputDepGraph graph...
	   if (inputDepGraph.containsNode(pred)) {
		inputDepGraph.addEdge(const_cast<DepGraphNode*>(pred), const_cast<DepGraphNode*>(node));
		   continue;
	   }
	   inputDepGraph.addNode(const_cast<DepGraphNode*>(pred));
	   inputDepGraph.addEdge(const_cast<DepGraphNode*>(pred), const_cast<DepGraphNode*>(node));
	   doGetInputRelevantGraph(pred, inputDepGraph);
   }
}

void DepGraph::addEdge(DepGraphNode* from, DepGraphNode* to) {
	NodesMapIterator it = nodes.find(from->getID());
	NodesMapIterator it2 = nodes.find(to->getID());
	if ( it == nodes.end() || it2 == nodes.end()) {
		throw runtime_error("Adding an edge with from/to that does not exist before");
	}
	this->edges[from].push_back(to);
}

//  *********************************************************************************

    // never add an already existing node
DepGraphNode* DepGraph::addNode(DepGraphNode* node) {
	NodesMapIterator it = nodes.find(node->getID());
	if ( it != nodes.end()) {
		throw runtime_error(stringbuilder() << "Can not add Node with ID " << node->getID() << " to dep graph. It already exists.");
	}
	this->nodes[node->getID()] = node;
	return node;
}

DepGraphNode* DepGraph::getNode(const int id){
    NodesMapConstIterator cIt = nodes.find(id);
    if (cIt == nodes.end())
        return NULL;
    else
        return cIt->second;
}
//  *********************************************************************************
bool DepGraph::containsNode(const DepGraphNode* node) {
	NodesMapConstIterator it = nodes.find(node->getID());
	return (it != nodes.end());
}

DepGraph DepGraph::parseDotFile(std::string fname){
    DepGraph depGraph;

    std::ifstream ifs;
    // This is how a node line looks like
    //  n18 [shape=box, label="/home/muath/pixy_output/test/vuln01.php : 13\nVar: $www\nFunc: _main\nID: 17, SCCID: -1, order: -1\n\n"];
    try {
        ifs.open(fname, std::ifstream::in);

        boost::smatch sm;
        boost::regex regxGraphLabel("^\\s*label=\"(.+)\";$");
        boost::regex regxGraphLabelloc("^\\s*labelloc=(.+);$");
        boost::regex regxNode("^\\s*n(\\d+)\\s\\[(.*)\\];$");
        boost::regex regxEdge("^\\s*n(\\d+)\\s->\\sn(\\d+)(\\[(.*)\\])?;$");
        //second group of regex node
        boost::regex regxNodeDescription("shape=(.+), label=\"(.+)ID: (\\d+),.*");
        //second group of regex nodeDescription
        boost::regex regxNodeUninit("<uninit>");
        boost::regex regxNodeVar("(.+) : (\\d+)\\\\nVar: (.+)\\\\nFunc: (.+)\\\\n");
        boost::regex regxNodeLit("^(.+) : (\\d+)\\\\nLit: (.*)\\\\n$");
        boost::regex regxConstant("^(.+) : (\\d+)\\\\nConst: (.*)\\\\n$");
        boost::regex regxNodeOp("^(.+) : (\\d+)\\\\n(.+):\\\\n(.+)\\\\n.*$");
        string nodeName;
        string nodeDescription;
        string edge;
        int nodeID;
        int fromNodeID;
        int toNodeID;
        int nodeSCCID;
        int nodeOrder;
        string nodeType;
        string nodeLabel;
        string nodeShape;
        string nodeFileName;
        int nodeLineNumber;
        string varName;
        string funcName;
        string litValue;
        string opName;
        bool builtinFunc;

        string inputLine;
        while (ifs.good()) {
            getline(ifs, inputLine);
//            cout << "\t parsing : " << inputLine << endl;
            if (boost::regex_match(inputLine, sm, regxGraphLabel)){
                depGraph.label = sm[1];
            }
            else if (boost::regex_match(inputLine, sm, regxGraphLabelloc)){
                depGraph.labelloc = sm[1];
            }
            else if (boost::regex_match(inputLine, sm, regxNode)){

                //process node
                nodeID = std::stoi(sm[1]) - 1;
                nodeDescription = sm[2];

                if (boost::regex_match(nodeDescription, sm, regxNodeDescription)) {
                    nodeShape = sm[1];
                    nodeLabel = sm[2];
//                    cout << "node label : " << nodeLabel << endl;
//                    nodeSCCID = std::stoi(sm[3]);
                    nodeSCCID = -1;
//                    nodeOrder = std::stoi(sm[4]);
                    nodeOrder = -1;
                    DepGraphNode* node = NULL;
                    if (boost::regex_match(nodeLabel, sm, regxNodeUninit)){
                        node = new DepGraphUninitNode(nodeID, nodeOrder, nodeSCCID);
                        depGraph.addNode(node);
                    }
                    else if (boost::regex_match(nodeLabel, sm, regxNodeVar)){

                        nodeFileName = sm[1];
                        nodeLineNumber = std::stoi(sm[2]);
                        varName = sm[3];
                        funcName = sm[4];
                        TacPlace* place = new Variable(varName, funcName);
                        node = new DepGraphNormalNode(nodeFileName, nodeLineNumber, nodeID, nodeSCCID, nodeOrder, place);
                        depGraph.addNode(node);

                    }else if (boost::regex_match(nodeLabel, sm, regxNodeLit)){
                        nodeFileName = sm[1];
                        nodeLineNumber = std::stoi(sm[2]);
                        litValue = sm[3];
                        TacPlace* place = new Literal(litValue);
                        node = new DepGraphNormalNode(nodeFileName, nodeLineNumber, nodeID, nodeSCCID, nodeOrder, place);
                        depGraph.addNode(node);
                    }else if(boost::regex_match(nodeLabel, sm, regxConstant)){
                    	nodeFileName = sm[1];
                    	nodeLineNumber = std::stoi(sm[2]);
                    	string constValue = sm[3];
                    	TacPlace* place = new Constant(constValue);
                    	node = new DepGraphNormalNode(nodeFileName, nodeLineNumber, nodeID, nodeSCCID, nodeOrder, place);
                    	depGraph.addNode(node);

                    } else if (boost::regex_match(nodeLabel, sm, regxNodeOp)){

                        nodeFileName = sm[1];
                        nodeLineNumber = std::stoi(sm[2]);
                        builtinFunc = (sm[3] == "builtin function");
                        opName = sm[4];
                        node = new DepGraphOpNode(nodeFileName, nodeLineNumber, nodeID, nodeSCCID, nodeOrder, opName, builtinFunc);
                        depGraph.addNode(node);

                    }
                    else {
                        throw invalid_argument("error parsing the dependency graph dot file. Can not parse node label (type)");
                    }
                    DepGraphNormalNode* root;
                    if (nodeShape == "doubleoctagon" && (root = dynamic_cast<DepGraphNormalNode*>(node)) != NULL )
                        depGraph.setRoot(root);
                    node->setShape(nodeShape);
                } else {
                    throw invalid_argument("error parsing the dependency graph dot file. Can not parse node description");
                }
            } else if (boost::regex_match(inputLine, sm, regxEdge)) {
                //process edge
                fromNodeID = std::stoi(sm[1]) - 1;
                toNodeID = std::stoi(sm[2]) - 1;
                DepGraphNode* fromNode = depGraph.getNode(fromNodeID);
                DepGraphNode* toNode = depGraph.getNode(toNodeID);
                depGraph.addEdge(fromNode, toNode);
            }
        }
        ifs.close();
        
    } catch (exception const &e) {
        cerr << "Can not construct depGraph from file " << fname << ". Following exception happened:\n" << e.what();
        if (ifs.is_open())
            ifs.close();
        exit(EXIT_FAILURE);
    }
    return depGraph;
}

std::string DepGraph::toDot(){
    std::stringstream ss;
    NodesMapConstIterator citNodes;
    EdgesMapConstIterator citFromNode;
    NodesListIterator citToNode;
    ss << "digraph cfg {\n"
       << "  label=\"" << DepGraphNode::escapeDot(this->label, 0) << "\";\n"
       << "  labelloc=" << DepGraphNode::escapeDot(this->labelloc, 0) << ";\n";
    
    for (auto citNodes : nodes){
        ss << "  n" << citNodes.first
           << " "
           << "[shape=" << citNodes.second->getShape() << ", "
           << "label=\""
           << citNodes.second->dotName() << "\\n"
           << "ID: " << citNodes.first << ", "
           << "SCCID: " << citNodes.second->getSccID() << ", "
           << "order: " << citNodes.second->getOrder() << "\\n\\n"
           << "\"];\n";
    }
    
    for (citFromNode = edges.begin(); citFromNode != edges.end(); ++citFromNode){
        const DepGraphNode* fromNode = citFromNode->first;
        NodesList toNodes = citFromNode->second;
        for (citToNode = toNodes.begin(); citToNode != toNodes.end(); ++citToNode){
            DepGraphNode* toNode = *citToNode;
            ss  << "  "
                << "n" << fromNode->getID()
                << " -> "
                << "n" << toNode->getID()
                << ";\n";
        }
    }
    ss << "}";
    return ss.str();
}

void DepGraph::dumpDot(string fname){
    ofstream ofs;
    string content;
    try {
        ofs.open(fname, std::ofstream::out);
        content = this->toDot();
        if (ofs.good())
            ofs << content;
        ofs.close();
    } catch (exception & e) {
        cerr << "Can not open a file to dump the depgraph into." << e.what();
        if (ofs.is_open())
            ofs.close();
    }
}
//  ********************************************************************************
    // sort the graph topologically
void DepGraph::sort(DepGraph& origGraph){

//        // Get strongly connected components of the graph
//    	vector<NodesList> sccs = this.getSccs();
//    	// first a map between each scc and its nodes
//        Map<DepGraphSccNode, List<DepGraphNode>> sccNodes = new HashMap<DepGraphSccNode, List<DepGraphNode>>();
//    	// for each scc set an ID
//        currentSccID = 0;
//        // eleminate cycles from current graph before sorting
//        for (List<DepGraphNode> scc : sccs) {
//
//            // one-element sccs are no problem
//            if (scc.size() < 2) {
//                continue;
//            }
//
//
//            // determine edges pointing into this SCC
//            Set<DepGraphNode> sccPredecessors = new HashSet<DepGraphNode>();
//            for (DepGraphNode sccMember : scc) {
//            	sccMember.setSccID(currentSccID);
//                Set<DepGraphNode> predecessors = this.getPredecessors(sccMember);
//                predecessors.removeAll(scc);  // don't take predecessors that are inside the SCC
//                sccPredecessors.addAll(predecessors);
//            }
//
//            // determine edges going out of this SCC
//            Set<DepGraphNode> sccSuccessors = new HashSet<DepGraphNode>();
//            for (DepGraphNode sccMember : scc) {
//                List<DepGraphNode> successors = this.getSuccessors(sccMember);
//                successors.removeAll(scc);  // don't take predecessors that are inside the SCC
//                sccSuccessors.addAll(successors);
//            }
//
//            // remove scc members
//            for (DepGraphNode sccMember : scc) {
//                //this.remove(sccMember, new HashSet<DepGraphNode>());
//                this.nodes.remove(sccMember);
//                this.edges.remove(sccMember);
//            }
//
//            // the replacement node
//            DepGraphSccNode sccNode = new DepGraphSccNode();
//            // TODO: you may get the wrong ID if this sorting has been done after generating
//            // another new graph
//            sccNode.setID(DepGraph.currentID++);
//            sccNode.setSccID(currentSccID);
//            sccNode.setOrder(-1);
//            sccNodes.put(sccNode, scc);
//            this.addNode(sccNode);
//
//
//
//            // adjust nodes coming in to the SCC
//            for (DepGraphNode pre : sccPredecessors) {
//                // remove stale nodes from the out-list
//                List<DepGraphNode> out = this.edges.get(pre);
//                for (Iterator iter = out.iterator(); iter.hasNext();) {
//                    DepGraphNode outNode = (DepGraphNode) iter.next();
//                    if (!this.nodes.containsKey(outNode)) {
//                        iter.remove();
//                    }
//                }
//                // add new replacement node to the out-list
//                out.add(sccNode);
//            }
//
//            // adjust nodes going out of the SCC
//            this.edges.put(sccNode, new LinkedList<DepGraphNode>(sccSuccessors));
//
//            currentSccID++;
//            // done!
//
//        }

        // Now do a breadth first search with a work queue and assign each node an order.
        // The nodes are going to be inserted in sorted list such that the root will be
        // the last node and one of the leaf nodes the first

        // prepare the list of sorted nodes. this list should include the nodes sorted
        // topologically. for each scc there would be only one node which is
        // the node in scc with the next successor in topological order

        // workqueue is used for the breadth first search algorithm
    	NodesList workQueue;
    	// avoid sorting nodes which has been already been sorted
    	NodesList visited;
        currentOrder = 0;
        DepGraphNode* root = this->getRoot();
        workQueue.push_back(root);
        while (!workQueue.empty()){
        	topLeaf = *(workQueue.begin());
            workQueue.erase(workQueue.begin());
        	// make sure all parents are visited and sorted before this node
        	bool allParentsVisited = true;
//        	for (NodesConstSetConstIterator it = this->getPredecessors(topLeaf).begin(); it != this->getPredecessors(topLeaf).end(); ++it){
//                const DepGraphNode* node = *it;
            for (auto node : this->getPredecessors(topLeaf)){
        		if (node->getOrder() == -1){
        			allParentsVisited = false;
        			break;
        		}
        	}
        	if (!allParentsVisited){
        		workQueue.push_back(topLeaf);
        	}
        	else {
        		sortedNodes.insert(sortedNodes.begin(), topLeaf);
        		topLeaf->setOrder(currentOrder);
//        		// if scc node then set order for original ones
//        		if (topLeaf instanceof DepGraphSccNode){
//        			for (DepGraphNode* node: sccNodes.get(topLeaf))
//        				node.setOrder(currentOrder);
//        		}
        		currentOrder++;
        	}
//        	for (NodesConstListConstIterator it = this->getSuccessors(topLeaf).begin(); it != this->getSuccessors(topLeaf).end(); ++it){
//                const DepGraphNode* node = *it;
            for (auto node : this->getSuccessors(topLeaf)){
                NodesListConstIterator it2;
                //if node is not visited
        		if ((it2 = std::find_if(visited.begin(), visited.end(), std::bind2nd(NodeEqual(), node))) == visited.end())
                {
        			workQueue.push_back(node);
        			visited.push_back(node);
        		}
        	}
        }
        // TODO: if topLeaf is an SCC node then this will not work
        origGraph.topLeaf = topLeaf;
    }

    //  ********************************************************************************

    // returns a list of strongly connected components;
    // uses the algorithm from "The Design and Analysis of Computer Algorithms"
    // (Aho, Hopcroft, Ullman), Chapter 5.5 ("Strong Connectivity")
//    vector<NodesList> DepGraph::getSccs() const {
//        n = 1;
//        List<List<DepGraphNode>> sccs = new LinkedList<List<DepGraphNode>>();
//        List<DepGraphNode> stack = new LinkedList<DepGraphNode>();
//        Map<DepGraphNode,Integer> dfsnum = new HashMap<DepGraphNode,Integer>();
//        Map<DepGraphNode,Integer> low = new HashMap<DepGraphNode,Integer>();
//        Set<DepGraphNode> old = new HashSet<DepGraphNode>();
//        sccVisit(this.root, stack, dfsnum, low, old, sccs);
//        return sccs;
//    }

//  ********************************************************************************

    // helper function for SCC computation
//    void DepGraph::sccVisit(DepGraphNode* v, NodesList stack,
//            map<const DepGraphNode*,int> dfsnum,
//            map<const DepGraphNode*,int> low,
//            NodesSet old,
//            vector<NodesList> sccs) const {
//
//        old.add(v);
//        dfsnum.put(v, n);
//        low.put(v, n);
//        n++;
//        stack.add(v);
//
//        for (DepGraphNode w : this.getSuccessors(v)) {
//            if (!old.contains(w)) {
//                sccVisit(w, stack, dfsnum, low, old, sccs);
//                int low_v = low.get(v);
//                int low_w = low.get(w);
//                low.put(v, Math.min(low_v, low_w));
//            } else {
//                int dfsnum_v = dfsnum.get(v);
//                int dfsnum_w = dfsnum.get(w);
//                if (dfsnum_w < dfsnum_v && stack.contains(w)) {
//                    int low_v = low.get(v);
//                    low.put(v, Math.min(dfsnum_w, low_v));
//                }
//            }
//        }
//
//        if (low.get(v).equals(dfsnum.get(v))) {
//            List<DepGraphNode> scc = new LinkedList<DepGraphNode>();
//            DepGraphNode x;
//            do {
//                x = stack.remove(stack.size() - 1);
//                scc.add(x);
//            } while (!x.equals(v));
//            sccs.add(scc);
//        }
//    }

//vector<vector<int> > DepGraph::getSccs() {
//    int n = getNumOfNodes();
//    lowlink = new int[n];
//    used = new bool[n];
//
//    for (int u = 0; u < n; u++)
//      if (!used[u])
//    	  dfsSCC(u);
//
//    return components;
//}
//
//void DepGraph::dfsSCC(int u) {
//    lowlink[u] = time++;
//    used[u] = true;
//    process_stack.push(u);
//    bool is_component_root = true;
//
//    for (int v : graph[u]) {
//      if (!used[v])
//    	  dfsSCC(v);
//      if (lowlink[u] > lowlink[v]) {
//        lowlink[u] = lowlink[v];
//        is_component_root = false;
//      }
//    }
//
//    if (is_component_root) {
//      List<Integer> component = new ArrayList<>();
//      while (true) {
//        int k = stack.remove(stack.size() - 1);
//        component.add(k);
//        lowlink[k] = Integer.MAX_VALUE;
//        if (k == u)
//          break;
//      }
//      components.add(component);
//    }
//}


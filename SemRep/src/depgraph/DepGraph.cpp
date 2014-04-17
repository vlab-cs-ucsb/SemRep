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
    root = nullptr;
    topLeaf = nullptr;
    label = "";
    labelloc = "";

}

DepGraph::DepGraph(const DepGraph& other){
    this->root = other.root;
    this->nodes = other.nodes;
    this->topLeaf = other.topLeaf;
    this->edges = other.edges;
    this->currentID = other.currentID;
    this->currentSccID = other.currentSccID;
    this->currentOrder = other.currentOrder;
    this->label = other.label;
    this->labelloc = other.labelloc;
    this->scc_components = other.scc_components;
    this->scc_map = other.scc_map;
}

DepGraph::~DepGraph() {

}

DepGraph& DepGraph::operator=(const DepGraph &other){
    this->root = other.root;
    this->nodes = other.nodes;
    this->topLeaf = other.topLeaf;
    this->edges = other.edges;
    this->currentID = other.currentID;
    this->currentSccID = other.currentSccID;
    this->currentOrder = other.currentOrder;
    this->label = other.label;
    this->labelloc = other.labelloc;
    this->scc_components = other.scc_components;
    this->scc_map = other.scc_map;
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
	inputDepGraph.setTopLeaf(this->root);
	return inputDepGraph;
}

void DepGraph::doGetInputRelevantGraph(DepGraphNode* node,
	    			DepGraph& inputDepGraph)
{
	NodesList preds = this->getPredecessors(node);

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

DepGraph DepGraph::parseDotFile(std::string fname) {
    DepGraph depGraph;

    std::ifstream ifs;
    // This is how a node line looks like
    //  n18 [shape=box, label="/home/muath/pixy_output/test/vuln01.php : 13\nVar: $www\nFunc: _main\nID: 17, SCCID: -1, order: -1\n\n"];
    try {
        ifs.open(fname, std::ifstream::in);

        boost::smatch sm;
        boost::regex regxNode("^\\s*n(\\d+)\\s\\[(.*)\\];$");
        boost::regex regxEdge("^\\s*n(\\d+)\\s->\\sn(\\d+)(\\[(.*)\\])?;$");
        //second group of regex node
        boost::regex regxNodeDescription("shape=(.+), label=\"(.+)\"");
        //second group of regex nodeDescription
        boost::regex regxNodeUninit("Input: (.+)");
        boost::regex regxNodeReturn("Return: (.+)");
        boost::regex regxNodeVar("Var: (.+)");
        boost::regex regxNodeLit("Lit: (.*)");
        boost::regex regxNodeOp("(.+)");
        string nodeName;
        string nodeDescription;
        string edge;
        int nodeID;
        int fromNodeID;
        int toNodeID;
        string nodeType;
        string nodeLabel;
        string nodeShape;
        string varName;
        string litValue;
        string opName;

        string inputLine;
        while (ifs.good()) {
            getline(ifs, inputLine);
//            cout << "\t parsing : " << inputLine << endl;
            if (boost::regex_match(inputLine, sm, regxNode)){

                //process node
                nodeID = std::stoi(sm[1]) - 1;
                nodeDescription = sm[2];

                if (boost::regex_match(nodeDescription, sm, regxNodeDescription)) {
                    nodeShape = sm[1];
                    nodeLabel = sm[2];
                    DepGraphNode* node = NULL;
                    if (boost::regex_match(nodeLabel, sm, regxNodeUninit)){
                        node = new DepGraphUninitNode(nodeID, -1, -1);
                        depGraph.addNode(node);
                    }
                    else if (boost::regex_match(nodeLabel, sm, regxNodeVar)){
                        varName = sm[1];
                        TacPlace* place = new Variable(varName, "noFunc");
                        node = new DepGraphNormalNode("noFile", -1, nodeID, -1, -1, place);
                        depGraph.addNode(node);
                    }else if (boost::regex_match(nodeLabel, sm, regxNodeLit)){
                        litValue = sm[1];
                        boost::regex bsDQuote("\\\"");//this will match \"
                        string newStr = "\"";
                        litValue = boost::regex_replace(litValue, bsDQuote, newStr);
                        boost::regex bsBs("\\\\");//this will match \"
                        newStr = "\\";
                        litValue = boost::regex_replace(litValue, bsBs, newStr);
                        TacPlace* place = new Literal(litValue);
                        node = new DepGraphNormalNode("noFile", -1, nodeID, -1, -1, place);
                        depGraph.addNode(node);
                    } else if (boost::regex_match(nodeLabel, sm, regxNodeOp)){
                        opName = sm[1];
                        node = new DepGraphOpNode("noFile", -1, nodeID, -1, -1, opName, false);
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

    depGraph.calculateSCCs();

    return depGraph;
}

DepGraph DepGraph::parsePixyDotFile(std::string fname) {
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

    depGraph.calculateSCCs();

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

void DepGraph::calculateSCCs() {
	int time_count;
	map<int, int> lowlink;
	map<int, bool> used;
	stack<int> process_stack;

    for (auto node_ptr : getNodes()) {
      if (!used[node_ptr->getID()]) {
    	  dfsSCC(node_ptr, time_count, lowlink, used, process_stack);
      }
    }

//    printSCCInfo();

    return;
}

void DepGraph::dfsSCC(DepGraphNode* node, int& time_count, map<int, int>& lowlink, map<int, bool>& used, stack<int>& process_stack) {
	int u = node->getID();
	lowlink[u] = time_count++;
	used[u] = true;
	process_stack.push(u);
	bool is_component_root = true;

	for (auto succ_node : getSuccessors(node)) {
		int v = succ_node->getID();
		if (!used[v])
			dfsSCC(succ_node, time_count, lowlink, used, process_stack);
		if (lowlink[u] > lowlink[v]) {
			lowlink[u] = lowlink[v];
			is_component_root = false;
		}
	}

	if (is_component_root) {
		int k;
		vector<DepGraphNode* > scc_component;
		while (true) {
			k = process_stack.top();
			process_stack.pop();
			scc_component.push_back(getNode(k));
			scc_map[k] = u;
			lowlink[k] = INT_MAX;
			if (k == u)
				break;
		}
		// only keep the scc nodes that has more than 1 element
		if (scc_component.size() > 1) {
			scc_components[u] = scc_component;
		} else {
			scc_map.erase(k);
		}
	}
}

bool DepGraph::isSCCElement(DepGraphNode* node) {
	return scc_map.find(node->getID()) != scc_map.end();
}

int DepGraph::getSCCID(DepGraphNode* node) {
	return scc_map[node->getID()];
}
NodesList DepGraph::getSCCNodes(int scc_id) {
	return scc_components[scc_id];
}

NodesList DepGraph::getSCCNodes(DepGraphNode* node) {
	return scc_components[getSCCID(node)];
}

void DepGraph::printSCCInfo() {
    cout << "components: "<< endl;
    for (auto& it : scc_components) {
    	cout << "u: " << it.first << " =>  ";
    	for (auto& c : it.second ) {
    		cout << c->getID() << " ";
    	}
    	cout << endl;
    }
    cout << "------------------" << endl;

    cout << "scc map: " << endl;
    for (auto& it : scc_map) {
    	cout << it.first << " => " << it.second << " ";
    }
    cout << endl << "------------------" << endl;
}


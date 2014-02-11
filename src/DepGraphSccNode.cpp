/*
 * DepGraphSccNode.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#include "DepGraphSccNode.hpp"

DepGraphSccNode::DepGraphSccNode() {
	// TODO Auto-generated constructor stub

}

DepGraphSccNode::~DepGraphSccNode() {
	// TODO Auto-generated destructor stub
}

// returns a name that can be used in dot file representation
std::string DepGraphSccNode::dotName() const {
	return "SCC";
}

std::string DepGraphSccNode::comparableName() const {
	return dotName();
}

std::string DepGraphSccNode::dotNameShort() const {
	return dotName();
}

std::string DepGraphSccNode::dotNameVerbose(bool isModelled) const {
    return dotName().append(DepGraphNode::dotNameVerbose(isModelled));
}

int DepGraphSccNode::getLine() const {
    return -1;
}

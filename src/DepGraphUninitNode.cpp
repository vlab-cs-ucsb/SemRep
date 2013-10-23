/*
 * DepGraphUninitNode.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#include "DepGraphUninitNode.h"

DepGraphUninitNode::DepGraphUninitNode() {
	// TODO Auto-generated constructor stub

}

DepGraphUninitNode::~DepGraphUninitNode() {
	// TODO Auto-generated destructor stub
}

// returns a name that can be used in dot file representation
std::string DepGraphUninitNode::dotName() const {
	return "<uninit>";
}

std::string DepGraphUninitNode::comparableName() const {
	return dotName();
}

std::string DepGraphUninitNode::dotNameShort() const {
	return dotName();
}

std::string DepGraphUninitNode::dotNameVerbose(bool isModelled) const {
    return dotName().append(DepGraphNode::dotNameVerbose(isModelled));
}

int DepGraphUninitNode::getLine() const {
    return -1;
}

/*
 * DepGraphNode.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#include "DepGraphNode.hpp"


void DepGraphNode::init() {
    // Node ID
    id = -1;
    // topological sort node
    order = -1;
    // strongly connected component id
    sccID = -1;
    // tells number of parents which has not been processed in computation yet
    nonprocessedParents = -1;
    
    shape = "";
}

DepGraphNode::~DepGraphNode() {
	// TODO Auto-generated destructor stub
}

int DepGraphNode::getNonprocessedParents() const {
	return nonprocessedParents;
}
void DepGraphNode::setNonprocessedParents(int nonprocessedParents) {
	this->nonprocessedParents = nonprocessedParents;
}
void DepGraphNode::setID(int id){
	this->id = id;
}
int DepGraphNode::getID() const {
	return id;
}
void DepGraphNode::setOrder(int order){
	this->order = order;
}
int DepGraphNode::getOrder() const {
	return order;
}
void DepGraphNode::setSccID(int sccID){
	this->sccID = sccID;
}
int DepGraphNode::getSccID() const {
	return sccID;
}
bool DepGraphNode::equals(const DepGraphNode* node) const{
	return this->id == node->id;
}


std::string DepGraphNode::dotNameVerbose(bool isModelled) const {
	return stringbuilder() << "ID: "  << id << ", SCCID: " << sccID << ", order: " << order << "\\n";
}

std::string DepGraphNode::escapeDot(const std::string escapeMe, const int limit) {
        if (limit > 0 && escapeMe.length() > limit) {
            return "...";
        }
        std::string escaped = escapeMe;
        for (int i = 0; i < escaped.length(); i++) {
            char inspectMe = escaped[i];
            if (inspectMe == '\n' || inspectMe == '\r') {
                // delete these control characters
                escaped.erase(escaped.begin()+i);
                i--;
            } else if (inspectMe == '"' || inspectMe == '\\') {
                // escape this character by prefixing it with a backslash
                escaped.insert(i,1, '\\');
                i++;
            }
        }
        return escaped;
}

/*
 * DepGraphNode.cpp
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

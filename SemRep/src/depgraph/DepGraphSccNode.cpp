/*
 * DepGraphSccNode.cpp
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

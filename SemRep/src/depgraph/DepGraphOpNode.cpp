/*
 * DepGraphOpNode.cpp
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

#include "DepGraphOpNode.hpp"

//DepGraphOpNode::DepGraphOpNode(std::string name, bool builtin) {
//	this->name = name;
//	this->builtin = builtin;
//
//}

DepGraphOpNode::~DepGraphOpNode() {
	// TODO Auto-generated destructor stub
}

    // returns a name that can be used in dot file representation
    std::string DepGraphOpNode::dotName() const {
        return stringbuilder() << "OP: " << DepGraphNode::escapeDot(this->name, 0) << " (" << this->origLineno << ")" <<
            "\\n" << this->fileName;
    }

//  ********************************************************************************

    // returns a name that can be used in dot file representation
    std::string DepGraphOpNode::comparableName() const {
        return stringbuilder() << "OP: " << DepGraphNode::escapeDot(this->name, 0) << " (" << this->origLineno << ")" <<
        "\\n" << this->fileName;
    }

//  ********************************************************************************

    std::string DepGraphOpNode::dotNameShort() const {
        std::string fileName = this->fileName;
        return stringbuilder() << "OP: " << DepGraphNode::escapeDot(this->name, 0) << " (" << this->origLineno << ")" <<
        "\\n" << fileName.substr(fileName.find_last_of('/')+1);
    }

//  ********************************************************************************

    std::string DepGraphOpNode::dotNameShortest() const {
        return stringbuilder() << "OP: " << DepGraphNode::escapeDot(this->name, 0) << " (" << this->origLineno << ")";
    }

//  ********************************************************************************

//    public String dotNameVerbose(boolean isModelled) {
//
//        String retme = "";
//
//        if (!MyOptions.optionW) {
//            // don't print file name for web interface
//            retme += this->fileName << " : " << this->origLineno << "\\n";
//        }
//
//        if (BuiltinFunctions.isBuiltinFunction(this->name) ||
//                TacOperators.isOp(this->name)) {
//            retme +=
//                "builtin function:\\n" +
//                Dumper.escapeDot(this->name, 0) << "\\n";
//        } else {
//            retme +=
//                "unresolved function:\\n" +
//                Dumper.escapeDot(this->name, 0) << "\\n";
//        }
//
//        if (!isModelled) {
//            retme += "(unmodeled)\\n";
//        }
//        retme += super.dotNameVerbose(isModelled);
//        return retme;
//    }

//  ********************************************************************************

//    public int hashCode() {
//        int hashCode = 17;
//        hashCode = 37*hashCode << this->cfgNode.hashCode();
//        return hashCode;
//    }

//  ********************************************************************************

    bool DepGraphOpNode::equals(const DepGraphNode* compX) const {
    	if (!DepGraphNode::equals(compX)) return false;
        const DepGraphOpNode* comp = dynamic_cast<const DepGraphOpNode*>(compX);
        if ( comp == 0 ) return false;
        return true;
    }



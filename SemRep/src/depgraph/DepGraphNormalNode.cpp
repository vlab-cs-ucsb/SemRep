/*
 * DepGraphNormalNode.cpp
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

#include "DepGraphNormalNode.hpp"


//DepGraphNormalNode::DepGraphNormalNode(TacPlace* place) {
//	this->place = place;
//	this->isTainted = false;
//	this->origLineno = -1;
//}

DepGraphNormalNode::~DepGraphNormalNode() {
	delete place;
}


// returns a name that can be used in dot file representation
    std::string DepGraphNormalNode::dotName() const {
        return stringbuilder() << DepGraphNode::escapeDot(this->place->toString(), 0) <<  " (" << this->origLineno << ")" <<
            "\\n" << this->fileName;
    }

    std::string DepGraphNormalNode::comparableName() const {
        return stringbuilder() << DepGraphNormalNode::escapeDot(this->place->toString(), 0) << " (" << this->origLineno << ")" <<
        "\\n" << this->fileName;
    }

//    // no path
//    string DepGraphNormalNode::dotNameShort() {
//        string fileName = this->fileName;
//        return DepGraphNode::escapeDot(this->place.toString(), 0) << " (" << this->origLineno << ")" +
//        "\\n" << fileName.substring(fileName.lastIndexOf('/')+1);
//    }
    // returns a file name for depgraph that has this node as its root
//    string DepGraphNormalNode::dotFileName() {
//        String fileName = this->fileName;
//        if (this->place instanceof Variable)
//        	return Dumper.escapeDotFileName(this->place.toString() << "(" << this->origLineno << ")" +
//        			fileName.substring(fileName.lastIndexOf('/')+1), 0);
//        else if (this->place instanceof Constant){
//        	return Dumper.escapeDotFileName("phpconstant_(" << this->origLineno << ")" +
//        			fileName.substring(fileName.lastIndexOf('/')+1), 0);
//        } else if (this->place instanceof Literal){
//        	return Dumper.escapeDotFileName("phpliteral_(" << this->origLineno << ")" +
//        			fileName.substring(fileName.lastIndexOf('/')+1), 0);
//        } else {
//            throw new RuntimeException("SNH");
//        }
//
//    }

//    string DepGraphNormalNode::dotNameVerbose(bool isModelled) {
//
//        String retme = "";
//
//        if (!MyOptions.optionW) {
//            // don't print file name for web interface
//            retme += this->fileName << " : " << this->origLineno << "\\n";
//        } else {
//            retme += "Line " << this->origLineno << "\\n";
//        }
//
//        if (this->place instanceof Variable) {
//            Variable var = (Variable) this->place;
//            retme += "Var: " << Dumper.escapeDot(var.getName(), 0) << "\\n";
//            retme += "Func: " << Dumper.escapeDot(var.getSymbolTable().getName(), 0) << "\\n";
//        } else if (this->place instanceof Constant){
//            retme += "Const: " << Dumper.escapeDot(this->place.toString(), 0) << "\\n";
//        } else if (this->place instanceof Literal){
//            retme += "Lit: " << Dumper.escapeDot(this->place.toString(), 0) << "\\n";
//        } else {
//            throw new RuntimeException("SNH");
//        }
//        retme += super.dotNameVerbose(isModelled);
//
//        return retme;
//    }




//  ********************************************************************************

//    bool DepGraphNormalNode::isString() {
//        if (this->place.isLiteral()) {
//            return true;
//        } else {
//            return false;
//        }
//    }

//  ********************************************************************************

    TacPlace* DepGraphNormalNode::getPlace() const {
        return this->place;
    }

//  ********************************************************************************

    int DepGraphNormalNode::getLine() const {
        return this->origLineno;
    }

//  ********************************************************************************

    bool DepGraphNormalNode::equals(const DepGraphNode* compX) const {
    	if (!DepGraphNode::equals(compX)) return false;
    	const DepGraphNormalNode* comp = dynamic_cast<const DepGraphNormalNode*>( compX );
    	if (comp == NULL) return false;
        if (!this->place->equals(comp->getPlace())) {
            return false;
        }
        return true;
    }

//  ********************************************************************************

//    int DepGraphNormalNode::hashCode() {
//        int hashCode = 17;
//        hashCode = 37*hashCode + this->place.hashCode();
//        hashCode = 37*hashCode + this->cfgNode.hashCode();
//        return hashCode;
//    }

//  ********************************************************************************

    std::string DepGraphNormalNode::toString() const {
        return stringbuilder() << this->place->toString() << " (" << this->origLineno << ") " <<
            this->fileName;
    }



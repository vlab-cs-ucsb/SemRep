/*
 * DepGraphOpNode.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#include "DepGraphOpNode.h"

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



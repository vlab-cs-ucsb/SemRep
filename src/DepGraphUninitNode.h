/*
 * DepGraphUninitNode.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef DEPGRAPHUNINITNODE_H_
#define DEPGRAPHUNINITNODE_H_

#include "DepGraphNode.h"

class DepGraphUninitNode: public DepGraphNode {
public:
    DepGraphUninitNode();
	DepGraphUninitNode(int id, int order, int sccID) : DepGraphNode("", -1, id, order, sccID){ };
	DepGraphUninitNode(const DepGraphUninitNode& other)
			:  DepGraphNode(other) {	};
	virtual ~DepGraphUninitNode();
	std::string dotName() const;
	std::string comparableName() const;
	std::string dotNameShort() const;
	std::string dotNameVerbose(bool isModelled) const;
	int getLine() const;
	DepGraphUninitNode* clone() const { return new DepGraphUninitNode(*this); };
};

#endif /* DEPGRAPHUNINITNODE_H_ */

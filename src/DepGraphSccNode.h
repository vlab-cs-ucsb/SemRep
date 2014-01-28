/*
 * DepGraphSccNode.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef DEPGRAPHSCCNODE_H_
#define DEPGRAPHSCCNODE_H_

#include "DepGraphNode.h"


class DepGraphSccNode: public DepGraphNode {
public:
	DepGraphSccNode();
	DepGraphSccNode(const DepGraphSccNode& other)
				: DepGraphNode(other) {	};
	virtual ~DepGraphSccNode();
	std::string dotName() const;
	std::string comparableName() const;
	std::string dotNameShort() const;
	std::string dotNameVerbose(bool isModelled) const;
	int getLine() const;
	DepGraphSccNode* clone() const { return new DepGraphSccNode(*this); };
};

#endif /* DEPGRAPHSCCNODE_H_ */

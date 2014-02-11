/*
 * DepGraphSccNode.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef DEPGRAPHSCCNODE_HPP_
#define DEPGRAPHSCCNODE_HPP_

#include "DepGraphNode.hpp"


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


#endif /* DEPGRAPHSCCNODE_HPP_ */

/*
 * DepGraphUninitNode.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef DEPGRAPHUNINITNODE_HPP_
#define DEPGRAPHUNINITNODE_HPP_

#include "DepGraphNode.hpp"

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


#endif /* DEPGRAPHUNINITNODE_HPP_ */

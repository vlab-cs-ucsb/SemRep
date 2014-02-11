/*
 * DepGraphOpNode.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef DEPGRAPHOPNODE_HPP_
#define DEPGRAPHOPNODE_HPP_

#include "DepGraphNode.hpp"

class DepGraphOpNode: public DepGraphNode {
public:
	DepGraphOpNode(std::string filename, int origLineno, int id, int order, int sccID, std::string opname, bool builtin) : DepGraphNode(filename, origLineno, id, order, sccID), name(opname), builtin(builtin){};
	DepGraphOpNode(const DepGraphOpNode& other)
			: DepGraphNode(other), name(other.name), builtin(other.builtin) {	};
	virtual ~DepGraphOpNode();
	std::string dotNameShortest() const;
	std::string getName() const {return this->name;};
	bool isBuiltin() const {return this->builtin;};
	bool equals (const DepGraphNode* compX) const;
	std::string dotName() const;
	std::string comparableName() const;
	std::string dotNameShort() const;
	DepGraphOpNode* clone() const { return new DepGraphOpNode(*this); };

private:
    std::string name;
	bool builtin;    // builtin function?

};


#endif /* DEPGRAPHOPNODE_HPP_ */

/*
 * DepGraphOpNode.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef DEPGRAPHOPNODE_H_
#define DEPGRAPHOPNODE_H_

#include "DepGraphNode.h"
#include "StringBuilder.h"


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

#endif /* DEPGRAPHOPNODE_H_ */

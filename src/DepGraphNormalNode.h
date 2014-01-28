/*
 * DepGraphNormalNode.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef DEPGRAPHNORMALNODE_H_
#define DEPGRAPHNORMALNODE_H_
#include "DepGraphNode.h"
#include "StringBuilder.h"
#include "TacPlace.h"
#include "Variable.h"
#include "Literal.h"
#include "Constant.h"


class DepGraphNormalNode: public DepGraphNode {
public:
	DepGraphNormalNode(std::string filename, int origLineno, int id, int order, int sccID, TacPlace* place) : DepGraphNode(filename, origLineno, id, order, sccID), place(place)
    {
        isTainted = false;
    };
    
	DepGraphNormalNode(const DepGraphNormalNode& other)
		: DepGraphNode(other), isTainted(other.isTainted){
		place = other.place->clone();
	};
	virtual ~DepGraphNormalNode();
	std::string dotName() const;
	std::string comparableName() const;
	std::string dotNameShort() const {return "";};
	std::string toString() const;
    int getLine() const;
    TacPlace* getPlace() const;
    bool equals(const DepGraphNode* comp) const;
    DepGraphNormalNode* clone() const { return new DepGraphNormalNode(*this); };

private:
	TacPlace* place;
	bool isTainted;


};

#endif /* DEPGRAPHNORMALNODE_H_ */

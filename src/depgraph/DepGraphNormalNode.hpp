/*
 * DepGraphNormalNode.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef DEPGRAPHNORMALNODE_HPP_
#define DEPGRAPHNORMALNODE_HPP_

#include "DepGraphNode.hpp"
#include "TacPlace.hpp"
#include "Variable.hpp"
#include "Literal.hpp"
#include "Constant.hpp"


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


#endif /* DEPGRAPHNORMALNODE_HPP_ */

/*
 * DepGraphNormalNode.hpp
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

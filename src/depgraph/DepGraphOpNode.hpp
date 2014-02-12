/*
 * DepGraphOpNode.hpp
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

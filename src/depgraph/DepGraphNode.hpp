/*
 * DepGraphNode.hpp
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

#ifndef DEPGRAPHNODE_HPP_
#define DEPGRAPHNODE_HPP_

#include "../StringBuilder.hpp"
#include <string>
#include <stdexcept>

class DepGraphNode {
public:
    DepGraphNode()
    {
        init();
    };

    DepGraphNode(std::string filename, int origLineno, int id, int order, int sccID)
    : fileName(filename), origLineno(origLineno), id(id), order(order), sccID(sccID)
    {
        nonprocessedParents = -1; shape = "";
    };

	DepGraphNode(const DepGraphNode& other)
		: fileName(other.fileName), origLineno(other.origLineno), id(other.id), order(other.order),
		  sccID(other.sccID), nonprocessedParents(other.nonprocessedParents), shape(other.shape)
    {
    };

	int getNonprocessedParents() const;
	void setNonprocessedParents(int nonprocessedParents);
	void setID(int id);
	int getID() const;
	void setOrder(int order);
	int getOrder() const;
	void setSccID(int sccID);
	int getSccID() const;
    std::string getFileName() const { return this->fileName;};
    int getOrigLineNo() const { return this->origLineno; };
	virtual bool equals(const DepGraphNode* pOther) const;
	virtual std::string dotNameVerbose(bool isModelled) const;
	static std::string escapeDot(const std::string escapeMe, const int limit);
	virtual ~DepGraphNode();
	virtual std::string dotName() const {throw std::runtime_error("pure virtual method");};
	virtual std::string comparableName() const {throw std::runtime_error("pure virtual method");};
	virtual std::string dotNameShort() const {throw std::runtime_error("pure virtual method");};
    virtual std::string dotFileName() const { return "";};
	virtual int getLine() const { return origLineno; };
	virtual DepGraphNode* clone() const { return new DepGraphNode(*this);};
    void setShape(std::string sh){ this->shape = sh; };
    std::string getShape() { return shape; };

protected:
	std::string fileName;
	int origLineno;
    std::string shape;
    void init();


private:
	// Node ID
	int id;
	// topological sort node
	int order;
	// strongly connected component id
	int sccID;
	// tells number of parents which has not been processed in computation yet
	int nonprocessedParents;

};


#endif /* DEPGRAPHNODE_HPP_ */

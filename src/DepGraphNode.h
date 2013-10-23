/*
 * DepGraphNode.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef DEPGRAPHNODE_H_
#define DEPGRAPHNODE_H_
#include "StringBuilder.h"
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

#endif /* DEPGRAPHNODE_H_ */

/*
 * Variable.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include "TacPlace.hpp"

using namespace std;

class Variable: public TacPlace {
public:
	Variable(){ name = funcName = ""; };
	Variable(std::string name, std::string funcName) : name(name), funcName(funcName) {};
	Variable(const Variable& other): name(other.name), funcName(other.funcName){};
	virtual ~Variable(){};
	std::string toString() const { return stringbuilder() << "Var: " << name << "\\nFunc: " << funcName;};
    std::string getName() const {return name;};
    std::string getFuncName() const { return funcName;};
	bool equals(const TacPlace* place) const {
			const Variable* o = dynamic_cast<const Variable*>(place);
			if (o == NULL) return false;
			return o->name == this->name;
	};
	Variable* clone() const { return new Variable(*this);};
private:
	std::string name;
    std::string funcName;
};


#endif /* VARIABLE_HPP_ */

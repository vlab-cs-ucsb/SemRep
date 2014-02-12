/*
 * Variable.hpp
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

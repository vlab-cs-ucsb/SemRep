/*
 * Literal.hpp
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

#ifndef LITERAL_HPP_
#define LITERAL_HPP_

#include "TacPlace.hpp"

class Literal: public TacPlace {
public:
	Literal(){ literalValue = ""; };
	Literal(std::string literalVal){ this->literalValue = literalVal;};
	Literal(const Literal& other):literalValue(other.literalValue){};
	virtual ~Literal(){};
	std::string toString() const {return this->literalValue;};
    std::string getLiteralValue() const { return this->literalValue;};
	bool equals(const TacPlace* place) const {
		const Literal* o = dynamic_cast<const Literal*>(place);
		if (o == NULL) return false;
		return o->literalValue == this->literalValue;
	};
	Literal* clone() const { return new Literal(*this);};

private:
	std::string literalValue;
};


#endif /* LITERAL_HPP_ */

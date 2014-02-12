/*
 * Constant.hpp
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

#ifndef CONSTANT_HPP_
#define CONSTANT_HPP_

#include "TacPlace.hpp"

using namespace std;

class Constant: public TacPlace {
public:
	Constant(){ label = ""; };
	Constant(string label){ this->label = label;};
	Constant(const Constant& other):label(other.label){};
	virtual ~Constant(){};
	string toString() const {return label;};
	bool equals(const TacPlace* place) const {
			const Constant* o = dynamic_cast<const Constant*>(place);
			if ( o == NULL) return false;
			return o->label == this->label;
	};
	Constant* clone() const { return new Constant(*this);};
private:
	string label;
};



#endif /* CONSTANT_HPP_ */

/*
 * Constant.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "TacPlace.h"
#include <iostream>
#include <exception>
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

#endif /* CONSTANT_H_ */
